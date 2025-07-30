/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TopicCmd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 12:06:46 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/30 20:52:32 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TopicCmd.hpp"
#include "Serv.hpp"

TopicCmd::TopicCmd( std::vector<std::string> tokens, Serv& serv ) : ACmd(tokens[0]), _tokens(tokens), _serv(serv) {}

TopicCmd::~TopicCmd( void ) {}

void TopicCmd::executeCmd( Client& client ) {
	std::string m = "";
	if (this->_tokens.size() < 2) {
		m = ERR_NEEDMOREPARAMS(client.getNick(), this->_tokens[0]);
		send(client.getFd(), m.c_str(), m.size(), 0);
		throw TopicCmd::FormatException();
	}
	std::string name = this->_tokens[1];
	if (name[0] != '#') {
		m = ERR_NOSUCHCHANNEL(client.getNick(), name);
		send(client.getFd(), m.c_str(), m.size(), 0);
		throw TopicCmd::FormatException();
	}
	Channel* channel = this->_serv.getChannelByName(name);
	if (!channel) {
		m = ERR_NOSUCHCHANNEL(client.getNick(), name);
		send(client.getFd(), m.c_str(), m.size(), 0);
		throw TopicCmd::FormatException();
	}
	if (!channel->getClientByName(client.getNick())) {
		m = ERR_NOTONCHANNEL(client.getNick(), channel->getName());
		send(client.getFd(), m.c_str(), m.size(), 0);
		throw TopicCmd::FormatException();
	}
	if (this->_tokens.size() == 2) {
		if (channel->getTopic().empty()) {
			m = RPL_NOTOPIC(client.getNick(), name);
			send(client.getFd(), m.c_str(), m.size(), 0);
		} else {
			m = RPL_TOPIC(client.getNick(), name, channel->getTopic());
			send(client.getFd(), m.c_str(), m.size(), 0);
			// this->_serv.sendToClient(client, "332", " " + name + " :" + channel->getTopic());
			std::ostringstream oss;
			oss << channel->getTopicSetTime();
			std::string str = oss.str();
			this->_serv.sendToClient(client, "333", " " + name + " " + channel->getTopicSetter() + " " + str);
		}
		return ;
	}
	if (channel->getMode("+t") && !channel->hasPerm(client)) {
		m = ERR_CHANOPRIVSNEEDED(client.getNick(), client.getUser(), channel->getName());
		send(client.getFd(), m.c_str(), m.size(), 0);
		throw TopicCmd::FormatException();
	}
	channel->setTopicSetter(client.getNick());
	channel->setTopic(this->_tokens[2]);
	channel->setTopicSetTime(std::time(NULL));
    // std::string msg = ":" + client.getPrefix() + " TOPIC " + channel->getName() + " :" + channel->getTopic() + "\r\n";
    m = SEND_TOPIC(channel->getName(), channel->getTopic());
	sendToChannelClient(channel, m);
}

const char* TopicCmd::FormatException::what() const throw()
{
    return ("WRONG PASSs");
}
