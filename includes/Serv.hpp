/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Serv.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 11:55:09 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/30 13:55:32 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Serv_hpp
#define Serv_hpp
#define ERR_NEEDMOREPARAMS " :Not enough parameters\r\n"
#define ERR_NOSUCHCHANNEL " :No such channel\r\n"
#define ERR_BADCHANNELKEY " :Cannot Kick channel\r\n"
#define ERR_BANNEDFROMCHAN " :Cannot Kick channel\r\n"
#define ERR_CHANNELISFULLK " :Cannot Kick channel\r\n"
#define ERR_USERNOTINCHANNEL " :They aren't on that channel\r\n"
#define ERR_BADCHANMASK " :Bad Channel Mask\r\n"
#define ERR_CHANOPRIVSNEEDED " :You're not channel operator\r\n"
#define ERR_NICKNAMEINUSE " :Nickname is already in use\r\n"
#define ERR_ALREADYREGISTERED " :You may not reregister\r\n"
#define ERR_PASSWDMISMATCH " :Password incorrect\r\n"
#define ERR_NONICKNAMEGIVEN " :No nickname given\r\n"
#define ERR_NOTONCHANNEL " :You're not on that channel\r\n"
#define ERR_NOSUCHNICK " :No such nick/channel\r\n"
#define ERR_USERONCHANNEL " :is already on channel\r\n"
#define ERR_INVITEONLYCHAN " :Cannot join channel (+i)\r\n"
#define ERR_UNKNOWNCOMMAND " :Unknown command\r\n"
#define ERR_CANNOTSENDTOCHAN " :Cannot send to channel\r\n"
#define ERR_TOOMANYCHANNELS " :You have joined too many channels\r\n"
#define ERR_CHANNELISFULL " :Cannot join channel (+l)\r\n"
#define RPL_CHANNELMODEIS "\r\n"
#define RPL_NOTOPIC " :No topic is set\r\n"
#define RPL_ENDOFNAMES " :End of /NAMES list.\r\n"
#include <vector>
#include <map>
#include <exception>
#include "ACmd.hpp"
#include "Client.hpp"
#include "Channel.hpp"

class Serv {
    friend class PassCmd;
    friend class NickCmd;
    friend class UserCmd;
    friend class JoinCmd;
    friend class KickCmd;
    friend class InviteCmd;
    friend class TopicCmd;
    friend class PartCmd;
    private:
        std::string _name;
        int _port;
        std::string _pass;
        int _socketfd;
        int _epollfd;
        std::map<int, Client> _connections;
        std::vector<Channel> _channels;
        int isValidPort( const std::string& port ) const;
        void isValidPass( const std::string& pass ) const;
        void createTcpServerSocket( void );
    public:
        Serv( char **arg );
        ~Serv( void );
        void shutdown( void );
        void run( void );
        const std::string& getPass( void ) const;
        const std::map<int, Client>& getConnections() const;
        std::vector<Channel>& getChannels();
        Channel* getChannelByName( std::string& name );
        ACmd* pass( std::vector<std::string> tokens );
        ACmd* nick( std::vector<std::string> tokens );
        ACmd* user( std::vector<std::string> tokens );
        ACmd* join( std::vector<std::string> tokens );
        ACmd* kick( std::vector<std::string> tokens );
        ACmd* invite( std::vector<std::string> tokens );
        ACmd* topic( std::vector<std::string> tokens );
        ACmd* part( std::vector<std::string> tokens );
        ACmd* getCmd( const char* buffer, Client& client );
        Client& getClientByFd( int fd );
        Client* getClientByName( const std::string& name );
        void sendToClient( Client& client, const std::string& code, const std::string& msg );
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
        class AlreadyAuthenticateException : public std::exception
        {
            public:
                virtual const char* what() const throw();
        };
        class NotAuthYetException : public std::exception
        {
            public:
                virtual const char* what() const throw();
        };
        class NotAnOperatorException : public std::exception
        {
            public:
                virtual const char* what() const throw();
        };
};

std::vector<std::string> split( const std::string& s, char delimiter );
void sendToChannelClient( Channel* channel, std::string& msg );

extern Serv* g_serv;

#endif //Serv_hpp