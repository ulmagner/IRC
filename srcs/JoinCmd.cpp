/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JoinCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 10:17:41 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/30 13:02:20 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "JoinCmd.hpp"
#include "Serv.hpp"

JoinCmd::JoinCmd( std::vector<std::string> tokens, Serv& serv ) : ACmd(tokens[0]), _tokens(tokens), _serv(serv) {}

JoinCmd::~JoinCmd( void ) {}

std::vector<std::string> split( const std::string& s, char delimiter ) {
    std::vector<std::string> tokens;
    std::stringstream ss(s);
    std::string word;
    while (std::getline(ss, word, delimiter)) {
        tokens.push_back(word);
    }
    return (tokens);
}

void JoinCmd::executeCmd( Client& client ) {
	if (this->_tokens.size() < 2 || this->_tokens.size() > 3) {
		this->_serv.sendToClient(client, "461", " " + this->_tokens[0] + ERR_NEEDMOREPARAMS);
		throw JoinCmd::FormatException();
	}
	std::vector<std::string> chan = split(this->_tokens[1], ',');
	std::vector<std::string> keys;
	if (this->_tokens.size() > 2)
		keys = split(this->_tokens[2], ',');
	std::vector<std::string>::const_iterator itt = chan.begin();
	for (;itt != chan.end(); ++itt) {
		std::string name = *itt;
		std::string key = "";
		if (itt != keys.end())
			key = *itt;
		if (name[0] != '#') {
			this->_serv.sendToClient(client, "403", " " + name + ERR_NOSUCHCHANNEL);
			continue ;
		}
		Channel* channel = this->_serv.getChannelByName(name);
		if (channel) {
			if (channel->getMode("+i") && !channel->getInvite(client.getNick())) {
				this->_serv.sendToClient(client, "473", " " + name + ERR_INVITEONLYCHAN);
				throw Channel::FormatException();
			}
			if (key != channel->getKey()) {
				this->_serv.sendToClient(client, "475", " " + name + ERR_BADCHANNELKEY);
				continue ;
			}
			if (!channel->hasAlreadyJoin(client.getFd())) {
				if (channel->getInvite(client.getNick()))
					channel->eraseFromInvite(client.getNick());
				channel->addClient(client);
			}
		}
		else {
			this->_serv.getChannels().push_back(Channel(name, key, client));
			channel = &this->_serv.getChannels().back();
			std::string modeMsg = ":" + this->_serv._name + " MODE " + name + " +o " + client.getNick() + "\r\n";
			send(client.getFd(), modeMsg.c_str(), modeMsg.size(), 0);
		}
		std::string msg = client.getPrefix() + " JOIN :" + name + "\r\n";
		send(client.getFd(), msg.c_str(), msg.size(), 0);
		if (channel->getTopic().empty()) {
			this->_serv.sendToClient(client, "331", " " + name + RPL_NOTOPIC);
		} else {
			this->_serv.sendToClient(client, "332", " " + name + " :" + channel->getTopic() + "\r\n");
			std::ostringstream oss;
			oss << channel->getTopicSetTime();
			std::string str = oss.str();
			this->_serv.sendToClient(client, "333", " " + this->_tokens[1] + " " + channel->getTopicSetter() + " " + str + "\r\n");
		}
		std::map<int, Client>::const_iterator at = channel->getClients().begin();
		msg = ":" + this->_serv._name + " 353 " + client.getNick() + " = " + name + " :";
		for (;at != channel->getClients().end();++at) {
			if (at != channel->getClients().begin())
				msg += " ";
			if (at->first == 1)
				msg += "@";
			msg += at->second.getNick();
		}
		msg += "\r\n";
		send(client.getFd(), msg.c_str(), msg.size(), 0);
		this->_serv.sendToClient(client, "366", " " + name + RPL_ENDOFNAMES);
	}
}

const char* JoinCmd::FormatException::what() const throw()
{
	return ("JOIN FORMAT");
}
