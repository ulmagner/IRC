/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poker.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 20:05:47 by ulmagner          #+#    #+#             */
/*   Updated: 2025/08/02 02:58:28 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Poker_hpp
#define Poker_hpp
// #include "Client.hpp"
#include "Channel.hpp"
// #include "Card.hpp"
#include <ctime>
#include <cstdlib>
#include <map>
#include <algorithm>
#include <vector>

class Card;
class Client;
class Channel;

class Poker {
    private:
		int _money;
		std::vector<Card> _deck;
		std::vector<Card> _community;
		int	_bet;
		std::map<int, std::pair<Client*, int> > _players;
		int _fdPlayer;
		int _firstPlayer;
		bool _turn;
		bool _river;
		std::string _winRound;
    public:
        Poker( void );
        Poker( int money, std::map<int, std::pair<Client*, int> > clients, int fd );
        ~Poker( void );
		std::vector<Card>& getDeck( void );
		void setFold( int fd );
		void setBet( int bet );
		void setFd( int fd );
		void setTurn( bool s );
		bool getTurn( void );
		void setRiver( bool s );
		bool getRiver( void );
		void setMoney( int money );
		void resetTurn( void );
		void addCardToCommunity( void );
		bool endGame( Channel* channel );
		int straightFlush( std::vector<Card> com );
		int square( std::vector<Card> com );
		int full( std::vector<Card> com );
		int flush( std::vector<Card> com );
		int straight( std::vector<Card> com );
		int threeOfKind( std::vector<Card> com );
		int twoPair( std::vector<Card> com );
		int pair( std::vector<Card> com );
		int nothing( std::vector<Card> com );
		Client* checkHands( void );
		std::string getCommunity( void );
		std::string& getWinRound( void );
		int getMoney( void );
		int getBet( void );
		int getFdPlayer( void );
		Client* getNextPlayer( int fd );
};

#endif // Poker.hpp