/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TopicCmd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 12:06:46 by ulmagner          #+#    #+#             */
/*   Updated: 2025/08/02 23:09:00 by ulmagner         ###   ########.fr       */
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
		std::cout << m << std::endl;
		throw TopicCmd::FormatException();
	}
	std::string name = this->_tokens[1];
	if (name[0] != '#') {
		m = ERR_NOSUCHCHANNEL(client.getNick(), name);
		send(client.getFd(), m.c_str(), m.size(), 0);
		std::cout << m << std::endl;
		throw TopicCmd::FormatException();
	}
	Channel* channel = this->_serv.getChannelByName(name);
	if (!channel) {
		m = ERR_NOSUCHCHANNEL(client.getNick(), name);
		send(client.getFd(), m.c_str(), m.size(), 0);
		std::cout << m << std::endl;
		throw TopicCmd::FormatException();
	}
	if (!channel->getClientByName(client.getNick())) {
		m = ERR_NOTONCHANNEL(client.getNick(), channel->getName());
		send(client.getFd(), m.c_str(), m.size(), 0);
		std::cout << m << std::endl;
		throw TopicCmd::FormatException();
	}
	if (this->_tokens.size() == 2) {
		if (channel->getTopic().empty()) {
			m = RPL_NOTOPIC(client.getNick(), name);
			send(client.getFd(), m.c_str(), m.size(), 0);
			std::cout << m << std::endl;
		} else {
			m = RPL_TOPIC(client.getNick(), name, channel->getTopic());
			send(client.getFd(), m.c_str(), m.size(), 0);
			std::cout << m << std::endl;
			std::ostringstream oss;
			oss << channel->getTopicSetTime();
			std::string str = oss.str();
			this->_serv.sendToClient(client, "333", " " + name + " " + channel->getTopicSetter() + " " + str);
			std::cout << m << std::endl;
		}
		return ;
	}
	if (channel->hasMode("+t") && !channel->hasPerm(client)) {
		m = ERR_CHANOPRIVSNEEDED(client.getNick(), client.getUser(), channel->getName());
		send(client.getFd(), m.c_str(), m.size(), 0);
		std::cout << m << std::endl;
		throw TopicCmd::FormatException();
	}
	channel->setTopicSetter(client.getNick());
	channel->setTopic(this->_tokens[2]);
	channel->setTopicSetTime(std::time(NULL));
    m = SEND_TOPIC(channel->getName(), channel->getTopic());
	sendToChannelClient(channel, m);
	std::cout << m << std::endl;
}

const char* TopicCmd::FormatException::what() const throw()
{
    return ("WRONG PASSs");
}
