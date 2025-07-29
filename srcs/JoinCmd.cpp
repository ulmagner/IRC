/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JoinCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 10:17:41 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/29 16:22:50 by ulmagner         ###   ########.fr       */
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
		this->sendToClient(client, "461", ERR_NEEDMOREPARAMS);
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
		Channel* channel = this->_serv.getChannelByName(name);
		if (channel) {
			if (channel->getMode("+i") && !channel->getInvite(client.getNick())) {
				sendToClient(client, "473", name + ERR_INVITEONLYCHAN);
				throw Channel::FormatException();
			}
			if (key != channel->getKey()) {
				this->sendToClient(client, "475", name + ERR_BADCHANNELKEY);
				continue ;
			}
			if (!channel->hasAlreadyJoin(client.getFd())) {
				if (channel->getInvite(client.getNick()))
					channel->eraseFromInvite(client.getNick());
				channel->addClient(client);
			}
		}
		else {
			// this->sendToClient(client, "403", name + ERR_NOSUCHCHANNEL);
			this->_serv.getChannels().push_back(Channel(name, key, client));
			channel = &this->_serv.getChannels().back();
			std::string modeMsg = ":" + this->_serv._name + " MODE " + name + " +o " + client.getNick() + "\r\n";
			send(client.getFd(), modeMsg.c_str(), modeMsg.size(), 0);
		}
		std::string msg = client.getPrefix() + " JOIN :" + name + "\r\n";
		send(client.getFd(), msg.c_str(), msg.size(), 0);
		if (channel->getTopic().empty()) {
			sendToClient(client, "331", name + RPL_NOTOPIC);
		} else {
			sendToClient(client, "332", name + " :" + channel->getTopic());
			std::ostringstream oss;
			oss << channel->getTopicSetTime();
			std::string str = oss.str();
			sendToClient(client, "333", this->_tokens[1] + " " + channel->getTopicSetter() + " " + str);
		}
		std::map<int, Client>::const_iterator at = channel->getClients().begin();
		msg = ":" + this->_serv._name + " 353 " + client.getNick() + " = " + name + " :";
		for (;at != channel->getClients().end();++at) {
			msg += " ";
			if (at->first == 1)
				msg += "@";
			msg += at->second.getNick();
		}
		msg += "\r\n";
		send(client.getFd(), msg.c_str(), msg.size(), 0);
		sendToClient(client, "366", name + RPL_ENDOFNAMES);
	}
}

void JoinCmd::sendToClient( Client& client, const std::string& code, const std::string& message ) {
	std::string fullMsg = ":" + this->_serv._name + " " + code + " " + client.getNick() + " ";
	if (code == "461")
		fullMsg += this->_tokens[0] + " " + message;
	else if (code == "475" || code == "403" || code == "473")
		fullMsg += message;
	else if (code == "331" || code == "332" || code == "333" || code == "366") {
		fullMsg += message;
	}
	send(client.getFd(), fullMsg.c_str(), fullMsg.size(), 0);
}

const char* JoinCmd::FormatException::what() const throw()
{
	return ("JOIN FORMAT");
}
