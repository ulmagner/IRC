/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NickCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 16:05:31 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/22 16:45:10 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "NickCmd.hpp"
#include "Serv.hpp"

NickCmd::NickCmd( std::vector<std::string> tokens, Serv& serv ) : ACmd(tokens[0]), _tokens(tokens), _serv(serv) {
	if (tokens.size() < 1 || tokens.size() > 2) {
		throw NickCmd::FormatException();
	}
}

NickCmd::~NickCmd( void ) {}

void NickCmd::executeCmd( Client& client ) {
    std::cout << ",,,,,,," << std::endl;
	if (client.getPass().empty())
		throw NickCmd::ErrorException();
	if (this->_tokens.size() == 1)
		throw NickCmd::FormatException();
	client.setNick(this->_tokens[1]);
    std::map<int, Client>::const_iterator it = this->_serv.getConnections().begin();
    for (; it != this->_serv.getConnections().end(); ++it) {
        if (!it->second.getNick().empty() && it->first != client.getFd() && !it->second.getNick().compare(this->_tokens[1])) {
			client.setNick("");
			throw NickCmd::ErrorException();
		}
    }
	std::cout << "Serv creates " << ACmd::_ACmdRef << std::endl;
}

const char* NickCmd::FormatException::what() const throw()
{
    return ("WRONG PASS");
}
