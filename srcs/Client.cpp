/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 11:55:55 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/31 19:11:43 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client( int fd ) : _fd(fd), _user(""), _nick(""), _host(""), _real(""), _pass(""), _isAuth(false), _operator(false), _money(1000) {}

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

std::pair<Card, Card>& Client::getCards( void ) {
	return (_cards);
}

void Client::setCards( const std::pair<Card, Card>& cards ) {
	this->_cards = cards;
}

void Client::setPlaying( bool s ) {
	this->_playing = s;
}

bool Client::getPlaying( void ) {
	return (this->_playing);
}

void Client::bet( int bet ) {
	this->_money -= bet;
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

bool Client::hasMode( const std::string& mode ) const {
	std::vector<std::string>::const_iterator it = this->_mode.begin();
	for(;it != this->_mode.end();) {
		if (*it == mode)
			return (true);
	}
	return (false);
}

void Client::addMode( const std::string& mode ) {
	std::vector<std::string>::iterator it = this->_mode.begin();
	for (; it != this->_mode.end(); ++it) {
		if (*it == mode)
			return ;
	}
	this->_mode.push_back(mode);
}

void Client::removeMode( const std::string& mode ) {
	std::vector<std::string>::iterator it = this->_mode.begin();
	for (; it != this->_mode.end();) {
		if ((*it)[1] == mode[1])
			it = this->_mode.erase(it);
		else
			++it;
	}
}

std::string Client::getMode( void ) {
	std::string m = "";
	std::vector<std::string>::iterator it = this->_mode.begin();
	for (; it != this->_mode.end(); ++it) {
		if (it == this->_mode.begin())
			m += "+";
		m += (*it).substr(1);
	}
	return (m);
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
