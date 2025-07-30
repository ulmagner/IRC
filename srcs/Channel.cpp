/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 13:51:29 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/30 18:28:23 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel( std::string& name, std::string& key, Client& client ) : _name(name), _key(key), _topic(""), _topicSetter(""), _topicSetTime(0) {
	this->_clientConnected[client.getFd()] = std::make_pair(&client, 1);
	client.setOp(true);
}

Channel::~Channel( void ) {
	this->_mode.clear();
    this->_clientConnected.clear();
	this->_inviteClient.clear();
}

void Channel::setName( const std::string& attName ) {
	this->_name = attName;
}

void Channel::setKey( const std::string& attKey ) {
	this->_key = attKey;
}

void Channel::addClient( Client& client ) {
	this->_clientConnected[client.getFd()] = std::make_pair(&client, 0);
}

const std::string& Channel::getName( void ) const {
	return (this->_name);
}

const std::string& Channel::getKey( void ) const {
	return (this->_key);
}

const std::string& Channel::getTopic( void ) const {
	return (this->_topic);
}

const std::string& Channel::getTopicSetter( void ) const {
	return (this->_topicSetter);
}

bool Channel::getMode( const std::string& mode ) const {
	std::vector<std::string>::const_iterator it = this->_mode.begin();
	for(;it != this->_mode.end();) {
		if (*it == mode)
			return (true);
	}
	return (false);
}

void Channel::setTopicSetTime( time_t time ) {
	this->_topicSetTime = time;
}

time_t Channel::getTopicSetTime( void ) const {
	return (this->_topicSetTime);
}

void Channel::setTopicSetter( const std::string& nick ) {
	this->_topicSetter = nick;
}

void Channel::addMode( const std::string& mode ) {
	this->_mode.push_back(mode);
}

void Channel::addToInvite( Client& client ) {
	this->_inviteClient.push_back(&client);
}

void Channel::eraseFromInvite( const std::string& nick ) {
	std::vector<Client*>::iterator it = this->_inviteClient.begin();
	for (; it != this->_inviteClient.end();) {
		if ((*it)->getNick() == nick)
			it = this->_inviteClient.erase(it);
		else
			++it;
	}
}

void Channel::setTopic( const std::string& topic )  {
	this->_topic = topic;
}

void Channel::removeClient( const std::string& nick ) {
	std::map<int, std::pair<Client*, int> >::iterator it = this->_clientConnected.begin();
	for (; it != this->_clientConnected.end();) {
		if (nick == it->second.first->getNick()) {
			std::map<int, std::pair<Client*, int> >::iterator toRemove = it++;
			this->_clientConnected.erase(toRemove);
		}
		else
			++it;
	}
}

bool Channel::hasPerm( Client& client ) const {
	int fd = client.getFd();
	std::map<int, std::pair<Client*, int> >::const_iterator it = this->_clientConnected.find(fd);
	if (it != this->_clientConnected.end()) {
		return (it->second.second == 1);
	}
	return (false);
}

Client* Channel::getClientByName( const std::string& name ) {
	std::map<int, std::pair<Client*, int> >::const_iterator it = this->_clientConnected.begin();
	for (;it != this->_clientConnected.end(); ++it) {
		if (name == it->second.first->getNick()) {
			return (it->second.first);
		}
	}
	return (NULL);
}

const Client* Channel::getInvite( const std::string& name ) const {
	std::vector<Client*>::const_iterator it = this->_inviteClient.begin();
	for (;it != this->_inviteClient.end(); ++it) {
		if (name == (*it)->getNick()) {
			return (*it);
		}
	}
	return (NULL);
}

bool Channel::hasAlreadyJoin( int fd ) {
	std::map<int, std::pair<Client*, int> >::const_iterator it = this->getClients().find(fd);
	return (it != this->getClients().end());
}

std::map<int, std::pair<Client*, int> >& Channel::getClients( void ) {
	return (this->_clientConnected);
}

const char* Channel::FormatException::what() const throw()
{
    return ("WRONG PASS");
}
