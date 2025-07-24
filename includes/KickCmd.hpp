/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KickCmd.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 14:30:07 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/24 14:33:02 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KickCmd_hpp
#define KickCmd_hpp
// #define ERR_NEEDMOREPARAMS " :Not enough parameters\n\r"
// #define ERR_NOSUCHCHANNEL " :No such channel\n\r"
// #define ERR_BADCHANNELKEY " :Cannot Kick channel\n\r"
// #define ERR_BANNEDFROMCHAN " :Cannot Kick channel\n\r"
// #define ERR_CHANNELISFULL " :Cannot Kick channel\n\r"
// #define ERR_INVITEONLYCHAN " :Cannot Kick channel\n\r"
// #define ERR_BADCHANMASK " :Bad Channel Mask\n\r"
// #define ERR_BANNEDFROMCHAN " :Cannot Kick channel\n\r"
#include "ACmd.hpp"

class Serv;
class Client;

class KickCmd : public ACmd{
    private:
		std::vector<std::string> _tokens;
        Serv& _serv;
        KickCmd( void );
    public:
        KickCmd( std::vector<std::string> tokens, Serv& serv );
        ~KickCmd( void );
        void executeCmd( Client& client );
        void sendToClient( Client& client, const std::string& code, const std::string& msg);
        class FormatException : public std::exception
        {
            public:
                virtual const char* what() const throw();
        };
};

#endif //KickCmd_hpp
