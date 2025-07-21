/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 11:03:54 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/21 14:36:33 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Client_hpp
#define Client_hpp
#include <string>
#include <exception>

class Client {
    private:
        int _fd;
        std::string _user;
        std::string _nick;
        std::string _host;
        std::string _real;
        std::string _pass;
		bool isAuth;
    public:
        Client( int fd );
        ~Client( void );
		int getFd( void ) const;
		void setPath( const std::string& attPath );
		const std::string& getPass( void ) const;
		bool CheckAuth( void );
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