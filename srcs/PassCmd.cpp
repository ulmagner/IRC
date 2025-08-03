/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PassCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 16:05:31 by ulmagner          #+#    #+#             */
/*   Updated: 2025/08/02 23:02:15 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PassCmd.hpp"
#include "Serv.hpp"

PassCmd::PassCmd( std::vector<std::string> tokens, Serv& serv ) : ACmd(tokens[0]), _tokens(tokens), _serv(serv) {}

PassCmd::~PassCmd( void ) {}

void PassCmd::executeCmd( Client& client ) {
	std::string m = "";
	if (this->_tokens.size() < 2) {
		m = ERR_NEEDMOREPARAMS(client.getNick(), this->_tokens[0]);
		send(client.getFd(), m.c_str(), m.size(), 0);
		std::cout << m << std::endl;
		throw PassCmd::FormatException();
	}
	if (!client.getPass().empty()) {
		m = ERR_ALREADYREGISTERED(client.getNick());
		send(client.getFd(), m.c_str(), m.size(), 0);
		std::cout << m << std::endl;
		throw PassCmd::ErrorException();
	}
	std::string tok = "";
	if (this->_tokens.size() > 1)
		tok = this->_tokens[1];
	if (tok != this->_serv.getPass()) {
		m = ERR_PASSWDMISMATCH(client.getNick());
		send(client.getFd(), m.c_str(), m.size(), 0);
		std::cout << m << std::endl;
		throw PassCmd::FormatException();
	}
	client.setPass(tok);
}

const char* PassCmd::FormatException::what() const throw()
{
    return ("WRONG PASSs");
}
