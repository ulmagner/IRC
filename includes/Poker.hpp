/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poker.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 20:05:47 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/31 20:15:27 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Poker_hpp
#define Poker_hpp
#include "Client.hpp"
#include "Card.hpp"
#include <ctime>
#include <cstdlib>
#include <map>
#include <algorithm>
#include <vector>

class Client;

class Poker {
    private:
		int _money;
		std::vector<Card> _deck;
		int	_bet;
		std::map<int, std::pair<Client*, int> > _players;
		int _fdPlayer;
    public:
        Poker( void );
        Poker( int money, std::map<int, std::pair<Client*, int> > clients );
        ~Poker( void );
		std::vector<Card>& getDeck( void );
		void setFold( int fd );
		void setBet( int bet );
		void setFd( int fd );
		void setMoney( int money );
		int getMoney( void );
		int getBet( void );
		Client* getNextPlayer( int fd );
};

#endif // Poker.hpp