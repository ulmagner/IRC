/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JoinCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 10:17:41 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/23 12:43:53 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "JoinCmd.hpp"
#include "Serv.hpp"

JoinCmd::JoinCmd( std::vector<std::string> tokens, Serv& serv ) : ACmd(tokens[0]), _tokens(tokens), _serv(serv) {
	if (tokens.size() < 2 || tokens.size() > 3) {
		throw JoinCmd::FormatException();
	}
}

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
	(void) client;
	for (size_t i = 0; i < this->_tokens.size(); ++i) {
		std::cout << this->_tokens[i] << std::endl;
	}
	std::vector<std::string> chan = split(this->_tokens[1], ',');
	std::vector<std::string> keys;
	if (this->_tokens.size() > 2)
		keys = split(this->_tokens[2], ',');
	std::vector<Channel>::const_iterator it = this->_serv.getChannels().begin();
	std::vector<std::string>::const_iterator itt = chan.begin();
	for (;itt != chan.end(); ++itt) {
		std::string name = *itt;
		std::string key = "";
		if (itt != keys.end())
			key = *itt;
		Channel* channel = NULL;
		for (size_t j = 0;j < this->_serv.getChannels().size(); ++j) {
			if (it->getName() == *itt) {
				channel = &this->_serv.getChannels()[j];
				break ;
			}
		}
		if (channel) {
			std::cout << "FOUND EXISTING CHANNEL." << std::endl;
			Client* cl = &this->_serv.getClientByFd(client.getFd());
			if (key != it->getKey())
				std::cout << "KEY DOESNT MATCH." << std::endl;
			else if (cl)
				std::cout << "CLIENT ALREADY IN CHANNEL" << channel->getName() << std::endl;
			else
				channel->addClient(client);
		}
		else {
			std::cout << "DID NOT FOUND EXISTING CHANNEL." << std::endl;
			this->_serv.getChannels().push_back(Channel(name, key, client));
		}
	}
}

const char* JoinCmd::FormatException::what() const throw()
{
	return ("JOIN FORMAT");
}
