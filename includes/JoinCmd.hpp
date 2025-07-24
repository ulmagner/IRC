/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JoinCmd.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 10:16:48 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/24 11:40:27 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef JoinCmd_hpp
#define JoinCmd_hpp
#define ERR_NEEDMOREPARAMS " :Not enough parameters\n\r"
#define ERR_NOSUCHCHANNEL " :No such channel\n\r"
#define ERR_BADCHANNELKEY " :Cannot join channel\n\r"
#define ERR_BANNEDFROMCHAN " :Cannot join channel\n\r"
#define ERR_CHANNELISFULL " :Cannot join channel\n\r"
#define ERR_INVITEONLYCHAN " :Cannot join channel\n\r"
#define ERR_BADCHANMASK " :Bad Channel Mask\n\r"
#define ERR_BANNEDFROMCHAN " :Cannot join channel\n\r"

#include "ACmd.hpp"

class Serv;
class Client;

class JoinCmd : public ACmd{
    private:
		std::vector<std::string> _tokens;
        Serv& _serv;
        JoinCmd( void );
    public:
        JoinCmd( std::vector<std::string> tokens, Serv& serv );
        ~JoinCmd( void );
        void executeCmd( Client& client );
        void sendToClient( Client& client, const std::string& code, const std::string& msg);
        class FormatException : public std::exception
        {
            public:
                virtual const char* what() const throw();
        };
};

#endif //JoinCmd_hpp
