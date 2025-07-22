/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 12:47:28 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/22 14:07:31 by ulmagner         ###   ########.fr       */
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
		void setName( const std::string& attPath );
		void setKey( const std::string& attNick );
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