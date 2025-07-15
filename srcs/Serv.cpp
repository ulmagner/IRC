/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Serv.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 11:58:33 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/15 15:25:14 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Serv.hpp"

Serv::Serv( char **arg ) : _socketfd(0), _epollfd(-1) {
    this->_port = this->isValidPort(arg[1]);
    this->isValidPass(arg[2]);
    this->_pass = arg[2];
}

Serv::~Serv( void ) {}

void Serv::run( void ) {
    this->_socketfd = socket(AF_INET, SOCK_STREAM, this->_epollfd);

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(this->_port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    
    bind(this->_socketfd, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

    listen(this->_socketfd, 10);

    int clientSocket = accept(this->_socketfd, nullptr, nullptr);

    
    
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
