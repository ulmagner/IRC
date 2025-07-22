/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PassCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 16:05:31 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/22 11:10:08 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PassCmd.hpp"
#include "Serv.hpp"

PassCmd::PassCmd( std::vector<std::string> tokens, Serv& serv ) : ACmd(tokens[0]), _PassCmdRef(""), _serv(serv) {
	if (tokens.size() > 1 && !tokens[1].empty()) {
		this->_PassCmdRef = tokens[1];
	}
}

PassCmd::~PassCmd( void ) {}

void PassCmd::executeCmd( Client& client ) const {
	if (this->_PassCmdRef.compare(this->_serv.getPass())) {
		throw PassCmd::FormatException();
	}
	client.setPath(_PassCmdRef);
	std::cout << "Serv creates " << ACmd::_ACmdRef << std::endl;
}

const char* PassCmd::FormatException::what() const throw()
{
    return ("WRONG PASS");
}
