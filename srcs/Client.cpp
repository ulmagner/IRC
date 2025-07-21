/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 11:55:55 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/21 14:13:03 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client( int fd ) : _fd(fd), _user(""), _nick(""), _host(""), _real(""), _pass(""), isAuth(false) {}

Client::~Client( void ) {}

void Client::setPath( const std::string& attPath ) {
	this->_pass = attPath;
}

int Client::getFd( void ) const {
	return (this->_fd);
}

const std::string& Client::getPass( void ) const {
	return (this->_pass);
}

const char* Client::FormatException::what() const throw()
{
    return ("WRONG PASS");
}
