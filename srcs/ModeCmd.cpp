/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 11:04:14 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/31 12:02:21 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ModeCmd.hpp"
#include "Serv.hpp"

ModeCmd::ModeCmd( std::vector<std::string> tokens, Serv& serv ) : ACmd(tokens[0]), _tokens(tokens), _serv(serv) {}

ModeCmd::~ModeCmd( void ) {}

void ModeCmd::executeCmd( Client& client ) {
	std::string m = "";
	if (this->_tokens.size() < 2) {
		m = ERR_NEEDMOREPARAMS(client.getNick(), this->_tokens[0]);
		send(client.getFd(), m.c_str(), m.size(), 0);
		throw ModeCmd::FormatException();
	}
	std::string mode = "";
	if (this->_tokens.size() > 2) {
		mode = this->_tokens[2];
		if (mode.size() != 2 || mode[0] != '+' || mode[0] != '-' || !isalpha(mode[1])) {
			m = ERR_UNKNOWNMODE(client.getNick(), client.getUser(), mode);
			send(client.getFd(), m.c_str(), m.size(), 0);
			throw ModeCmd::FormatException();
		}
	}
	std::string name = "";
	name = this->_tokens[1];
	if (name[0] == '#') {
		Channel* channel = this->_serv.getChannelByName(name);
		if (!channel) {
			m = ERR_NOSUCHCHANNEL(client.getNick(), name);
			send(client.getFd(), m.c_str(), m.size(), 0);
			throw ModeCmd::FormatException();
		}
		if (!channel->getClientByName(client.getNick())) {
			m = ERR_NOTONCHANNEL(client.getNick(), channel->getName());
			send(client.getFd(), m.c_str(), m.size(), 0);
			throw ModeCmd::FormatException();
		}
		if (this->_tokens.size() == 2) {
			m = RPL_CHANNELMODEIS(client.getNick(), channel->getName(), channel->getMode());
			send(client.getFd(), m.c_str(), m.size(), 0);
			std::ostringstream oss;
			oss << channel->getTopicSetTime();
			std::string str = oss.str();
			m = RPL_CREATIONTIME(client.getNick(), channel->getName(), str);
			send(client.getFd(), m.c_str(), m.size(), 0);
			throw ModeCmd::FormatException();
		}
		m = RPL_MODE(client.getNick(), client.getUser(), channel->getName(), mode);
		send(client.getFd(), m.c_str(), m.size(), 0);
		if (mode[0] == '+')
			channel->addMode(mode);
		else
			channel->removeMode(mode);
	}
	else {
		Client* cl = this->_serv.getClientByName(name);
		if (!cl) {
			m = ERR_NOSUCHNICK(client.getNick(), this->_tokens[1]);
			send(client.getFd(), m.c_str(), m.size(), 0);
			throw ModeCmd::FormatException();
		}
		if (this->_tokens.size() == 2) {
			m = RPL_UMODEIS(client.getNick(), cl->getNick(), cl->getMode());
			send(cl->getFd(), m.c_str(), m.size(), 0);
			throw ModeCmd::FormatException();
		}
		m = RPL_MODE(client.getNick(), client.getUser(), cl->getNick(), mode);
		send(cl->getFd(), m.c_str(), m.size(), 0);
		if (mode[0] == '+')
			cl->addMode(mode);
		else
			cl->removeMode(mode);
	}
}

const char* ModeCmd::FormatException::what() const throw()
{
    return ("WRONG PASS");
}
