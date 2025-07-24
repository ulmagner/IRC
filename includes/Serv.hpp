/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Serv.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 11:55:09 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/24 18:45:23 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Serv_hpp
#define Serv_hpp
#define ERR_NEEDMOREPARAMS " :Not enough parameters\n\r"
#define ERR_NOSUCHCHANNEL " :No such channel\n\r"
#define ERR_BADCHANNELKEY " :Cannot Kick channel\n\r"
#define ERR_BANNEDFROMCHAN " :Cannot Kick channel\n\r"
#define ERR_CHANNELISFULL " :Cannot Kick channel\n\r"
#define ERR_USERNOTINCHANNEL " :They aren't on that channel\n\r"
#define ERR_BADCHANMASK " :Bad Channel Mask\n\r"
#define ERR_CHANOPRIVSNEEDED " :You're not channel operator\n\r"
#define ERR_NICKNAMEINUSE " :Nickname is already in use\n\r"
#define ERR_ALREADYREGISTERED " :You may not reregister\n\r"
#define ERR_PASSWDMISMATCH " :Password incorrect\n\r"
#define ERR_NONICKNAMEGIVEN " :No nickname given\n\r"
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
        ACmd* pass( std::vector<std::string> tokens );
        ACmd* nick( std::vector<std::string> tokens );
        ACmd* user( std::vector<std::string> tokens );
        ACmd* join( std::vector<std::string> tokens );
        ACmd* kick( std::vector<std::string> tokens );
        ACmd* getCmd( const char* buffer, Client& client );
        Client& getClientByFd( int fd );
        Client& getClientByName( const std::string& name );
        void sendToClient( Client& client, const std::string& code, const std::string& msg);
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

extern Serv* g_serv;
// void sendToClient( int clientFd, const std::string& message );

#endif //Serv_hpp