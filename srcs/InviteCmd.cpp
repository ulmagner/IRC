/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InviteCmd.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 17:21:30 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/29 16:02:49 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "InviteCmd.hpp"
#include "Serv.hpp"

InviteCmd::InviteCmd( std::vector<std::string> tokens, Serv& serv ) : ACmd(tokens[0]), _tokens(tokens), _serv(serv) {}

InviteCmd::~InviteCmd( void ) {}

void InviteCmd::executeCmd( Client& client ) {
	if (this->_tokens.size() < 3 || this->_tokens.size() > 4) {
		this->sendToClient(client, "461", ERR_NEEDMOREPARAMS);
		throw InviteCmd::FormatException();
	}
	std::string name = this->_tokens[2];
	Channel* channel = this->_serv.getChannelByName(name);
	if (!channel) {
		this->sendToClient(client, "403", channel->getName() + ERR_NOSUCHCHANNEL);
		throw InviteCmd::FormatException();
	}
	if (channel && channel->getClients().size() < 1) {
		this->sendToClient(client, "403", channel->getName() + ERR_NOSUCHCHANNEL);
		throw InviteCmd::FormatException();
	}
	std::map<int, Client>& cl = channel->getClients();
	std::map<int, Client>::iterator it = cl.begin();
	int is_i = 0;
	Client* toInvite = NULL;
	for (;it != cl.end(); ++it) {
		if (it->second.getFd() == client.getFd()) {
			is_i = 1;
			if (it->first == 1) {
				is_i = 2;
			}
		}
		if (it->second.getNick() == this->_tokens[1]) {
			this->sendToClient(client, "443", channel->getName() + ERR_USERONCHANNEL);
			throw InviteCmd::FormatException();
		}
	}
	if (!is_i) {
        this->sendToClient(client, "442", channel->getName() + ERR_NOTONCHANNEL);
	}
	else if (is_i == 1 && channel->getMode("+i")) {
        this->sendToClient(client, "482", channel->getName() + ERR_CHANOPRIVSNEEDED);
		throw InviteCmd::FormatException();
	}
	toInvite = this->_serv.getClientByName(this->_tokens[1]);
	if (!toInvite) {
		this->sendToClient(client, "401", ERR_NOSUCHNICK);
		throw InviteCmd::FormatException();
	}
	if (channel->getClientByName(toInvite->getNick())) {
		this->sendToClient(client, "443", channel->getName() + ERR_USERONCHANNEL);
		throw InviteCmd::FormatException();
	}
	channel->addToInvite(*toInvite);
	std::string msgRpl = "";
	msgRpl = ":" + this->_serv._name + " 341 " + client.getNick() + " " + toInvite->getNick() + " " + channel->getName() + "\r\n";
	send(client.getFd(), msgRpl.c_str(), msgRpl.size(), 0);
	msgRpl = ":" + client.getPrefix() + " INVITE " + toInvite->getNick() + " :" + channel->getName() + "\r\n";
	send(toInvite->getFd(), msgRpl.c_str(), msgRpl.size(), 0);
}

void InviteCmd::sendToClient( Client& client, const std::string& code, const std::string& message ) {
	std::string fullMsg = ":" + this->_serv._name + " " + code + " ";
	if (code == "461")
		fullMsg += this->_tokens[0] + " " + message;
	else if (code == "482" || code == "403" || code == "442" || code == "401")
		fullMsg += message;
	else if (code == "441" || code == "443")
		fullMsg += client.getNick() + " " + message;
	send(client.getFd(), fullMsg.c_str(), fullMsg.size(), 0);
}

const char* InviteCmd::FormatException::what() const throw()
{
	return ("Invite FORMAT");
}
