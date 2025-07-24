/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NickCmd.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 15:29:46 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/24 17:43:49 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NickCmd_hpp
#define NickCmd_hpp
// #define ERR_NONICKNAMEGIVEN " :No nickname given\n\r"
// #define ERR_NICKNAMEINUSE " :Nickname is already in use\n\r"
#include "ACmd.hpp"

class Serv;
class Client;

class NickCmd : public ACmd {
    private:
		std::vector<std::string> _tokens;
        Serv& _serv;
        NickCmd( void );
    public:
        NickCmd( std::vector<std::string> tokens, Serv& serv );
        ~NickCmd( void );
        void executeCmd( Client& client );
        void sendToClient( Client& client, const std::string& code, const std::string& msg);
        class FormatException : public std::exception
        {
            public:
                virtual const char* what() const throw();
        };
};

#endif //NickCmd_hpp
