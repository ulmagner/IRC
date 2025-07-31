/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poker.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 20:11:46 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/31 20:15:29 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Poker.hpp"

Poker::Poker( void ) : _money(0), _bet(0), _fdPlayer(0) {}

Poker::Poker( int money, std::map<int, std::pair<Client*, int> > clients ) :  _money(money), _bet(5), _players(clients), _fdPlayer(0) {			std::string suits[] = {"heart", "diamond", "spade", "clover"};

	for (size_t i = 0; i < 4; ++i) {
		for (int value = 1; value <= 13; ++value) {
			this->_deck.push_back(Card(value, suits[i]));
		}
	}

	_players.clear();
	int key = 1;
	std::srand(std::time(0));
	std::random_shuffle(this->_deck.begin(), this->_deck.end());
	std::map<int, std::pair<Client*, int> >::iterator it = clients.begin();
	for (; it != clients.end(); ++it, ++key) {
		Card c1 = _deck.back(); _deck.pop_back();
		Card c2 = _deck.back(); _deck.pop_back();
		it->second.first->setCards(std::make_pair(c1, c2));
		_players[key] = std::make_pair(it->second.first, 1);
	}
}

Poker::~Poker( void ) {}

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
	_bet = bet;
}

void Poker::setFd( int fd ) {
	_fdPlayer = fd;
}

void Poker::setMoney( int money ) {
	_money = money;
}

int Poker::getMoney( void ) {
	return (_money);
}

int Poker::getBet( void ) {
	return (_bet);
}

Client* Poker::getNextPlayer( int fd ) {
	std::map<int, std::pair<Client*, int> >::iterator i = this->_players.find(fd);
	std::map<int, std::pair<Client*, int> >::iterator a = i;
	while (i != a) {
		if (i != this->_players.end()) {
			++i;
			if (i == _players.end())
				i = this->_players.begin();
			if (i->second.second == 1) {
				return (i->second.first);
			}
		}
	}
	return (NULL);
}