/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poker.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 20:11:46 by ulmagner          #+#    #+#             */
/*   Updated: 2025/08/03 19:15:57 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Poker.hpp"

Poker::Poker( void ) : _money(0), _bet(0), _fdPlayer(0), _firstPlayer(0), _turn(false), _river(false) {}

Poker::Poker( int money, std::map<int, std::pair<Client*, int> > clients, int fd ) :  _money(money), _bet(5), _players(clients), _fdPlayer(0), _firstPlayer(fd), _turn(false), _river(false) {
	std::string suits[] = {"heart", "diamond", "spade", "clover"};

	for (size_t i = 0; i < 4; ++i) {
		for (int value = 1; value <= 13; ++value) {
			this->_deck.push_back(Card(value, suits[i]));
		}
	}

	this->_players.clear();
	std::srand(std::time(0));
	std::random_shuffle(this->_deck.begin(), this->_deck.end());
	std::map<int, std::pair<Client*, int> >::iterator it = clients.begin();
	for (; it != clients.end(); ++it) {
		Card c1 = this->_deck.back(); this->_deck.pop_back();
		Card c2 = this->_deck.back(); this->_deck.pop_back();
		it->second.first->setCards(std::make_pair(c1, c2));
		this->_players[it->second.first->getFd()] = std::make_pair(it->second.first, 1);
	}

	for (size_t i = 0; i < 3; ++i) {
		Card flop = this->_deck.back(); this->_deck.pop_back();
		this->_community.push_back(flop);
	}
}

Poker::~Poker( void ) {}

std::string Poker::getCommunity( void ) {
	std::string m = "";
	std::vector<Card>::iterator it = this->_community.begin();
	for (; it != this->_community.end(); ++it) {
		if (it != this->_community.begin())
			m += " ";
		m += it->toString();
	}
	return (m);
}

int Poker::straightFlush( std::vector<Card> com ) {
	std::map<std::string, std::vector<int> > suitedValues;

	for (size_t i = 0; i < com.size(); ++i)
		suitedValues[com[i].getSuits()].push_back(com[i].getValue());

	for (std::map<std::string, std::vector<int> >::iterator it = suitedValues.begin(); it != suitedValues.end(); ++it) {
		std::vector<int>& vals = it->second;
		if (vals.size() < 5)
			continue;

		std::sort (vals.begin(), vals.end());
		vals.erase(std::unique(vals.begin(), vals.end()), vals.end());
		int count = 1;
		int ref = vals[0];
		for (size_t i = 1; i < vals.size(); ++i) {
			if (vals[i] == ref + 1) {
				count++;
				if (count == 5) {
					if (std::find(vals.begin(), vals.end(), 14) != vals.end())
						return (14);
					return (vals[i]);
				}
			}
			else {
				count = 1;
			}
			ref = vals[i];
		}
	}
	return (false);
}

int Poker::square( std::vector<Card> com ) {
	std::map<int, int> valueCount;

	for (size_t i = 0; i < com.size(); ++i) {
		valueCount[com[i].getValue()]++;
	}

	for (std::map<int, int>::iterator it = valueCount.begin(); it != valueCount.end(); ++it) {
		if (it->second == 4)
			return (true);
	}
	return (false);
}

int Poker::full( std::vector<Card> com ) {
	bool hasThree = false;
	bool hasTwo = false;
	std::map<int, int> valueCount;

	for (size_t i = 0; i < com.size(); ++i)
		valueCount[com[i].getValue()]++;

	for (std::map<int, int>::iterator it = valueCount.begin(); it != valueCount.end(); ++it) {
		if (it->second >= 3)
			hasThree = true;
		else if (it->second >= 2)
			hasTwo = true;
	}
	return (hasThree && hasTwo);
}

int Poker::flush( std::vector<Card> com ) {
	std::map<std::string, int> suitsCount;

	for (size_t i = 0; i < com.size(); ++i) {
		suitsCount[com[i].getSuits()]++;
	}

	for (std::map<std::string, int>::iterator it = suitsCount.begin(); it != suitsCount.end(); ++it) {
		if (it->second == 5)
			return (true);
	}
	return (false);
}

int Poker::straight( std::vector<Card> com ) {

	std::vector<int> values;

	for (size_t i = 0; i < com.size(); ++i)
		values.push_back(com[i].getValue());

	std::sort (values.begin(), values.end());
	values.erase(std::unique(values.begin(), values.end()), values.end());

	int count = 1;
	int ref = values[0];
	for (size_t i = 1; i < values.size(); ++i) {
		if (values[i] == ref + 1) {
			count++;
			if (count == 5) {
				if (std::find(values.begin(), values.end(), 14) != values.end())
					return (14);
				return (values[i]);
			}
		}
		else {
			count = 1;
		}
		ref = values[i];
	}
	return (false);
}

int Poker::threeOfKind( std::vector<Card> com ) {
	std::map<int, int> valueCount;

	for (size_t i = 0; i < com.size(); ++i) {
		valueCount[com[i].getValue()]++;
	}

	for (std::map<int, int>::iterator it = valueCount.begin(); it != valueCount.end(); ++it) {
		if (it->second == 3)
			return (true);
	}
	return (false);
}

int Poker::twoPair( std::vector<Card> com ) {
	std::map<int, int> valueCount;

	for (size_t i = 0; i < com.size(); ++i) {
		valueCount[com[i].getValue()]++;
	}

	int pair = 0;
	for (std::map<int, int>::iterator it = valueCount.begin(); it != valueCount.end(); ++it) {
		if (it->second == 2)
			pair++;
	}
	return (pair >= 2);
}

int Poker::pair( std::vector<Card> com ) {
	std::map<int, int> valueCount;

	for (size_t i = 0; i < com.size(); ++i) {
		valueCount[com[i].getValue()]++;
	}

	int pair = 0;
	for (std::map<int, int>::iterator it = valueCount.begin(); it != valueCount.end(); ++it) {
		if (it->second == 2)
			pair++;
	}
	return (pair == 1);
}

int Poker::nothing( std::vector<Card> com ) {
	(void) com;
	return (true);
}

Client* Poker::checkHands( void ) {
	std::string BestHand[] = {"Straight Flush", "Square", "Full", "Flush", "Straight", "Three Of Kind", "Two Pair", "Pair"};
	int (Poker::*comb[])( std::vector<Card> ) = {
		&Poker::straightFlush,
		&Poker::square,
		&Poker::full,
		&Poker::flush,
		&Poker::straight,
		&Poker::threeOfKind,
		&Poker::twoPair,
		&Poker::pair,
		&Poker::nothing,
	};

	std::map<int, std::pair<Client*, int> >::iterator it = this->_players.begin();
	int i = 0;
	int best = 8;
	Client* winner = NULL;
	for (; it != this->_players.end(); ++it) {
		if (it->second.second != 1)
			continue ;
		std::vector<Card> hand = this->_community;
		hand.push_back(it->second.first->getCards().first);
		hand.push_back(it->second.first->getCards().second);
		for (i = 0; i < 9; ++i) {
			if ((this->*comb[i])(hand)) {
				break ;
			}
		}
		if (i < best) {
			best = i;
			winner = it->second.first;
			this->_winRound = BestHand[i];
		}
	}

	if (best == 8) {
		this->_winRound = "Highest Card";
		it = this->_players.begin();
		++it;
		for (; it != this->_players.end(); ++it) {
			if (it->second.second != 1)
				continue ;
			Card c1 = it->second.first->getCards().first;
			Card c2 = it->second.first->getCards().second;
			if (!winner) {
				winner = it->second.first;
				continue ;
			}
			Card ref1 = winner->getCards().first;
			Card ref2 = winner->getCards().second;
			if (c1.getValue() > ref1.getValue() \
				|| ((c1.getValue() == ref1.getValue()) && (c2.getValue() > ref2.getValue()))) {
					winner = it->second.first;
			}
		}
	}
	return (winner);
}

bool Poker::endGame( Channel* channel ) {
	if (this->_players.size() == 1) {
		std::map<int, std::pair<Client *, int> >::const_iterator it = channel->getClients().begin();
		for (;it != channel->getClients().end(); ++it) {
			it->second.first->setMoney(1000);
		}
		return (true);
	}
	return (false);
}

void Poker::resetTurn( void ) {
	std::map<int, std::pair<Client*, int> >::iterator i = this->_players.begin();
	for (; i != this->_players.end(); ++i) {
		if (i->second.first->getMoney() > 0)
			i->second.second = 1;
		else
			i->second.second = 0;
		this->_deck.push_back(i->second.first->getCards().first);
		this->_deck.push_back(i->second.first->getCards().second);
	}
	while (!this->_community.empty()) {
		this->_deck.push_back(this->_community.back());
		this->_community.pop_back();
	}
	std::srand(std::time(0));
	int key = 1;
	std::map<int, std::pair<Client*, int> > newPlayers;
	std::map<int, std::pair<Client*, int> >::iterator it = this->_players.begin();
	for (; it != this->_players.end(); ++it) {
		if (it->second.first->getMoney() > 0)
			newPlayers[key++] = std::make_pair(it->second.first, 1);
	}
	this->_players = newPlayers;

	std::random_shuffle(this->_deck.begin(), this->_deck.end());
	for (it = this->_players.begin(); it != this->_players.end(); ++it) {
		if (it->second.second != 1)
			continue ;
		Card c1 = this->_deck.back(); this->_deck.pop_back();
		Card c2 = this->_deck.back(); this->_deck.pop_back();
		it->second.first->setCards(std::make_pair(c1, c2));
	}

	for (size_t i = 0; i < 3; ++i) {
		Card flop = this->_deck.back(); this->_deck.pop_back();
		this->_community.push_back(flop);
	}
	this->_fdPlayer = this->_firstPlayer;
	this->_turn = false;
	this->_river = false;
}

void Poker::addCardToCommunity( void ) {
	Card card1 = this->_deck.back(); this->_deck.pop_back();
	this->_community.push_back(card1);
	if (this->_river && this->_community.size() == 3) {
		Card card2 = this->_deck.back(); this->_deck.pop_back();
		this->_community.push_back(card2);
	}
	this->_fdPlayer = this->_firstPlayer;
}

std::vector<Card>& Poker::getDeck( void ) {
	return (_deck);
}

void Poker::setFold( int fd ) {
	std::map<int, std::pair<Client*, int> >::iterator i = this->_players.find(fd);
	if (i != this->_players.end()) {
		i->second.second = 0;
	}
}

void Poker::setBet( int bet ) {
	_bet += bet;
}

void Poker::setFd( int fd ) {
	_fdPlayer = fd;
}

void Poker::setMoney( int money ) {
	_money += money;
}

int Poker::getMoney( void ) {
	return (_money);
}

void Poker::setTurn( bool s ) {
	_turn = s;
}

bool Poker::getTurn( void ) {
	return (_turn);
}

void Poker::setRiver( bool s ) {
	_river = s;
}

bool Poker::getRiver( void ) {
	return (_river);
}

int Poker::getBet( void ) {
	return (_bet);
}

int Poker::getFdPlayer( void ) {
	return (_firstPlayer);
}

std::string& Poker::getWinRound( void ) {
	return (this->_winRound);
}

Client* Poker::getNextPlayer( int fd ) {
	if (_players.size() <= 1) {
		return (NULL);
	}

	std::map<int, std::pair<Client*, int> >::iterator it = _players.find(fd);
	std::map<int, std::pair<Client*, int> >::iterator at = _players.begin();
	for (; at != _players.end(); ++at) {
		std::cout << at->second.second << std::endl;
	}
	if (it == _players.end()) {
		return (NULL);
	}

	std::map<int, std::pair<Client*, int> >::iterator start = it;
	do {
		++it;
		if (it == _players.end())
			it = _players.begin();

		if (it->second.second == 1) {
			this->_fdPlayer = it->second.first->getFd();
			return (it->second.first);
		}
	} while (start != it);
	return (NULL);
}
