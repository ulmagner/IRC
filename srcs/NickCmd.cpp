/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NickCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 16:05:31 by ulmagner          #+#    #+#             */
/*   Updated: 2025/08/02 23:01:20 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "NickCmd.hpp"
#include "Serv.hpp"

NickCmd::NickCmd( std::vector<std::string> tokens, Serv& serv ) : ACmd(tokens[0]), _tokens(tokens), _serv(serv) {}

NickCmd::~NickCmd( void ) {}

void NickCmd::executeCmd( Client& client ) {
	std::string m = "";
	if (this->_tokens.size() < 2) {
		m = ERR_NEEDMOREPARAMS(client.getNick(), this->_tokens[0]);
		send(client.getFd(), m.c_str(), m.size(), 0);
		std::cout << m << std::endl;
		throw NickCmd::FormatException();
	}
	if (client.getPass().empty())
		throw NickCmd::ErrorException();
	if (!client.getNick().empty())
		throw NickCmd::ErrorException();
	if (this->_tokens.size() == 1) {
		m = ERR_NONICKNAMEGIVEN();
		send(client.getFd(), m.c_str(), m.size(), 0);
		std::cout << m << std::endl;
		throw NickCmd::FormatException();
	}
	client.setNick(this->_tokens[1]);
    std::map<int, Client>::const_iterator it = this->_serv.getConnections().begin();
    for (; it != this->_serv.getConnections().end(); ++it) {
        if (!it->second.getNick().empty() && it->first != client.getFd() && !it->second.getNick().compare(this->_tokens[1])) {
			client.setNick("");
			m = ERR_NICKNAMEINUSE(this->_tokens[1]);
			send(client.getFd(), m.c_str(), m.size(), 0);
			std::cout << m << std::endl;
			throw NickCmd::ErrorException();
		}
    }
}

const char* NickCmd::FormatException::what() const throw()
{
    return ("WRONG PASS");
}
