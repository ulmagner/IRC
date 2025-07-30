/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PartCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 18:43:44 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/30 11:56:28 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PartCmd.hpp"
#include "Serv.hpp"

PartCmd::PartCmd( std::vector<std::string> tokens, Serv& serv ) : ACmd(tokens[0]), _tokens(tokens), _serv(serv) {}

PartCmd::~PartCmd( void ) {}

void PartCmd::executeCmd( Client& client ) {
	if (this->_tokens.size() < 1 || this->_tokens.size() > 3) {
		this->_serv.sendToClient(client, "461", " " + this->_tokens[0] + ERR_NEEDMOREPARAMS);
		throw PartCmd::FormatException();
	}
	Channel* channel = this->_serv.getChannelByName(this->_tokens[1]);
	if (!channel) {
		this->_serv.sendToClient(client, "403", " " + channel->getName() + ERR_NOSUCHCHANNEL);
		throw PartCmd::FormatException();
	}
	if (!channel->getClientByName(client.getNick())) {
		this->_serv.sendToClient(client, "442", " " + channel->getName() + ERR_NOTONCHANNEL);
		throw PartCmd::FormatException();
	}
	if (this->_tokens.size() == 2) {
		if (channel->getTopic().empty()) {
			this->_serv.sendToClient(client, "331", " " + this->_tokens[1] + RPL_NOTOPIC);
		} else {
			this->_serv.sendToClient(client, "332", " " + this->_tokens[1] + " :" + channel->getTopic());
			std::ostringstream oss;
			oss << channel->getTopicSetTime();
			std::string str = oss.str();
			this->_serv.sendToClient(client, "333", " " + this->_tokens[1] + " " + channel->getTopicSetter() + " " + str);
		}
		return ;
	}
	if (channel->getMode("+t") && !channel->hasPerm(client)) {
        this->_serv.sendToClient(client, "482", " " + channel->getName() + ERR_CHANOPRIVSNEEDED);
		throw PartCmd::FormatException();
	}
	channel->setTopicSetter(client.getNick());
	channel->setTopic(this->_tokens[2]);
	channel->setTopicSetTime(std::time(NULL));
    std::string msg = ":" + client.getPrefix() + " TOPIC " + channel->getName() + " :" + channel->getTopic() + "\r\n";
    sendToChannelClient(channel, msg);
}

const char* PartCmd::FormatException::what() const throw()
{
    return ("WRONG PASSs");
}
