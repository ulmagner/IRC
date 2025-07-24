/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JoinCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 10:17:41 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/24 16:43:35 by ulmagner         ###   ########.fr       */
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
	std::vector<Channel>& allChannels = this->_serv.getChannels();
	for (;itt != chan.end(); ++itt) {
		std::string name = *itt;
		std::string key = "";
		if (itt != keys.end())
			key = *itt;
		Channel* channel = NULL;
		for (size_t j = 0;j < allChannels.size(); ++j) {
			if (allChannels[j].getName() == name) {
				channel = &allChannels[j];
				break ;
			}
		}
		if (channel) {
			std::cout << "FOUND EXISTING CHANNEL." << std::endl;
			if (key != channel->getKey()) {
				this->sendToClient(client, "475", name + ERR_BADCHANNELKEY);
				std::cout << "KEY DOESNT MATCH." << std::endl;
				continue ;
			}
			Client* cl = &this->_serv.getClientByFd(client.getFd());
			if (cl)
				std::cout << "CLIENT ALREADY IN CHANNEL." << channel->getName() << std::endl;
			else
				channel->addClient(client);
		}
		else {
			this->sendToClient(client, "403", name + ERR_NOSUCHCHANNEL);
			std::cout << "DID NOT FOUND EXISTING CHANNEL." << std::endl;
			this->_serv.getChannels().push_back(Channel(name, key, client));
		}
	}
}

void JoinCmd::sendToClient( Client& client, const std::string& code, const std::string& message ) {
	std::string fullMsg = "";
	if (code == "461")
		fullMsg = ":" + this->_serv._name + " " + code + client.getUser() + this->_tokens[0] + " " + message;
	else if (code == "475" || "403")
		fullMsg = ":" + this->_serv._name + " " + code + client.getUser() + message;
	send(client.getFd(), fullMsg.c_str(), fullMsg.size(), 0);
}

const char* JoinCmd::FormatException::what() const throw()
{
	return ("JOIN FORMAT");
}
