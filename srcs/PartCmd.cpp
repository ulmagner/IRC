/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PartCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 18:43:44 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/30 16:33:48 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PartCmd.hpp"
#include "Serv.hpp"

PartCmd::PartCmd( std::vector<std::string> tokens, Serv& serv ) : ACmd(tokens[0]), _tokens(tokens), _serv(serv) {}

PartCmd::~PartCmd( void ) {}

void PartCmd::executeCmd( Client& client ) {
	if (this->_tokens.size() < 2) {
		this->_serv.sendToClient(client, "461", " " + this->_tokens[0] + ERR_NEEDMOREPARAMS);
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
			this->_serv.sendToClient(client, "403", " " + name + ERR_NOSUCHCHANNEL);
			continue ;
		}
		Channel* channel = this->_serv.getChannelByName(name);
		if (!channel) {
			this->_serv.sendToClient(client, "403", " " + name + ERR_NOSUCHCHANNEL);
			continue ;
		}
		if (!channel->getClientByName(client.getNick())) {
			this->_serv.sendToClient(client, "442", " " + channel->getName() + ERR_NOTONCHANNEL);
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
