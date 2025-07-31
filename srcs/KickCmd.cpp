/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KickCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 14:30:32 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/31 10:43:17 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "KickCmd.hpp"
#include "Serv.hpp"

KickCmd::KickCmd( std::vector<std::string> tokens, Serv& serv ) : ACmd(tokens[0]), _tokens(tokens), _serv(serv) {}

KickCmd::~KickCmd( void ) {}

void sendToChannelClient( Channel* channel, std::string& msg ) {
	std::map<int, std::pair<Client *, int> >::const_iterator it = channel->getClients().begin();
	for (;it != channel->getClients().end(); ++it) {
		send(it->second.first->getFd(), msg.c_str(), msg.size(), 0);
	}
}

void KickCmd::executeCmd( Client& client ) {
	std::string m = "";
	if (this->_tokens.size() < 2) {
		m = ERR_NEEDMOREPARAMS(client.getNick(), this->_tokens[0]);
		send(client.getFd(), m.c_str(), m.size(), 0);
		throw KickCmd::FormatException();
	}
	std::string name = this->_tokens[1];
	if (name[0] != '#') {
		m = ERR_NOSUCHCHANNEL(client.getNick(), name);
		send(client.getFd(), m.c_str(), m.size(), 0);
		throw KickCmd::FormatException();
	}
	Channel* channel = this->_serv.getChannelByName(name);
	if (!channel) {
		m = ERR_NOSUCHCHANNEL(client.getNick(), name);
		send(client.getFd(), m.c_str(), m.size(), 0);
		throw KickCmd::FormatException();
	}
	if (!channel->hasPerm(client)) {
		m = ERR_CHANOPRIVSNEEDED(client.getNick(), client.getUser(), channel->getName());
		send(client.getFd(), m.c_str(), m.size(), 0);
		throw KickCmd::FormatException();
	}
	std::map<int, std::pair<Client *, int> >& cl = channel->getClients();
	std::vector<std::string> cl_name = split(this->_tokens[2], ',');
	std::string reason = (this->_tokens.size() == 4) ? this->_tokens[3] : "because";
	std::vector<std::string>::const_iterator cl_it = cl_name.begin();
	bool is = false;
	for (;cl_it != cl_name.end(); ++cl_it) {
		is = false;
		for (std::map<int, std::pair<Client *, int> >::iterator it = cl.begin(); it != cl.end(); ) {
			std::string kickMsg = "";
			if (it->second.first->getUser() == *cl_it) {
				is = true;
				if (*cl_it == client.getUser()) {
					kickMsg = ":IRC_SERVER 999 " + client.getNick() + " " + channel->getName() + " :You cannot kick yourself\r\n";
					send(client.getFd(), kickMsg.c_str(), kickMsg.size(), 0);
					break ;
				}
				m = KICK_MSG(client.getNick(), client.getUser(), channel->getName(), *cl_it, reason);
				sendToChannelClient(channel, m);
				cl.erase(it++);
				break ;
			} else {
				++it;
			}
		}
		if (!is) {
			m = ERR_USERNOTINCHANNEL(client.getNick(), *cl_it, channel->getName());
			send(client.getFd(), m.c_str(), m.size(), 0);
		}
	}
}

const char* KickCmd::FormatException::what() const throw()
{
	return ("Kick FORMAT");
}
