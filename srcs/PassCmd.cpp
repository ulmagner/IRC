/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PassCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 16:05:31 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/24 18:36:13 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PassCmd.hpp"
#include "Serv.hpp"

PassCmd::PassCmd( std::vector<std::string> tokens, Serv& serv ) : ACmd(tokens[0]), _tokens(tokens), _serv(serv) {}

PassCmd::~PassCmd( void ) {}

void PassCmd::executeCmd( Client& client ) {
	if (this->_tokens.size() < 1 || this->_tokens.size() > 2) {
		this->sendToClient(client, "461", ERR_NEEDMOREPARAMS);
		throw PassCmd::FormatException();
	}
	if (!client.getPass().empty()) {
		this->sendToClient(client, "462", ERR_ALREADYREGISTERED);
		throw PassCmd::ErrorException();
	}
	std::string tok = "";
	if (this->_tokens.size() > 1)
		tok = this->_tokens[1];
	if (tok != this->_serv.getPass()) {
		this->sendToClient(client, "464", ERR_PASSWDMISMATCH);
		throw PassCmd::FormatException();
	}
	client.setPass(tok);
	std::cout << "Serv creates " << ACmd::_ACmdRef << std::endl;
}

void PassCmd::sendToClient( Client& client, const std::string& code, const std::string& message ) {
	std::string fullMsg = "";
	if (code == "461")
		fullMsg = ":" + this->_serv._name + " " + code + " * " + this->_tokens[0] + " " + message;
	else if (code == "464" || code == "462")
		fullMsg = ":" + this->_serv._name + " " + code + " * " + message;
	send(client.getFd(), fullMsg.c_str(), fullMsg.size(), 0);
}

const char* PassCmd::FormatException::what() const throw()
{
    return ("WRONG PASSs");
}
