/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Serv.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 11:55:09 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/15 12:17:17 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef Serv_hpp
#define Serv_hpp
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <exception>
#include <deque>
#include <list>
#include <algorithm>
#include <ctime>

class Serv {
    private:
        int _port;
        std::string _pass;
        int isValidPort( const std::string& port ) const;
        void isValidPass( const std::string& pass ) const;
    public:
        Serv( char **arg );
        ~Serv( void );
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