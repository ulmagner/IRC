/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PartCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 18:43:44 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/30 20:27:45 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PartCmd.hpp"
#include "Serv.hpp"

PartCmd::PartCmd( std::vector<std::string> tokens, Serv& serv ) : ACmd(tokens[0]), _tokens(tokens), _serv(serv) {}

PartCmd::~PartCmd( void ) {}

void PartCmd::executeCmd( Client& client ) {
	std::string m = "";
	if (this->_tokens.size() < 2) {
		m = ERR_NEEDMOREPARAMS(client.getNick(), this->_tokens[0]);
		send(client.getFd(), m.c_str(), m.size(), 0);
		throw PartCmd::FormatException();
	}
	std::vector<std::string> chan = split(this->_tokens[1], ',');
	std::string reason = (this->_tokens.size() == 3) ? this->_tokens[2] : "Leaving";
	if (reason[0] != ':')
		reason = ":" + reason;
	std::vector<std::string>::const_iterator itt = chan.begin();
	std::string name = "";
	for (;itt != chan.end(); ++itt) {
		name = *itt;
		if (name[0] != '#') {
			m = ERR_NOSUCHCHANNEL(client.getNick(), name);
			send(client.getFd(), m.c_str(), m.size(), 0);
			continue ;
		}
		Channel* channel = this->_serv.getChannelByName(name);
		if (!channel) {
			m = ERR_NOSUCHCHANNEL(client.getNick(), name);
			send(client.getFd(), m.c_str(), m.size(), 0);
			continue ;
		}
		if (!channel->getClientByName(client.getNick())) {
			m = ERR_NOTONCHANNEL(client.getNick(), channel->getName());
			send(client.getFd(), m.c_str(), m.size(), 0);
			continue ;
		}
		std::string msg = ":" + client.getPrefix() + " PART " + channel->getName() + " " + reason + "\r\n";
		sendToChannelClient(channel, msg);
		channel->removeClient( client.getNick() );
	}
}

const char* PartCmd::FormatException::what() const throw()
{
    return ("WRONG PASSs");
}
