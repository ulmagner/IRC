/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InviteCmd.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 17:21:30 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/30 20:44:22 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "InviteCmd.hpp"
#include "Serv.hpp"

InviteCmd::InviteCmd( std::vector<std::string> tokens, Serv& serv ) : ACmd(tokens[0]), _tokens(tokens), _serv(serv) {}

InviteCmd::~InviteCmd( void ) {}

void InviteCmd::executeCmd( Client& client ) {
	std::string m = "";
	if (this->_tokens.size() < 3) {
		m = ERR_NEEDMOREPARAMS(client.getNick(), this->_tokens[0]);
		send(client.getFd(), m.c_str(), m.size(), 0);
		throw InviteCmd::FormatException();
	}
	std::string name = this->_tokens[2];
	if (name[0] != '#') {
		m = ERR_NOSUCHCHANNEL(client.getNick(), name);
		send(client.getFd(), m.c_str(), m.size(), 0);
		throw InviteCmd::FormatException();
	}
	Channel* channel = this->_serv.getChannelByName(name);
	if (!channel) {
		m = ERR_NOSUCHCHANNEL(client.getNick(), name);
		send(client.getFd(), m.c_str(), m.size(), 0);
		throw InviteCmd::FormatException();
	}
	if (channel && channel->getClients().size() < 1) {
		m = ERR_NOSUCHCHANNEL(client.getNick(), name);
		send(client.getFd(), m.c_str(), m.size(), 0);
		throw InviteCmd::FormatException();
	}
	std::map<int, std::pair<Client *, int> >& cl = channel->getClients();
	std::map<int, std::pair<Client *, int> >::iterator it = cl.begin();
	int is_i = 0;
	Client* toInvite = NULL;
	for (;it != cl.end(); ++it) {
		if (it->second.first->getFd() == client.getFd()) {
			is_i = 1;
			if (it->first == 1) {
				is_i = 2;
			}
		}
		if (it->second.first->getNick() == this->_tokens[1]) {
			m = ERR_USERONCHANNEL(client.getNick(), this->_tokens[1], channel->getName());
			send(client.getFd(), m.c_str(), m.size(), 0);
			throw InviteCmd::FormatException();
		}
	}
	if (!is_i) {
		m = ERR_NOTONCHANNEL(client.getNick(), channel->getName());
		send(client.getFd(), m.c_str(), m.size(), 0);
	}
	else if (is_i == 1 && channel->getMode("+i")) {
		m = ERR_CHANOPRIVSNEEDED(client.getNick(), client.getUser(), channel->getName());
		send(client.getFd(), m.c_str(), m.size(), 0);
		throw InviteCmd::FormatException();
	}
	toInvite = this->_serv.getClientByName(this->_tokens[1]);
	if (!toInvite) {
		m = ERR_NOSUCHNICK(client.getNick(), this->_tokens[1]);
		send(client.getFd(), m.c_str(), m.size(), 0);
		throw InviteCmd::FormatException();
	}
	if (channel->getClientByName(toInvite->getNick())) {
		m = ERR_USERONCHANNEL(client.getNick(), toInvite->getNick(), channel->getName());
		send(client.getFd(), m.c_str(), m.size(), 0);
		throw InviteCmd::FormatException();
	}
	channel->addToInvite(*toInvite);
	// std::string msgRpl = "";
	// msgRpl = ":" + this->_serv._name + " 341 " + client.getNick() + " " + toInvite->getNick() + " " + channel->getName() + "\r\n";
	m = RPL_INVITING(client.getNick(), toInvite->getNick(), channel->getName());
	send(client.getFd(), m.c_str(), m.size(), 0);
	// msgRpl = ":" + client.getPrefix() + " INVITE " + toInvite->getNick() + " :" + channel->getName() + "\r\n";
	m = SEND_INVITE(client.getNick(), client.getUser(), toInvite->getNick(), channel->getName());
	send(toInvite->getFd(), m.c_str(), m.size(), 0);
}

const char* InviteCmd::FormatException::what() const throw()
{
	return ("Invite FORMAT");
}
