/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 11:03:54 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/22 09:50:39 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Client_hpp
#define Client_hpp
#include <string>
#include <exception>
#include <iostream>

class Client {
    private:
        int _fd;
        std::string _user;
        std::string _nick;
        std::string _host;
        std::string _real;
        std::string _pass;
		bool _isAuth;
    public:
        Client( int fd );
        ~Client( void );
		int getFd( void ) const;
		const std::string& getNick( void ) const;
		const std::string& getUser( void ) const;
		const std::string& getPass( void ) const;
		const bool& getAuth( void ) const;
		void setPath( const std::string& attPath );
		void setNick( const std::string& attNick );
		void setUser( const std::string& attUser );
		void setAuth( bool attAuth );
		bool checkAuth( void );
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