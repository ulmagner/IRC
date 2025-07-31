/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrvCmd.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 19:07:20 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/31 19:54:39 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PrvCmd.hpp"
#include "Serv.hpp"

PrvCmd::PrvCmd( std::vector<std::string> tokens, Serv& serv ) : ACmd(tokens[0]), _tokens(tokens), _serv(serv) {}

PrvCmd::~PrvCmd( void ) {
	delete this->_poker;
}

int stringToInt(const std::string& str) {
    std::stringstream ss(str);
    int result;
    ss >> result;
    return result;
}

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
			if (reason == ":!bot" && !channel->getPlaying()) {
				this->_poker = new Poker(0, channel->getClients());
				channel->setPlaying(true);
				std::map<int, std::pair<Client *, int> >::const_iterator it = channel->getClients().begin();
				for (;it != channel->getClients().end(); ++it) {
					m = "Your cards: " + it->second.first->getCards().first.toString() + " " + it->second.first->getCards().second.toString() + "\r\n";
					send(it->second.first->getFd(), m.c_str(), m.size(), 0);
				}
				m = "Check Fold Ask:\r\n";
				send(client.getFd(), m.c_str(), m.size(), 0);
				client.setPlaying(true);
				continue ;
			}
			if (client.getPlaying()) {
				int bet = this->_poker->getBet();
				if (reason.substr(1) == "Fold") {
					this->_poker->setFold(client.getFd());
				}
				else if (reason.substr(1) == "Check") {
					client.bet(bet);
					this->_poker->setMoney(bet);
				}
				else if (reason.substr(1) == "Ask") {
					client.bet(stringToInt(this->_tokens[3]));
					this->_poker->setMoney(stringToInt(this->_tokens[3]));
					this->_poker->setBet(stringToInt(this->_tokens[3]));
				}
				client.setPlaying(false);
				Client *next = this->_poker->getNextPlayer(client.getFd());
				// if (!next) {

				// }
				next->setPlaying(true);
				m = "Check Fold Ask:\r\n";
				send(next->getFd(), m.c_str(), m.size(), 0);
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
