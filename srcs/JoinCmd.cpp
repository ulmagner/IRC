/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JoinCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 10:17:41 by ulmagner          #+#    #+#             */
/*   Updated: 2025/08/03 18:28:37 by ulmagner         ###   ########.fr       */
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
	if (s.find(delimiter) == std::string::npos) {
		tokens.push_back(s);
		return (tokens);
	}

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
	std::vector<std::string>::const_iterator kt = keys.begin();
	for (;itt != chan.end(); ++itt) {
		std::string name = *itt;
		std::string key = "";
		if (kt != keys.end()) {
			key = *kt;
			++kt;
		}
		std::cout << key << std::endl;
		if (name[0] != '#') {
			m = ERR_NOSUCHCHANNEL(client.getNick(), name);
			send(client.getFd(), m.c_str(), m.size(), 0);
			continue ;
		}
		std::cout << ">>> Checking if channel exists" << std::endl;
		Channel* channel = this->_serv.getChannelByName(name);
		std::cout << ">>> ..." << std::endl;
		if (channel) {
			std::cout << ">>> exist." << std::endl;
			if (channel->getClients().size() >= (size_t)channel->getLim()) {
				m = ERR_CHANNELISFULL(client.getNick(), name);
				send(client.getFd(), m.c_str(), m.size(), 0);
				std::cout << m << std::endl;
				continue ;
			}
			std::cout << ">>> check mode." << std::endl;
			if (channel->hasMode("+i") && !channel->getInvite(client.getNick())) {
				m = ERR_INVITEONLYCHAN(client.getNick(), name);
				send(client.getFd(), m.c_str(), m.size(), 0);
				std::cout << m << std::endl;
				continue ;
			}
			std::cout << ">>> check key." << std::endl;
			std::cout << "[" << channel->getKey() << "]" << "[" << key << "]" << std::endl;
			if (channel->hasMode("+k") && key != channel->getKey()) {
				m = ERR_BADCHANNELKEY(client.getNick(), name);
				send(client.getFd(), m.c_str(), m.size(), 0);
				std::cout << m << std::endl;
				continue ;
			}
			if (!channel->hasAlreadyJoin(client.getFd())) {
				if (channel->getInvite(client.getNick()))
					channel->eraseFromInvite(client.getNick());
				channel->addClient(client);
			}
		}
		else {
			std::cout << ">>> exist pas." << std::endl;
			Channel* newChan = new Channel(name, key, client);
			if (!key.empty())
				newChan->addMode("+k");
			this->_serv.getChannels().push_back(newChan);
			channel = newChan;
			std::cout << channel->getKey() << std::endl;
			m = RPL_MODE(client.getNick(), client.getUser(), name, "+o");
			send(client.getFd(), m.c_str(), m.size(), 0);
			std::cout << m << std::endl;
		}
		m = JOIN_CHANNEL(client.getNick(), client.getUser(), name);
		sendToChannelClient(channel, m);
		std::cout << m << std::endl;
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
			this->_serv.sendToClient(client, "333", " " + this->_tokens[1] + " " + channel->getTopicSetter() + " " + str + "\r\n");
			std::cout << m << std::endl;
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
		std::cout << m << std::endl;
		m = RPL_ENDOFNAMES(client.getNick(), name);
		send(client.getFd(), m.c_str(), m.size(), 0);
		std::cout << m << std::endl;
	}
}

const char* JoinCmd::FormatException::what() const throw()
{
	return ("JOIN FORMAT");
}
