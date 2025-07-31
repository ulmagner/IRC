/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 11:03:54 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/31 12:06:43 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Client_hpp
#define Client_hpp
#include <string>
#include <exception>
#include <iostream>
#include <vector>

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
    public:
        Client( int fd );
        ~Client( void );
		int getFd( void ) const;
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
		bool checkAuth( void );
        void addMode( const std::string& mode );
        void removeMode( const std::string& mode );
        std::string getMode( void );
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