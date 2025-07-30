/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PassCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 16:05:31 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/30 11:54:12 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PassCmd.hpp"
#include "Serv.hpp"

PassCmd::PassCmd( std::vector<std::string> tokens, Serv& serv ) : ACmd(tokens[0]), _tokens(tokens), _serv(serv) {}

PassCmd::~PassCmd( void ) {}

void PassCmd::executeCmd( Client& client ) {
	if (this->_tokens.size() < 2) {
		this->_serv.sendToClient(client, "461", " " + this->_tokens[0] + ERR_NEEDMOREPARAMS);
		throw PassCmd::FormatException();
	}
	if (!client.getPass().empty()) {
		this->_serv.sendToClient(client, "462", ERR_ALREADYREGISTERED);
		throw PassCmd::ErrorException();
	}
	std::string tok = "";
	if (this->_tokens.size() > 1)
		tok = this->_tokens[1];
	if (tok != this->_serv.getPass()) {
		this->_serv.sendToClient(client, "464", ERR_PASSWDMISMATCH);
		throw PassCmd::FormatException();
	}
	client.setPass(tok);
}

const char* PassCmd::FormatException::what() const throw()
{
    return ("WRONG PASSs");
}
