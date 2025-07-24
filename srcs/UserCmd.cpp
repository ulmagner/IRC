/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 16:05:31 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/24 11:59:52 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "UserCmd.hpp"
#include "Serv.hpp"

UserCmd::UserCmd( std::vector<std::string> tokens, Serv& serv ) : ACmd(tokens[0]), _tokens(tokens), _serv(serv) {}

UserCmd::~UserCmd( void ) {}

void UserCmd::executeCmd( Client& client ) {
	if (this->_tokens.size() < 1 || this->_tokens.size() > 2) {
		this->sendToClient(client, "461", ERR_NEEDMOREPARAMS);
		throw UserCmd::FormatException();
	}
	if (client.getPass().empty())
		throw UserCmd::ErrorException();
	if (!client.getUser().empty()) {
		this->sendToClient(client, "462", ERR_ALREADYREGISTERED);
		throw UserCmd::ErrorException();
	}
	if (this->_tokens.size() == 1)
		throw UserCmd::FormatException();
	client.setUser(this->_tokens[1]);
    std::map<int, Client>::const_iterator it = this->_serv.getConnections().begin();
    for (; it != this->_serv.getConnections().end(); ++it) {
        if (!it->second.getUser().empty() && it->first != client.getFd() && !it->second.getUser().compare(this->_tokens[1])) {
			client.setUser("");
			throw UserCmd::FormatException();
		}
    }
	std::cout << "Serv creates " << ACmd::_ACmdRef << std::endl;
}

void UserCmd::sendToClient( Client& client, const std::string& code, const std::string& message ) {
	std::string fullMsg = "";
	if (code == "461")
		fullMsg = ":" + this->_serv._name + " " + code + " * " + this->_tokens[0] + " " + message;
	else if (code == "462")
		fullMsg = ":" + this->_serv._name + " " + code + " * " + message;
	send(client.getFd(), fullMsg.c_str(), fullMsg.size(), 0);
}

const char* UserCmd::FormatException::what() const throw()
{
    return ("FORMAT USER");
}
