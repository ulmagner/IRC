/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 11:03:54 by ulmagner          #+#    #+#             */
/*   Updated: 2025/08/13 17:56:07 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Client_hpp
#define Client_hpp
#include <string>
#include <exception>
#include <iostream>
#include <vector>
#include "Card.hpp"
#include "Poker.hpp"

class Client {
    private:
        int _fd;
        std::string _user;
        std::string _nick;
        std::string _host;
        std::string _real;
        std::string _pass;
        std::vector<std::string> _mode;
		bool _isAuth;
        bool _operator;
        int _money;
        std::pair<Card, Card> _cards;
        bool _playing;
    public:
        // std::vector<std::string> _buff;
        std::string _buff;
        int _d;
        Client( int fd );
        ~Client( void );
		int getFd( void ) const;
        int getMoney( void ) const;
        void addMoney( int m );
        void setMoney( int m );
		const std::string& getNick( void ) const;
		const std::string& getUser( void ) const;
		const std::string& getPass( void ) const;
		const bool& getOp( void ) const;
		const bool& getAuth( void ) const;
        std::string getPrefix( void ) const;
		void setPass( const std::string& attPath );
		void setNick( const std::string& attNick );
		void setUser( const std::string& attUser );
		void setAuth( bool attAuth );
		void setOp( bool op );
        void setPlaying( bool s );
        bool getPlaying( void );
        void bet( int bet );
		bool checkAuth( void );
        void addMode( const std::string& mode );
        void removeMode( const std::string& mode );
        std::string getMode( void );
        std::pair<Card, Card>& getCards( void );
        void setCards( const std::pair<Card, Card>& cards );
        bool hasMode( const std::string& mode ) const;
        class FormatException : public std::exception
        {
            public:
                virtual const char* what() const throw();
        };
        class ErrorException : public std::exception
        {
            public:
                virtual const char* what() const throw();
        };
        class CmdNotFoundException : public std::exception
        {
            public:
                virtual const char* what() const throw();
        };
};

#endif //Client_hpp