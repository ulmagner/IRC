/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 12:47:28 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/29 15:23:50 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Channel_hpp
#define Channel_hpp
#include <string>
#include <exception>
#include <iostream>
#include <map>
#include <vector>
#include "Client.hpp"

class Channel {
    private:
		std::string _name;
		std::string _key;
        std::string _topic;
        std::vector<std::string> _mode;
        std::string _topicSetter;
        time_t _topicSetTime;
        std::map<int, Client> _clientConnected;
        std::vector<Client> _inviteClient;
    public:
        Channel( std::string& name, std::string& key, Client& client );
        ~Channel( void );
		int getFd( void ) const;
		const std::string& getName( void ) const;
		const std::string& getKey( void ) const;
        const std::string& getTopic( void ) const;
        const std::string& getTopicSetter( void ) const;
        void setTopicSetter( const std::string& nick );
        std::map<int, Client>& getClients( void );
        bool hasAlreadyJoin( int fd );
		void setName( const std::string& attPath );
		void setKey( const std::string& attNick );
        void setTopic( const std::string& topic );
        void addClient( Client& client );
        void addMode( std::string& mode );
        bool getMode( const std::string& mode ) const;
        void setTopicSetTime( time_t time );
        time_t getTopicSetTime( void ) const;
        void addToInvite( Client& client );
        void eraseFromInvite( const std::string& nick );
        bool hasPerm( Client& client ) const;
        const Client* getInvite( const std::string& name ) const;
        Client* getClientByName( const std::string& name );
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