/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrvCmd.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 19:07:20 by ulmagner          #+#    #+#             */
/*   Updated: 2025/08/03 19:06:29 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PrvCmd.hpp"
#include "Serv.hpp"

PrvCmd::PrvCmd( std::vector<std::string> tokens, Serv& serv ) : ACmd(tokens[0]), _tokens(tokens), _serv(serv) {}

PrvCmd::~PrvCmd( void ) {}

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
		m = "<<<<<<<<<<< NEW TURN >>>>>>>>>>>\n\n\r\n";
		send(it->second.first->getFd(), m.c_str(), m.size(), 0);
		m = MSG_COLOR "The Flop: " RESET_COLOR + poker->getCommunity() + "\n\r\n";
		send(it->second.first->getFd(), m.c_str(), m.size(), 0);
		m = ERR_COLOR "Your cards: " RESET_COLOR + it->second.first->getCards().first.toString() + " " + it->second.first->getCards().second.toString() + "\n\r\n";
		send(it->second.first->getFd(), m.c_str(), m.size(), 0);
		m = RPL_COLOR "You have: " RESET_COLOR + itoa(it->second.first->getMoney()) + "\n\r\n";
		send(it->second.first->getFd(), m.c_str(), m.size(), 0);
	}
}

void PrvCmd::startPoker( Channel* channel, Client& client ) {
	std::string m = "";
	this->_serv._poker = new Poker(0, channel->getClients(), client.getFd());
	channel->setPlaying(true);
	annonceTurn(this->_serv._poker, channel);
	m = "Check for: " + itoa(this->_serv._poker->getBet()) + " Fold or Ask:\n\r\n";
	send(client.getFd(), m.c_str(), m.size(), 0);
	client.setPlaying(true);
	this->_serv._poker->setFd(client.getFd());
}

void PrvCmd::PokerTurn( Client& client, Channel* channel, std::string& reason ) {
	std::string m = "";
	int bet = this->_serv._poker->getBet();
	if (reason.substr(1) == "Fold") {
		this->_serv._poker->setFold(client.getFd());
	}
	else if (reason.substr(1) == "Check") {
		if (client.getMoney() < bet) {
			bet = client.getMoney();
			this->_serv._poker->setRiver(true);
		}
		client.bet(bet);
		this->_serv._poker->setMoney(bet);
	}
	else if (reason.substr(1) == "Ask") {
		client.bet(stringToInt(this->_tokens[3]));
		this->_serv._poker->setMoney(stringToInt(this->_tokens[3]));
		this->_serv._poker->setBet(stringToInt(this->_tokens[3]));
	}
	else {
		return ;
	}
	client.setPlaying(false);
	m = client.getNick() + " " + reason + "\r\n";
	sendToChannelClient(channel, m);
	std::cout << m << std::endl;
	Client *next = this->_serv._poker->getNextPlayer(client.getFd());
	if (!next) {
		std::cerr << "Error: getNextPlayer returned NULL\n";
		return ;
	}
	if (next->getFd() == this->_serv._poker->getFdPlayer()) {
		if (this->_serv._poker->getTurn() && this->_serv._poker->getRiver()) {
			Client *win = this->_serv._poker->checkHands();
			win->addMoney(this->_serv._poker->getMoney());
			m = MSG_COLOR + win->getNick() + RESET_COLOR " WON " + itoa(this->_serv._poker->getMoney()) + " WITH A " + ERR_COLOR + this->_serv._poker->getWinRound() + RESET_COLOR "\n\r\n";
			sendToChannelClient(channel, m);
			std::cout << m << std::endl;
			if (this->_serv._poker->endGame(channel)) {
				std::string m = RPL_COLOR + win->getNick() + RESET_COLOR " WON THE POKER\r\n";
				sendToChannelClient(channel, m);
				std::cout << m << std::endl;
				channel->setPlaying(false);
				delete this->_serv._poker;
				return ;
			}
			this->_serv._poker->resetTurn();
			annonceTurn(this->_serv._poker, channel);
		}
		this->_serv._poker->addCardToCommunity();
		if (!this->_serv._poker->getTurn() && !this->_serv._poker->getRiver()) {
			m = MSG_COLOR "The Turn: " + this->_serv._poker->getCommunity() + RESET_COLOR "\r\n";
			this->_serv._poker->setTurn(true);
		}
		else if (!this->_serv._poker->getRiver()) {
			m = MSG_COLOR "The River: " + this->_serv._poker->getCommunity() + RESET_COLOR "\r\n";
			this->_serv._poker->setRiver(true);
		}
		sendToChannelClient(channel, m);
		std::cout << m << std::endl;
	}
	next->setPlaying(true);
	m = "Check for: " + itoa(this->_serv._poker->getBet()) + " or Fold:";
	if (next->getMoney() > this->_serv._poker->getBet())
		m += " or Ask:";
	m += "\n\r\n";
	send(next->getFd(), m.c_str(), m.size(), 0);
	m = RPL_COLOR "You have: " RESET_COLOR + itoa(next->getMoney()) + "\n\r\n";
	send(next->getFd(), m.c_str(), m.size(), 0);
}

void PrvCmd::executeCmd( Client& client ) {
	std::string m = "";
	if (this->_tokens.size() < 3) {
		m = ERR_NEEDMOREPARAMS(client.getNick(), this->_tokens[0]);
		send(client.getFd(), m.c_str(), m.size(), 0);
		std::cout << m << std::endl;
		throw PrvCmd::FormatException();
	}
	std::vector<std::string> nicks = split(this->_tokens[1], ',');
	std::string reason = this->_tokens[2];
	if (reason.empty() || reason[0] != ':')
		reason = ":" + reason;
	for (size_t i = 3; i < this->_tokens.size(); ++i) {
		reason += " ";
		reason += this->_tokens[i];
	}
	std::vector<std::string>::const_iterator itt = nicks.begin();
	std::string name = "";
	for (;itt != nicks.end(); ++itt) {
		name = *itt;
		if (name[0] == '#') {
			Channel* channel = this->_serv.getChannelByName(name);
			if (!channel) {
				m = ERR_NOSUCHCHANNEL(client.getNick(), name);
				std::cout << m << std::endl;
				send(client.getFd(), m.c_str(), m.size(), 0);
				std::cout << m << std::endl;
				continue ;
			}
			if (!channel->getClientByName(client.getNick())) {
				m = ERR_NOTONCHANNEL(client.getNick(), channel->getName());
				std::cout << m << std::endl;
				send(client.getFd(), m.c_str(), m.size(), 0);
				std::cout << m << std::endl;
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
				std::cout << m << std::endl;
				if (it->second.first->getFd() == client.getFd())
					continue ;
				send(it->second.first->getFd(), m.c_str(), m.size(), 0);
				std::cout << m << std::endl;
			}
		}
		else {
			Client* cl = this->_serv.getClientByName(name);
			if (!cl) {
				m = ERR_NOSUCHNICK(client.getNick(), this->_tokens[1]);
				send(client.getFd(), m.c_str(), m.size(), 0);
				std::cout << m << std::endl;
				throw PrvCmd::FormatException();
			}
			m = SEND_PRIVMSG(client.getNick(), client.getUser(), cl->getNick(), reason);
			std::cout << m << std::endl;
			send(cl->getFd(), m.c_str(), m.size(), 0);
			std::cout << m << std::endl;
		}
	}
}

const char* PrvCmd::FormatException::what() const throw()
{
    return ("WRONG PASSs");
}
