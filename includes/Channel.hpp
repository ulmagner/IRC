/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 12:47:28 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/28 12:49:56 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Channel_hpp
#define Channel_hpp
#include <string>
#include <exception>
#include <iostream>
#include <map>
#include "Client.hpp"

class Channel {
    private:
		std::string _name;
		std::string _key;
        std::map<int, Client> _clientConnected;
    public:
        Channel( std::string& name, std::string& key, Client& client );
        ~Channel( void );
		int getFd( void ) const;
		const std::string& getName( void ) const;
		const std::string& getKey( void ) const;
        std::map<int, Client>& getClients( void );
        bool hasAlreadyJoin( int fd );
		void setName( const std::string& attPath );
		void setKey( const std::string& attNick );
        void addClient( Client& client );
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

#endif //Channel_hpp