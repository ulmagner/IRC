/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 16:05:31 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/22 11:29:43 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "UserCmd.hpp"
#include "Serv.hpp"

UserCmd::UserCmd( std::vector<std::string> tokens, Serv& serv ) : ACmd(tokens[0]), _UserCmdRef(""), _serv(serv) {
	if (tokens.size() > 1 && !tokens[1].empty()) {
		this->_UserCmdRef = tokens[1];
	}
}

UserCmd::~UserCmd( void ) {}

void UserCmd::executeCmd( Client& client ) const {
	if (client.getPass().empty())
		throw UserCmd::ErrorException();
	client.setUser(this->_UserCmdRef);
    std::map<int, Client>::const_iterator it = this->_serv.getConnections().begin();
    for (; it != this->_serv.getConnections().end(); ++it) {
        if (!it->second.getUser().empty() && it->first != client.getFd() && !it->second.getUser().compare(_UserCmdRef)) {
			client.setPath("");
			throw UserCmd::FormatException();
		}
    }
	std::cout << "Serv creates " << ACmd::_ACmdRef << std::endl;
}

const char* UserCmd::FormatException::what() const throw()
{
    return ("WRONG PASS");
}
