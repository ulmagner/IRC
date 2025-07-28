/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 13:51:29 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/28 14:05:41 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel( std::string& name, std::string& key, Client& client ) : _name(name), _key(key) {
	this->_clientConnected.insert(std::make_pair(1, client));
	client.setOp(true);
}

Channel::~Channel( void ) {}

void Channel::setName( const std::string& attName ) {
	this->_name = attName;
}

void Channel::setKey( const std::string& attKey ) {
	this->_key = attKey;
}

void Channel::addClient( Client& client ) {
	this->_clientConnected.insert(std::make_pair(0, client));
}

const std::string& Channel::getName( void ) const {
	return (this->_name);
}

const std::string& Channel::getKey( void ) const {
	return (this->_key);
}

bool Channel::hasAlreadyJoin( int fd ) {
	std::map<int, Client>::const_iterator it = this->getClients().begin();
	for (;it != this->getClients().end(); ++it) {
		if (fd == it->second.getFd())
			return (true);
	}
	return (false);
}

std::map<int, Client>& Channel::getClients( void ) {
	return (this->_clientConnected);
}

const char* Channel::FormatException::what() const throw()
{
    return ("WRONG PASS");
}
