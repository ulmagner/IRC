/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Serv.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 11:55:09 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/18 16:23:54 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef Serv_hpp
#define Serv_hpp
#include "ACmd.hpp"
#include "PassCmd.hpp"

class ACmd;
class PassCmd;

class Serv {
    private:
        int _port;
        std::string _pass;
        int _socketfd;
        int _epollfd;
        std::vector<int> _connections;
        ACmd* _cmd;
        int isValidPort( const std::string& port ) const;
        void isValidPass( const std::string& pass ) const;
        void createTcpServerSocket( void );
    public:
        Serv( char **arg );
        ~Serv( void );
        void shutdown( void );
        void run( void );
        ACmd* pass( std::vector<std::string> tokens ) const;
        ACmd* getCmd( char* buffer ) const;
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

extern Serv* g_serv;

#endif //Serv_hpp