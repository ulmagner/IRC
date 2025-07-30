/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NickCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 16:05:31 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/30 11:55:01 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "NickCmd.hpp"
#include "Serv.hpp"

NickCmd::NickCmd( std::vector<std::string> tokens, Serv& serv ) : ACmd(tokens[0]), _tokens(tokens), _serv(serv) {}

NickCmd::~NickCmd( void ) {}

void NickCmd::executeCmd( Client& client ) {
	if (this->_tokens.size() < 1 || this->_tokens.size() > 2) {
		this->_serv.sendToClient(client, "461", " " + this->_tokens[0] + ERR_NEEDMOREPARAMS);
		throw NickCmd::FormatException();
	}
	if (client.getPass().empty())
		throw NickCmd::ErrorException();
	if (!client.getNick().empty())
		throw NickCmd::ErrorException();
	if (this->_tokens.size() == 1) {
		this->_serv.sendToClient(client, "431", ERR_NONICKNAMEGIVEN);
		throw NickCmd::FormatException();
	}
	client.setNick(this->_tokens[1]);
    std::map<int, Client>::const_iterator it = this->_serv.getConnections().begin();
    for (; it != this->_serv.getConnections().end(); ++it) {
        if (!it->second.getNick().empty() && it->first != client.getFd() && !it->second.getNick().compare(this->_tokens[1])) {
			client.setNick("");
			this->_serv.sendToClient(client, "433", " " + this->_tokens[1] + ERR_NICKNAMEINUSE);
			throw NickCmd::ErrorException();
		}
    }
}

const char* NickCmd::FormatException::what() const throw()
{
    return ("WRONG PASS");
}
