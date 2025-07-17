/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Serv.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 11:55:09 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/17 12:21:35 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef Serv_hpp
#define Serv_hpp
#define MAX_EVENTS 1024
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <exception>
#include <deque>
#include <vector>
#include <list>
#include <algorithm>
#include <ctime>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>

class Serv {
    private:
        int _port;
        std::string _pass;
        int _socketfd;
        int _epollfd;
        std::vector<int> _connections;
        int isValidPort( const std::string& port ) const;
        void isValidPass( const std::string& pass ) const;
        void createTcpServerSocket( void );
    public:
        Serv( char **arg );
        ~Serv( void );
        void run( void );
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
};

#endif //Serv_hpp