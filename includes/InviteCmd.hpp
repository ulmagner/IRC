/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InviteCmd.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 17:20:56 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/24 17:43:09 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef InviteCmd_hpp
#define InviteCmd_hpp
// #define ERR_NEEDMOREPARAMS " :Not enough parameters\n\r"
// #define ERR_NOSUCHCHANNEL " :No such channel\n\r"
// #define ERR_BADCHANNELKEY " :Cannot Invite channel\n\r"
// #define ERR_BANNEDFROMCHAN " :Cannot Invite channel\n\r"
// #define ERR_CHANNELISFULL " :Cannot Invite channel\n\r"
// #define ERR_USERNOTINCHANNEL " :They aren't on that channel\n\r"
// #define ERR_BADCHANMASK " :Bad Channel Mask\n\r"
// #define ERR_CHANOPRIVSNEEDED " :You're not channel operator\n\r"
#include "ACmd.hpp"

class Serv;
class Client;

class InviteCmd : public ACmd{
    private:
		std::vector<std::string> _tokens;
        Serv& _serv;
        InviteCmd( void );
    public:
        InviteCmd( std::vector<std::string> tokens, Serv& serv );
        ~InviteCmd( void );
        void executeCmd( Client& client );
        void sendToClient( Client& client, const std::string& code, const std::string& msg);
        class FormatException : public std::exception
        {
            public:
                virtual const char* what() const throw();
        };
};

#endif //InviteCmd_hpp
