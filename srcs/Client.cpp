/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 11:55:55 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/28 11:05:38 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client( int fd ) : _fd(fd), _user(""), _nick(""), _host(""), _real(""), _pass(""), _isAuth(false), _operator(false) {}

Client::~Client( void ) {}

void Client::setPass( const std::string& attPath ) {
	this->_pass = attPath;
}

void Client::setNick( const std::string& attNick ) {
	this->_nick = attNick;
}

void Client::setUser( const std::string& attUser ) {
	this->_user = attUser;
}

void Client::setAuth( bool attAuth ) {
	this->_isAuth = attAuth;
}

int Client::getFd( void ) const {
	return (this->_fd);
}

const bool& Client::getAuth( void ) const {
	return (this->_isAuth);
}

const std::string& Client::getNick( void ) const {
	return (this->_nick);
}

const std::string& Client::getUser( void ) const {
	return (this->_user);
}

const std::string& Client::getPass( void ) const {
	return (this->_pass);
}

std::string Client::getPrefix( void ) const {
	return (this->_nick + "!" + this->_user + "@localhost");
}

const bool& Client::getOp( void ) const {
	return (this->_operator);
}

void Client::setOp( bool op ) {
	this->_operator = op;
}

bool Client::checkAuth( void ) {
	if (!this->_pass.empty() && !this->_user.empty() && !this->_nick.empty()) {
		std::cout << "CLIENT CONNECTED" << std::endl;
		return (true);
	}
	return (false);
}

const char* Client::FormatException::what() const throw()
{
    return ("WRONG PASS");
}
