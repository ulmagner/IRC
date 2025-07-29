/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TopicCmd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 12:06:46 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/29 15:38:43 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TopicCmd.hpp"
#include "Serv.hpp"

TopicCmd::TopicCmd( std::vector<std::string> tokens, Serv& serv ) : ACmd(tokens[0]), _tokens(tokens), _serv(serv) {}

TopicCmd::~TopicCmd( void ) {}

void TopicCmd::executeCmd( Client& client ) {
	if (this->_tokens.size() < 1 || this->_tokens.size() > 3) {
		this->sendToClient(client, "461", ERR_NEEDMOREPARAMS);
		throw TopicCmd::FormatException();
	}
	Channel* channel = this->_serv.getChannelByName(this->_tokens[1]);
	if (!channel) {
		this->sendToClient(client, "403", channel->getName() + ERR_NOSUCHCHANNEL);
		throw TopicCmd::FormatException();
	}
	if (!channel->getClientByName(client.getNick())) {
		this->sendToClient(client, "442", channel->getName() + ERR_NOTONCHANNEL);
		throw TopicCmd::FormatException();
	}
	if (this->_tokens.size() == 2) {
		if (channel->getTopic().empty()) {
			sendToClient(client, "331", this->_tokens[1] + RPL_NOTOPIC);
		} else {
			sendToClient(client, "332", this->_tokens[1] + " :" + channel->getTopic());
			std::ostringstream oss;
			oss << channel->getTopicSetTime();
			std::string str = oss.str();
			sendToClient(client, "333", this->_tokens[1] + " " + channel->getTopicSetter() + " " + str);
		}
		return ;
	}
	if (channel->getMode("+t") && !channel->hasPerm(client)) {
        this->sendToClient(client, "482", channel->getName() + ERR_CHANOPRIVSNEEDED);
		throw TopicCmd::FormatException();
	}
	channel->setTopicSetter(client.getNick());
	channel->setTopic(this->_tokens[2]);
	channel->setTopicSetTime(std::time(NULL));
    std::string msg = ":" + client.getPrefix() + " TOPIC " + channel->getName() + " :" + channel->getTopic() + "\r\n";
    sendToChannelClient(channel, msg);
}

void TopicCmd::sendToClient( Client& client, const std::string& code, const std::string& message ) {
	std::string fullMsg = ":" + this->_serv._name + " " + code + " ";
	if (code == "461")
		fullMsg += this->_tokens[0] + " " + message;
	else if (code == "482" || code == "403" || code == "442" || code == "401")
		fullMsg += message;
	else if (code == "441" || code == "443" || code == "331" || code == "332" || code == "333")
		fullMsg += client.getNick() + " " + message;
	send(client.getFd(), fullMsg.c_str(), fullMsg.size(), 0);
}

const char* TopicCmd::FormatException::what() const throw()
{
    return ("WRONG PASSs");
}
