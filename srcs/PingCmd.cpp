/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PingCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 19:21:37 by ulmagner          #+#    #+#             */
/*   Updated: 2025/08/03 19:35:20 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PingCmd.hpp"
#include "Serv.hpp"

PingCmd::PingCmd( std::vector<std::string> tokens, Serv& serv ) : ACmd(tokens[0]), _tokens(tokens), _serv(serv) {}

PingCmd::~PingCmd( void ) {}

void PingCmd::executeCmd( Client& client ) {
	std::string m = PONG(this->_tokens[1]);
    send(client.getFd(), m.c_str(), m.size(), 0);
    std::cout << m << std::endl;
}

const char* PingCmd::FormatException::what() const throw()
{
    return ("WRONG PASSs");
}
