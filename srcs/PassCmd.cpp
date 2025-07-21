/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PassCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 16:05:31 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/21 15:10:54 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PassCmd.hpp"
#include "Serv.hpp"

PassCmd::PassCmd( std::vector<std::string> tokens, Serv& serv ) : ACmd(tokens[0]), _PassCmdRef(tokens[1]), _serv(serv) {}

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
