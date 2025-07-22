/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JoinCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 10:17:41 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/22 11:11:48 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "JoinCmd.hpp"
#include "Serv.hpp"

JoinCmd::JoinCmd( std::vector<std::string> tokens, Serv& serv ) : ACmd(tokens[0]), _JoinCmdRef(""), _serv(serv) {
	if (tokens.size() > 1 && !tokens[1].empty()) {
		this->_JoinCmdRef = tokens[1];
	}
}

JoinCmd::~JoinCmd( void ) {}

void JoinCmd::executeCmd( Client& client ) const {
	(void) client;
	// if (!client.getPass().empty())
	// 	throw JoinCmd::ErrorException();
	// client.setNick(_JoinCmdRef);
    // std::map<int, Client>::const_iterator it = this->_serv.getConnections().begin();
    // for (; it != this->_serv.getConnections().end(); ++it) {
    //     if (!it->second.getNick().empty() && it->first != client.getFd() && !it->second.getNick().compare(_JoinCmdRef)) {
	// 		client.setPath("");
	// 		throw JoinCmd::ErrorException();
	// 	}
    // }
	// std::cout << "Serv creates " << ACmd::_ACmdRef << std::endl;
}

const char* JoinCmd::FormatException::what() const throw()
{
    return ("WRONG PASS");
}
