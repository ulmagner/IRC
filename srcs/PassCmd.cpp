/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PassCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 16:05:31 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/22 16:45:01 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PassCmd.hpp"
#include "Serv.hpp"

PassCmd::PassCmd( std::vector<std::string> tokens, Serv& serv ) : ACmd(tokens[0]), _tokens(tokens), _serv(serv) {
	if (tokens.size() < 1 || tokens.size() > 2) {
		throw PassCmd::FormatException();
	}
}

PassCmd::~PassCmd( void ) {}

void PassCmd::executeCmd( Client& client ) {
	std::string tok = "";
	if (this->_tokens.size() > 1)
		tok = this->_tokens[1];
	if (tok != this->_serv.getPass()) {
		throw PassCmd::FormatException();
	}
	client.setPass(tok);
	std::cout << "Serv creates " << ACmd::_ACmdRef << std::endl;
}

const char* PassCmd::FormatException::what() const throw()
{
    return ("WRONG PASS");
}
