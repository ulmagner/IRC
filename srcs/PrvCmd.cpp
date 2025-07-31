/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrvCmd.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 19:07:20 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/30 20:58:36 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PrvCmd.hpp"
#include "Serv.hpp"

PrvCmd::PrvCmd( std::vector<std::string> tokens, Serv& serv ) : ACmd(tokens[0]), _tokens(tokens), _serv(serv) {}

PrvCmd::~PrvCmd( void ) {}

void PrvCmd::executeCmd( Client& client ) {
	std::string m = "";
	if (this->_tokens.size() < 3) {
		m = ERR_NEEDMOREPARAMS(client.getNick(), this->_tokens[0]);
		send(client.getFd(), m.c_str(), m.size(), 0);
		throw PrvCmd::FormatException();
	}
	std::vector<std::string> nicks = split(this->_tokens[1], ',');
	std::string reason = this->_tokens[2];
	if (reason[0] != ':')
		reason = ":" + reason;
	std::vector<std::string>::const_iterator itt = nicks.begin();
	std::string name = "";
	for (;itt != nicks.end(); ++itt) {
		name = *itt;
		if (name[0] == '#') {
			Channel* channel = this->_serv.getChannelByName(name);
			if (!channel) {
				m = ERR_NOSUCHCHANNEL(client.getNick(), name);
				send(client.getFd(), m.c_str(), m.size(), 0);
				continue ;
			}
			if (!channel->getClientByName(client.getNick())) {
				m = ERR_NOTONCHANNEL(client.getNick(), channel->getName());
				send(client.getFd(), m.c_str(), m.size(), 0);
				continue ;
			}
			m = ":" + client.getPrefix() + " PRIVMSG " + channel->getName() + " " + reason + "\r\n";
			std::map<int, std::pair<Client *, int> >::const_iterator it = channel->getClients().begin();
			for (;it != channel->getClients().end(); ++it) {
				if (it->second.first->getFd() == client.getFd())
					continue ;
				send(it->second.first->getFd(), m.c_str(), m.size(), 0);
			}
		}
		else {
			Client* cl = this->_serv.getClientByName(name);
			if (!cl) {
				m = ERR_NOSUCHNICK(client.getNick(), this->_tokens[1]);
				send(client.getFd(), m.c_str(), m.size(), 0);
				throw PrvCmd::FormatException();
			}
			m = SEND_PRIVMSG(client.getNick(), client.getUser(), cl->getNick(), reason);
			send(cl->getFd(), m.c_str(), m.size(), 0);
		}
	}
}

const char* PrvCmd::FormatException::what() const throw()
{
    return ("WRONG PASSs");
}
