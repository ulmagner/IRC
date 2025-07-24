/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InviteCmd.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 17:21:30 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/24 17:23:57 by ulmagner         ###   ########.fr       */
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
	std::vector<Channel>& channels = this->_serv.getChannels();
	std::string name = this->_tokens[1];
	Channel* channel = NULL;
	for (size_t j = 0; j < channels.size(); ++j) {
		if (name == channels[j].getName()) {
			channel = &channels[j];
			break ;
		}
	}
	if (!channel) {
		this->sendToClient(client, "403", channel->getName() + " " + ERR_NOSUCHCHANNEL);
		throw InviteCmd::FormatException();
	}
	std::map<int, Client>& cl = channel->getClients();
	std::map<int, Client>::iterator it = cl.begin();
	bool is_i = false;
	for (;it != cl.end(); ++it) {
		if (it->second.getFd() == client.getFd())
			is_i = true;
	}
	if (!is_i) {
        this->sendToClient(client, "482", channel->getName() + " " + ERR_CHANOPRIVSNEEDED);
		throw InviteCmd::FormatException();
	}
}

void InviteCmd::sendToClient( Client& client, const std::string& code, const std::string& message ) {
	std::string fullMsg = "";
	if (code == "461")
		fullMsg = ":" + this->_serv._name + " " + code + client.getUser() + this->_tokens[0] + " " + message;
	else if (code == "482" || "403")
		fullMsg = ":" + this->_serv._name + " " + code + client.getUser() + message;
	else if (code == "441")
		fullMsg = ":" + this->_serv._name + " " + code + client.getNick() + message;
	send(client.getFd(), fullMsg.c_str(), fullMsg.size(), 0);
}

const char* InviteCmd::FormatException::what() const throw()
{
	return ("Invite FORMAT");
}
