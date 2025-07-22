/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NickCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 16:05:31 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/22 11:35:35 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "NickCmd.hpp"
#include "Serv.hpp"

NickCmd::NickCmd( std::vector<std::string> tokens, Serv& serv ) : ACmd(tokens[0]), _NickCmdRef(""), _serv(serv) {
	if (tokens.size() > 1 && !tokens[1].empty()) {
		this->_NickCmdRef = tokens[1];
	}
}

NickCmd::~NickCmd( void ) {}

void NickCmd::executeCmd( Client& client ) const {
	if (client.getPass().empty())
		throw NickCmd::ErrorException();
	client.setNick(_NickCmdRef);
    std::map<int, Client>::const_iterator it = this->_serv.getConnections().begin();
    for (; it != this->_serv.getConnections().end(); ++it) {
        if (!it->second.getNick().empty() && it->first != client.getFd() && !it->second.getNick().compare(_NickCmdRef)) {
			client.setPath("");
			throw NickCmd::ErrorException();
		}
    }
	std::cout << "Serv creates " << ACmd::_ACmdRef << std::endl;
}

const char* NickCmd::FormatException::what() const throw()
{
    return ("WRONG PASS");
}
