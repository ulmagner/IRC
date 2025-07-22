/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 16:05:31 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/22 16:45:48 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "UserCmd.hpp"
#include "Serv.hpp"

UserCmd::UserCmd( std::vector<std::string> tokens, Serv& serv ) : ACmd(tokens[0]), _tokens(tokens), _serv(serv) {
	if (tokens.size() < 1 || tokens.size() > 2) {
		throw UserCmd::FormatException();
	}
}

UserCmd::~UserCmd( void ) {}

void UserCmd::executeCmd( Client& client ) {
	if (client.getPass().empty())
		throw UserCmd::ErrorException();
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

const char* UserCmd::FormatException::what() const throw()
{
    return ("FORMAT USER");
}
