/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 13:51:29 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/22 14:08:07 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel( std::string& name, std::string& key, Client& client ) : _name(name), _key(key) {
	this->_clientConnected.insert(std::make_pair(client.getFd(), client));
}

Channel::~Channel( void ) {}

void Channel::setName( const std::string& attName ) {
	this->_name = attName;
}

void Channel::setKey( const std::string& attKey ) {
	this->_key = attKey;
}

const std::string& Channel::getName( void ) const {
	return (this->_name);
}

const std::string& Channel::getKey( void ) const {
	return (this->_key);
}

const char* Channel::FormatException::what() const throw()
{
    return ("WRONG PASS");
}
