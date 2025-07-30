/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TopicCmd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 12:06:46 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/30 13:05:19 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TopicCmd.hpp"
#include "Serv.hpp"

TopicCmd::TopicCmd( std::vector<std::string> tokens, Serv& serv ) : ACmd(tokens[0]), _tokens(tokens), _serv(serv) {}

TopicCmd::~TopicCmd( void ) {}

void TopicCmd::executeCmd( Client& client ) {
	if (this->_tokens.size() < 1) {
		this->_serv.sendToClient(client, "461", " " + this->_tokens[0] + ERR_NEEDMOREPARAMS);
		throw TopicCmd::FormatException();
	}
	std::string name = this->_tokens[1];
	if (name[0] != '#') {
		this->_serv.sendToClient(client, "403", " " + name + ERR_NOSUCHCHANNEL);
		throw TopicCmd::FormatException();
	}
	Channel* channel = this->_serv.getChannelByName(name);
	if (!channel) {
		this->_serv.sendToClient(client, "403", " " + channel->getName() + ERR_NOSUCHCHANNEL);
		throw TopicCmd::FormatException();
	}
	if (!channel->getClientByName(client.getNick())) {
		this->_serv.sendToClient(client, "442", " " + channel->getName() + ERR_NOTONCHANNEL);
		throw TopicCmd::FormatException();
	}
	if (this->_tokens.size() == 2) {
		if (channel->getTopic().empty()) {
			this->_serv.sendToClient(client, "331", " " + name + RPL_NOTOPIC);
		} else {
			this->_serv.sendToClient(client, "332", " " + name + " :" + channel->getTopic());
			std::ostringstream oss;
			oss << channel->getTopicSetTime();
			std::string str = oss.str();
			this->_serv.sendToClient(client, "333", " " + name + " " + channel->getTopicSetter() + " " + str);
		}
		return ;
	}
	if (channel->getMode("+t") && !channel->hasPerm(client)) {
        this->_serv.sendToClient(client, "482", " " + channel->getName() + ERR_CHANOPRIVSNEEDED);
		throw TopicCmd::FormatException();
	}
	channel->setTopicSetter(client.getNick());
	channel->setTopic(this->_tokens[2]);
	channel->setTopicSetTime(std::time(NULL));
    std::string msg = ":" + client.getPrefix() + " TOPIC " + channel->getName() + " :" + channel->getTopic() + "\r\n";
    sendToChannelClient(channel, msg);
}

const char* TopicCmd::FormatException::what() const throw()
{
    return ("WRONG PASSs");
}
