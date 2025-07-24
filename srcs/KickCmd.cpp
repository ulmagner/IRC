/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KickCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 14:30:32 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/24 14:32:03 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "KickCmd.hpp"
#include "Serv.hpp"

KickCmd::KickCmd( std::vector<std::string> tokens, Serv& serv ) : ACmd(tokens[0]), _tokens(tokens), _serv(serv) {}

KickCmd::~KickCmd( void ) {}

void KickCmd::executeCmd( Client& client ) {
	(void) client;
}

void KickCmd::sendToClient( Client& client, const std::string& code, const std::string& message ) {
	std::string fullMsg = "";
	if (code == "461")
		fullMsg = ":" + this->_serv._name + " " + code + " * " + this->_tokens[0] + " " + message;
	else if (code == "475" || "403")
		fullMsg = ":" + this->_serv._name + " " + code + " * " + message;
	send(client.getFd(), fullMsg.c_str(), fullMsg.size(), 0);
}

const char* KickCmd::FormatException::what() const throw()
{
	return ("Kick FORMAT");
}
