/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AACmd.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 14:22:07 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/18 15:05:28 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ACmd_hpp
#define ACmd_hpp
#define MAX_EVENTS 1024
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <exception>
#include <deque>
#include <cstring>
#include <vector>
#include <list>
#include <algorithm>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <csignal>
#include "Client.hpp"

class Client;
class Serv;

class ACmd {
    protected:
		std::string _ACmdRef;
    public:
        ACmd( std::string& ACmd );
        virtual ~ACmd( void );
        virtual void executeCmd( Client& client ) = 0;
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

#endif //ACmd_hpp