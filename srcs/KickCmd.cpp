/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KickCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 14:30:32 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/30 13:04:17 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "KickCmd.hpp"
#include "Serv.hpp"

KickCmd::KickCmd( std::vector<std::string> tokens, Serv& serv ) : ACmd(tokens[0]), _tokens(tokens), _serv(serv) {}

KickCmd::~KickCmd( void ) {}

void sendToChannelClient( Channel* channel, std::string& msg ) {
	std::map<int, Client>::const_iterator it = channel->getClients().begin();
	for (;it != channel->getClients().end(); ++it) {
		send(it->second.getFd(), msg.c_str(), msg.size(), 0);
	}
}

void KickCmd::executeCmd( Client& client ) {
	if (this->_tokens.size() < 2 || this->_tokens.size() > 5) {
		this->_serv.sendToClient(client, "461", " " + this->_tokens[0] + ERR_NEEDMOREPARAMS);
		throw KickCmd::FormatException();
	}
	std::string name = this->_tokens[1];
	if (name[0] != '#') {
		this->_serv.sendToClient(client, "403", " " + name + ERR_NOSUCHCHANNEL);
		throw KickCmd::FormatException();
	}
	Channel* channel = this->_serv.getChannelByName(name);
	if (!channel) {
		this->_serv.sendToClient(client, "403", " " + name + ERR_NOSUCHCHANNEL);
		throw KickCmd::FormatException();
	}
	if (!channel->hasPerm(client)) {
        this->_serv.sendToClient(client, "482", " " + channel->getName() + ERR_CHANOPRIVSNEEDED);
		throw KickCmd::FormatException();
	}
	std::map<int, Client>& cl = channel->getClients();
	std::vector<std::string> cl_name = split(this->_tokens[2], ',');
	std::string reason = (this->_tokens.size() == 4) ? this->_tokens[3] : "";
	std::vector<std::string>::const_iterator cl_it = cl_name.begin();
	bool is = false;
	for (;cl_it != cl_name.end(); ++cl_it) {
		is = false;
		for (std::map<int, Client>::iterator it = cl.begin(); it != cl.end(); ) {
			std::string kickMsg = "";
			if (it->second.getUser() == client.getUser()) {
				is = true;
				kickMsg = ":IRC_SERVER 999 " + client.getNick() + " " + channel->getName() + " :You cannot kick yourself\r\n";
				send(client.getFd(), kickMsg.c_str(), kickMsg.size(), 0);
				break ;
			}
			if (it->second.getUser() == *cl_it) {
				is = true;
				kickMsg = ":" + client.getPrefix() + " KICK " + channel->getName() + " " + *cl_it + " " + reason + "\r\n";
				std::cout << kickMsg << std::endl;
				sendToChannelClient(channel, kickMsg);
				cl.erase(it++);	
				break ;
			} else {
				++it;
			}
		}
		if (!is) {
			this->_serv.sendToClient(client, "441", " " + *cl_it + " " + channel->getName() + ERR_USERNOTINCHANNEL);
		}
	}
}

const char* KickCmd::FormatException::what() const throw()
{
	return ("Kick FORMAT");
}
