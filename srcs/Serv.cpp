/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Serv.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 11:58:33 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/23 10:37:52 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Serv.hpp"
#include "PassCmd.hpp"
#include "NickCmd.hpp"
#include "UserCmd.hpp"
#include "JoinCmd.hpp"

Serv::Serv( char **arg ) : _socketfd(0), _epollfd(0) {
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
    std::map<int, Client>::const_iterator it = this->_connections.begin();
    for (; it != this->_connections.end(); ++it) {
        int fd = it->second.getFd();
        if (fd != this->_socketfd && fd != -1) {
            close(fd);
            fd = -1;
        }
    }
    this->_connections.clear();
    if (this->_epollfd != -1)
        close(this->_epollfd);
    if (this->_socketfd != -1)
        close(this->_socketfd);
    this->_epollfd = -1;
    this->_socketfd = -1;
}

ACmd*	Serv::pass( std::vector<std::string> tokens )
{
	return (new PassCmd(tokens, *this));
}

ACmd*	Serv::nick( std::vector<std::string> tokens )
{
	return (new NickCmd(tokens, *this));
}

ACmd*	Serv::user( std::vector<std::string> tokens )
{
	return (new UserCmd(tokens, *this));
}

ACmd*	Serv::join( std::vector<std::string> tokens )
{
	return (new JoinCmd(tokens, *this));
}

const std::string& Serv::getPass( void ) const {
	return (this->_pass);
}

const std::map<int, Client>& Serv::getConnections( void ) const {
    return (this->_connections);
}

std::vector<Channel>& Serv::getChannels( void ){
    return (this->_channels);
}

ACmd* Serv::getCmd( char* buffer, Client& client ) {
	std::string auth[] = {"PASS", "NICK", "USER", "JOIN"};

    std::stringstream ss(buffer);
    std::string word;
    std::vector<std::string> tokens;

    while (ss >> word) {
        tokens.push_back(word);
    }

	ACmd* (Serv::*cmds[])( std::vector<std::string> ) = {
		&Serv::pass,
        &Serv::nick,
        &Serv::user,
        &Serv::join,
	};

    if (!client.getAuth()) {
        for (int i = 0; i < 3; i++) {
            if (auth[i].compare( tokens[0] ) == 0) {
                std::cout << "]]]]]]]]]" << std::endl;
                return (this->*cmds[i])(tokens);
            }
        }
    }
    std::cout << ",,,,,,," << std::endl;
    std::cout << client.getAuth() << std::endl;
    std::cout << "........" << std::endl;
    for (size_t i = 0; i < sizeof(cmds) / sizeof(cmds[0]); i++) {
        if (!client.getAuth() && i >= 3 && auth[i].compare( tokens[0] ) == 0) {
            std::cout << "!!!!!!!" << std::endl;
            throw Serv::NotAuthYetException();
        }
        if (client.getAuth() == true && i < 3 && auth[i].compare( tokens[0] ) == 0) {
            std::cout << "?????????" << std::endl;
            throw Serv::AlreadyAuthenticateException();
        }
        else if (auth[i].compare( tokens[0] ) == 0) {
            return (this->*cmds[i])(tokens);
        }
    }
    throw Serv::CmdNotFoundException();
    return (NULL);
}

Client& Serv::getClientByFd( int fd ) {
    for (std::map<int, Client>::iterator it = this->_connections.begin(); it != this->_connections.end(); ++it) {
        if (it->first == fd)
            return (it->second);
    }
    throw Serv::ErrorException();
}

void Serv::run( void ) {
    createTcpServerSocket();
    fcntl(this->_socketfd, F_SETFL, O_NONBLOCK);
    struct epoll_event ev, events[MAX_EVENTS];
    int nfds, clientSocket;
    this->_connections.insert(std::make_pair(this->_socketfd, Client(this->_socketfd)));
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
                this->_connections.insert(std::make_pair(clientSocket, Client(clientSocket)));
                if (epoll_ctl(this->_epollfd, EPOLL_CTL_ADD, clientSocket, &ev) == -1) {
                    perror("epoll_ctl: clientSocket");
                    // exit(EXIT_FAILURE);
                    continue ;
                }
            }
            else {
                char buffer[MAX_EVENTS] = {0};
                ssize_t bytes = recv(events[n].data.fd, buffer, sizeof(buffer), 0);
                if (!bytes) {
                    std::cout << "Server closed connection." << std::endl;
                    close(events[n].data.fd);
                    epoll_ctl(this->_epollfd, EPOLL_CTL_DEL, events[n].data.fd, NULL);
                    this->_connections.erase(events[n].data.fd);
                }
                else if (bytes < 0) {
                    std::cout << "Client " << events[n].data.fd << " disconnected." << std::endl;
                    close(events[n].data.fd);
                    epoll_ctl(this->_epollfd, EPOLL_CTL_DEL, events[n].data.fd, NULL);
                    this->_connections.erase(events[n].data.fd);
                }
                else {
                    std::cout << "Message from client: " << buffer << std::endl;
                    ACmd* cmd = NULL;
                    try {
                        Client& client = this->getClientByFd(events[n].data.fd);
                        cmd = Serv::getCmd(buffer, client);
                        cmd->executeCmd(client);
                        if (!client.getAuth())
                            client.setAuth(client.checkAuth());
                    }
                    catch (const std::exception& e) {
                        std::cout << e.what() << std::endl;
                    }
                    delete cmd;
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

const char* Serv::ErrorException::what() const throw()
{
    return ("Error");
}

const char* Serv::CmdNotFoundException::what() const throw()
{
    return ("CMD NOT FOUND.");
}

const char* Serv::AlreadyAuthenticateException::what() const throw()
{
    return ("CLIENT ALREADY AUTHENTICATE.");
}

const char* Serv::NotAuthYetException::what() const throw()
{
    return ("CLIENT NOT AUTHENTICATE YET.");
}
