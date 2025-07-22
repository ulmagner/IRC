/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserCmd.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 15:29:46 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/22 16:45:27 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UserCmd_hpp
#define UserCmd_hpp
#include "ACmd.hpp"

class Serv;
class Client;

class UserCmd : public ACmd {
    private:
		std::vector<std::string> _tokens;
        Serv& _serv;
        UserCmd( void );
    public:
        UserCmd( std::vector<std::string> tokens, Serv& serv );
        ~UserCmd( void );
        void executeCmd( Client& client );
        class FormatException : public std::exception
        {
            public:
                virtual const char* what() const throw();
        };
};

#endif //UserCmd_hpp
