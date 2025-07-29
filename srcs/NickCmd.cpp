/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NickCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 16:05:31 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/28 15:27:01 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "NickCmd.hpp"
#include "Serv.hpp"

NickCmd::NickCmd( std::vector<std::string> tokens, Serv& serv ) : ACmd(tokens[0]), _tokens(tokens), _serv(serv) {}

NickCmd::~NickCmd( void ) {}

void NickCmd::executeCmd( Client& client ) {
	if (this->_tokens.size() < 1 || this->_tokens.size() > 2) {
		throw NickCmd::FormatException();
	}
	if (client.getPass().empty())
		throw NickCmd::ErrorException();
	if (!client.getNick().empty())
		throw NickCmd::ErrorException();
	if (this->_tokens.size() == 1) {
		this->sendToClient(client, "431", ERR_NONICKNAMEGIVEN);
		throw NickCmd::FormatException();
	}
	client.setNick(this->_tokens[1]);
    std::map<int, Client>::const_iterator it = this->_serv.getConnections().begin();
    for (; it != this->_serv.getConnections().end(); ++it) {
        if (!it->second.getNick().empty() && it->first != client.getFd() && !it->second.getNick().compare(this->_tokens[1])) {
			client.setNick("");
			this->sendToClient(client, "433", ERR_NICKNAMEINUSE);
			throw NickCmd::ErrorException();
		}
    }
}

void NickCmd::sendToClient( Client& client, const std::string& code, const std::string& message ) {
	std::string fullMsg = "";
	if (code == "431")
		fullMsg = ":" + this->_serv._name + " " + code + " * " + message;
	else if (code == "433")
		fullMsg = ":" + this->_serv._name + " " + code + " * " + this->_tokens[1] + " " + message;
	send(client.getFd(), fullMsg.c_str(), fullMsg.size(), 0);
}

const char* NickCmd::FormatException::what() const throw()
{
    return ("WRONG PASS");
}
