/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Serv.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 11:58:33 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/18 17:57:42 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Serv.hpp"

Serv::Serv( char **arg ) : _socketfd(0), _epollfd(0), _cmd(NULL) {
    this->_port = this->isValidPort(arg[1]);
    this->isValidPass(arg[2]);
    this->_pass = arg[2];
}

Serv::~Serv( void ) {}

void Serv::createTcpServerSocket( void ) {
    this->_socketfd = socket(AF_INET, SOCK_STREAM, this->_epollfd);

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(this->_port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    bind(this->_socketfd, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

    listen(this->_socketfd, 10);
}

void Serv::shutdown( void ) {
    std::cout << this->_connections.size() << std::endl;
    std::vector<int>::const_iterator it = this->_connections.begin();
    for (; it != this->_connections.end(); ++it) {
        if (*it != this->_socketfd && *it != -1) {
            close(*it);
        }
    }
    if (this->_cmd)
        delete this->_cmd;
    if (this->_epollfd != -1)
        close(this->_epollfd);
    if (this->_socketfd != -1)
        close(this->_socketfd);
}

ACmd*	Serv::pass( std::vector<std::string> tokens ) const
{
	return (new PassCmd(tokens));
}

ACmd* Serv::getCmd( char* buffer ) const {
	std::string levels[] = {"PASS"};

	std::stringstream ss(buffer);
    std::string word;
    std::vector<std::string> tokens;

    while (ss >> word) {
        tokens.push_back(word);
    }

	ACmd* (Serv::*cmds[])( std::vector<std::string> ) const = {
		&Serv::pass,
	};

	for (int i = 0; i < 1; i++) {
		if (levels[i].compare( tokens[0] ) == 0) {
			std::cout << "Serv creates " << levels[i] << std::endl;
			return (this->*cmds[i])(tokens);
		}
	}

    throw Serv::CmdNotFoundException();
    return (NULL);
}

void Serv::run( void ) {
    createTcpServerSocket();
    fcntl(this->_socketfd, F_SETFL, O_NONBLOCK);
    struct epoll_event ev, events[MAX_EVENTS];
    int nfds, clientSocket;
    this->_connections.push_back(this->_socketfd);
    this->_epollfd = epoll_create1(0);
    if (this->_epollfd == -1) {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }
    ev.events = EPOLLIN;
    ev.data.fd = this->_socketfd;
    if (epoll_ctl(this->_epollfd, EPOLL_CTL_ADD, this->_socketfd, &ev) == -1) {
        perror("epoll_ctl: this->_socketfd");
        exit(EXIT_FAILURE);
    }
    for (;;) {
        nfds = epoll_wait(this->_epollfd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            perror("epoll_wait");
            exit(EXIT_FAILURE);
        }

        for (int n = 0; n < nfds; ++n) {
            if (events[n].data.fd == this->_socketfd) {
                sockaddr_in clientAddr;
                socklen_t addrlen = sizeof(clientAddr);
                clientSocket = accept(events[n].data.fd, (struct sockaddr*)&clientAddr, &addrlen);
                if (clientSocket == -1) {
                    perror("accept");
                    exit(EXIT_FAILURE);
                }
                fcntl(clientSocket, F_SETFL, O_NONBLOCK);
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = clientSocket;
                this->_connections.push_back(clientSocket);
                if (epoll_ctl(this->_epollfd, EPOLL_CTL_ADD, clientSocket, &ev) == -1) {
                    perror("epoll_ctl: clientSocket");
                    // exit(EXIT_FAILURE);
                    continue ;
                }
                std::cout << "CLIENT CONNECTED" << std::endl;
            }
            else {
                char buffer[MAX_EVENTS] = {0};
                ssize_t bytes = recv(events[n].data.fd, buffer, sizeof(buffer), 0);
                if (!bytes) {
                    std::cout << "Server closed connection." << std::endl;
                    close(events[n].data.fd);
                    epoll_ctl(this->_epollfd, EPOLL_CTL_DEL, events[n].data.fd, NULL);
                    this->_connections.erase(
                        std::remove(this->_connections.begin(), this->_connections.end(), events[n].data.fd),
                        this->_connections.end());
                }
                else if (bytes < 0) {
                    std::cout << "Client " << events[n].data.fd << " disconnected." << std::endl;
                    close(events[n].data.fd);
                    epoll_ctl(this->_epollfd, EPOLL_CTL_DEL, events[n].data.fd, NULL);
                    this->_connections.erase(
                        std::remove(this->_connections.begin(), this->_connections.end(), events[n].data.fd),
                        this->_connections.end());
                }
                else {
                    std::cout << "Message from client: " << buffer << std::endl;
                    try {
                        this->_cmd = Serv::getCmd(buffer);
                        this->_cmd->executeCmd();
                    }
                    catch (const std::exception& e) {
                        std::cout << e.what() << std::endl;
                    }
                }
            }
        }
    }
}

int Serv::isValidPort( const std::string& port ) const {
    if (port.size() > 5 || port.size() < 4)
        throw Serv::FormatException();
    for (size_t i = 0; i < port.size(); i++) {
        if (!isdigit(port[i]))
            throw Serv::FormatException();
    }
    int p = std::atoi(port.c_str());
    if (p < 1025 || p > 65535)
        throw Serv::FormatException();
    return (p);
}

void Serv::isValidPass( const std::string& pass ) const {
    if (pass.size() > 10 || pass.size() < 4)
        throw Serv::FormatException();
}

const char* Serv::FormatException::what() const throw()
{
    return ("Error");
}

const char* Serv::CmdNotFoundException::what() const throw()
{
    return ("CMD NOT FOUND.");
}
