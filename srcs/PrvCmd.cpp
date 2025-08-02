/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrvCmd.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 19:07:20 by ulmagner          #+#    #+#             */
/*   Updated: 2025/08/01 23:32:33 by ulmagner         ###   ########.fr       */
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

std::string itoa(int value) {
	std::stringstream ss;
	ss << value;
	return ss.str();
}

void PrvCmd::annonceTurn( Poker* poker, Channel* channel ) {
	std::string m = "";
	std::map<int, std::pair<Client *, int> >::const_iterator it = channel->getClients().begin();
	for (;it != channel->getClients().end(); ++it) {
		m = "The Flop: " + poker->getCommunity() + "\r\n";
		send(it->second.first->getFd(), m.c_str(), m.size(), 0);
		m = "Your cards: " + it->second.first->getCards().first.toString() + " " + it->second.first->getCards().second.toString() + "\r\n";
		send(it->second.first->getFd(), m.c_str(), m.size(), 0);
		m = "You have: " + itoa(it->second.first->getMoney()) + "\r\n";
		send(it->second.first->getFd(), m.c_str(), m.size(), 0);
	}
}

void PrvCmd::startPoker( Channel* channel, Client& client ) {
	std::string m = "";
	this->_poker = new Poker(0, channel->getClients(), client.getFd());
	channel->setPlaying(true);
	annonceTurn(this->_poker, channel);
	m = "Check for: " + itoa(this->_poker->getBet()) + " Fold or Ask:\r\n";
	send(client.getFd(), m.c_str(), m.size(), 0);
	client.setPlaying(true);
	this->_poker->setFd(client.getFd());
}

void PrvCmd::PokerTurn( Client& client, Channel* channel, std::string& reason ) {
	std::string m = "";
	int bet = this->_poker->getBet();
	if (reason.substr(1) == "Fold") {
		this->_poker->setFold(client.getFd());
	}
	else if (reason.substr(1) == "Check") {
		if (client.getMoney() < bet) {
			bet = client.getMoney();
			this->_poker->setRiver(true);
		}
		client.bet(bet);
		this->_poker->setMoney(bet);
	}
	else if (reason.substr(1) == "Ask") {
		client.bet(stringToInt(this->_tokens[3]));
		this->_poker->setMoney(stringToInt(this->_tokens[3]));
		this->_poker->setBet(stringToInt(this->_tokens[3]));
	}
	client.setPlaying(false);
	m = client.getNick() + " " + reason + "\r\n";
	sendToChannelClient(channel, m);
	Client *next = this->_poker->getNextPlayer(client.getFd());
	if (next->getFd() == this->_poker->getFdPlayer()) {
		if (this->_poker->getTurn() && this->_poker->getRiver()) {
			Client *win = this->_poker->checkHands();
			win->addMoney(this->_poker->getMoney());
			m = win->getNick() + " WON " + itoa(this->_poker->getMoney()) + " WITH A " + this->_poker->getWinRound() + "\r\n";
			sendToChannelClient(channel, m);
			if (this->_poker->endGame(channel)) {
				std::string m = win->getNick() + " WON THE POKER\r\n";
				sendToChannelClient(channel, m);
				channel->setPlaying(false);
				delete this->_poker;
				return ;
			}
			this->_poker->resetTurn();
			annonceTurn(this->_poker, channel);
		}
		this->_poker->addCardToCommunity();
		if (!this->_poker->getTurn() && !this->_poker->getRiver()) {
			m = "The Turn: " + this->_poker->getCommunity() + "\r\n";
			this->_poker->setTurn(true);
			this->_poker->setRiver(true);
		}
		else if (this->_poker->getRiver()) {
			m = "The River: " + this->_poker->getCommunity() + "\r\n";
		}
		sendToChannelClient(channel, m);
	}
	next->setPlaying(true);
	m = "Check for: " + itoa(this->_poker->getBet()) + " or Fold:";
	if (next->getMoney() > this->_poker->getBet())
		m += " or Ask:";
	m += "\r\n";
	send(next->getFd(), m.c_str(), m.size(), 0);
	m = "You have: " + itoa(next->getMoney()) + "\r\n";
	send(next->getFd(), m.c_str(), m.size(), 0);
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
				return (this->startPoker(channel, client));
			}
			if (client.getPlaying()) {
				return (this->PokerTurn(client, channel, reason));
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
