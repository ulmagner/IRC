/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JoinCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 10:17:41 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/31 11:35:22 by ulmagner         ###   ########.fr       */
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
	std::string m = "";
	if (this->_tokens.size() < 2) {
		m = ERR_NEEDMOREPARAMS(client.getNick(), this->_tokens[0]);
		send(client.getFd(), m.c_str(), m.size(), 0);
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
			m = ERR_NOSUCHCHANNEL(client.getNick(), name);
			send(client.getFd(), m.c_str(), m.size(), 0);
			continue ;
		}
		Channel* channel = this->_serv.getChannelByName(name);
		if (channel) {
			if (channel->hasMode("+i") && !channel->getInvite(client.getNick())) {
				m = ERR_INVITEONLYCHAN(client.getNick(), name);
				send(client.getFd(), m.c_str(), m.size(), 0);
				throw Channel::FormatException();
			}
			if (key != channel->getKey()) {
				m = ERR_BADCHANNELKEY(client.getNick(), name);
				send(client.getFd(), m.c_str(), m.size(), 0);
				continue ;
			}
			if (!channel->hasAlreadyJoin(client.getFd())) {
				if (channel->getInvite(client.getNick()))
					channel->eraseFromInvite(client.getNick());
				channel->addClient(client);
			}
		}
		else {
			Channel* newChan = new Channel(name, key, client);
			this->_serv.getChannels().push_back(newChan);
			channel = newChan;
			m = RPL_MODE(client.getNick(), client.getUser(), name, "+o");
			send(client.getFd(), m.c_str(), m.size(), 0);
		}
		m = JOIN_CHANNEL(client.getNick(), client.getUser(), name);
		sendToChannelClient(channel, m);
		if (channel->getTopic().empty()) {
			m = RPL_NOTOPIC(client.getNick(), name);
			send(client.getFd(), m.c_str(), m.size(), 0);
		} else {
			m = RPL_TOPIC(client.getNick(), name, channel->getTopic());
			send(client.getFd(), m.c_str(), m.size(), 0);
			std::ostringstream oss;
			oss << channel->getTopicSetTime();
			std::string str = oss.str();
			this->_serv.sendToClient(client, "333", " " + this->_tokens[1] + " " + channel->getTopicSetter() + " " + str + "\r\n");
		}
		std::map<int, std::pair<Client *, int> >::const_iterator at = channel->getClients().begin();
		std::string ms = "";
		for (;at != channel->getClients().end();++at) {
			if (at != channel->getClients().begin())
				ms += " ";
			if (at->second.second == 1)
				ms += "@";
			ms += at->second.first->getNick();
		}
		m = RPL_NAMREPLY(client.getNick(), name, ms);
		sendToChannelClient(channel, m);
		m = RPL_ENDOFNAMES(client.getNick(), name);
		send(client.getFd(), m.c_str(), m.size(), 0);
	}
}

const char* JoinCmd::FormatException::what() const throw()
{
	return ("JOIN FORMAT");
}
