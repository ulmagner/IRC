/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Serv.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 11:58:33 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/17 14:02:59 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Serv.hpp"

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

void Serv::run( void ) {
    createTcpServerSocket();
    fcntl(this->_socketfd, F_SETFL, O_NONBLOCK);
    struct epoll_event ev, events[MAX_EVENTS];
    this->_connections.push_back(this->_socketfd);
    int nfds, clientSocket;
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
                    exit(EXIT_FAILURE);
                }
            }
            else {
                char buffer[MAX_EVENTS] = {0};
                recv(events[n].data.fd, buffer, sizeof(buffer), 0);
                std::cout << "Message from client: " << buffer << std::endl;
            }
        }
    }
    close(this->_socketfd);
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
