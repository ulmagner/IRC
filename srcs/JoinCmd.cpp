/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JoinCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 10:17:41 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/22 17:10:24 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "JoinCmd.hpp"
#include "Serv.hpp"

JoinCmd::JoinCmd( std::vector<std::string> tokens, Serv& serv ) : ACmd(tokens[0]), _tokens(tokens), _serv(serv) {
	if (tokens.size() < 1 || tokens.size() > 2) {
		throw JoinCmd::FormatException();
	}
}

JoinCmd::~JoinCmd( void ) {}

void JoinCmd::executeCmd( Client& client ) {
	(void) client;
	for (size_t i = 0; i < this->_tokens.size(); ++i) {
		std::cout << this->_tokens[i] << std::endl;
	}
	std::vector<std::string> chan;
	std::vector<std::string> key;
	std::vector<Channel>::const_iterator it = this->_serv.getChannels().begin();
	for (;it != this->_serv.getChannels().end(); ++it) {
		for (size_t i = 0; i < this->_serv.getChannels().size(); ++i) {
			if (it->getName() == this->_tokens[1]) {
				;
			}
		}
	}
}

const char* JoinCmd::FormatException::what() const throw()
{
	return ("JOIN FORMAT");
}
