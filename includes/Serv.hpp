/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Serv.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 11:55:09 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/22 14:36:03 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Serv_hpp
#define Serv_hpp
#include <vector>
#include <map>
#include <exception>
#include "ACmd.hpp"
#include "Client.hpp"
#include "Channel.hpp"

class Serv {
    friend class PassCmd;
    friend class NickCmd;
    friend class UserCmd;
    private:
        int _port;
        std::string _pass;
        int _socketfd;
        int _epollfd;
        std::map<int, Client> _connections;
        std::vector<Channel> _channels;
        int isValidPort( const std::string& port ) const;
        void isValidPass( const std::string& pass ) const;
        void createTcpServerSocket( void );
    public:
        Serv( char **arg );
        ~Serv( void );
        void shutdown( void );
        void run( void );
        const std::string& getPass( void ) const;
        const std::map<int, Client>& getConnections() const;
        const std::vector<Channel>& getChannels() const;
        ACmd* pass( std::vector<std::string> tokens );
        ACmd* nick( std::vector<std::string> tokens );
        ACmd* user( std::vector<std::string> tokens );
        ACmd* join( std::vector<std::string> tokens );
        ACmd* getCmd( char* buffer, Client& client );
        Client& getClientByFd( int fd );
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
        class AlreadyAuthenticateException : public std::exception
        {
            public:
                virtual const char* what() const throw();
        };
        class NotAuthYetException : public std::exception
        {
            public:
                virtual const char* what() const throw();
        };
};

extern Serv* g_serv;

#endif //Serv_hpp