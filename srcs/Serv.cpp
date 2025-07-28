/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Serv.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 11:58:33 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/28 15:27:22 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Serv.hpp"
#include "PassCmd.hpp"
#include "NickCmd.hpp"
#include "UserCmd.hpp"
#include "JoinCmd.hpp"
#include "KickCmd.hpp"

Serv::Serv( char **arg ) : _name("IRC_DEF"), _socketfd(0), _epollfd(0) {
    this->_port = this->isValidPort(arg[1]);
    this->isValidPass(arg[2]);
    this->_pass = arg[2];
}

Serv::~Serv( void ) {}

void Serv::createTcpServerSocket( void ) {
    this->_socketfd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(this->_port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    bind(this->_socketfd, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

    listen(this->_socketfd, 10);
}

void Serv::shutdown( void ) {
    std::cout << this->_connections.size() << std::endl;
    std::map<int, Client>::const_iterator it = this->_connections.begin();
    for (; it != this->_connections.end(); ++it) {
        int fd = it->second.getFd();
        if (fd != this->_socketfd && fd != -1) {
            close(fd);
            fd = -1;
        }
    }
    this->_connections.clear();
    if (this->_epollfd != -1)
        close(this->_epollfd);
    if (this->_socketfd != -1)
        close(this->_socketfd);
    this->_epollfd = -1;
    this->_socketfd = -1;
}

ACmd*	Serv::pass( std::vector<std::string> tokens )
{
	return (new PassCmd(tokens, *this));
}

ACmd*	Serv::nick( std::vector<std::string> tokens )
{
	return (new NickCmd(tokens, *this));
}

ACmd*	Serv::user( std::vector<std::string> tokens )
{
	return (new UserCmd(tokens, *this));
}

ACmd*	Serv::join( std::vector<std::string> tokens )
{
	return (new JoinCmd(tokens, *this));
}

ACmd*	Serv::kick( std::vector<std::string> tokens )
{
	return (new KickCmd(tokens, *this));
}

const std::string& Serv::getPass( void ) const {
	return (this->_pass);
}

const std::map<int, Client>& Serv::getConnections( void ) const {
    return (this->_connections);
}

std::vector<Channel>& Serv::getChannels( void ){
    return (this->_channels);
}

ACmd* Serv::getCmd( const char* buffer, Client& client ) {
	std::string auth[] = {"PASS", "NICK", "USER", "JOIN", "KICK"};
    std::stringstream ss(buffer);
    std::string word;
    std::vector<std::string> tokens;

    while (ss >> word) {
        tokens.push_back(word);
    }
    if (tokens.empty()) {
        throw std::runtime_error("Empty command");
    }
	ACmd* (Serv::*cmds[])( std::vector<std::string> ) = {
		&Serv::pass,
        &Serv::nick,
        &Serv::user,
        &Serv::join,
        &Serv::kick,
	};

    if (!client.getAuth()) {
        for (int i = 0; i < 3; i++) {
            if (auth[i].compare( tokens[0] ) == 0) {
                return (this->*cmds[i])(tokens);
            }
        }
    }
    for (size_t i = 0; i < sizeof(cmds) / sizeof(cmds[0]); i++) {
        if (!client.getAuth() && i >= 3 && auth[i].compare( tokens[0] ) == 0) {
            throw Serv::NotAuthYetException();
        }
        if (client.getAuth() == true && i < 3 && auth[i].compare( tokens[0] ) == 0) {
            throw Serv::AlreadyAuthenticateException();
        }
        else if (auth[i].compare( tokens[0] ) == 0) {
            return (this->*cmds[i])(tokens);
        }
    }
    throw Serv::CmdNotFoundException();
    return (NULL);
}

Client& Serv::getClientByFd( int fd ) {
    for (std::map<int, Client>::iterator it = this->_connections.begin(); it != this->_connections.end(); ++it) {
        if (it->first == fd)
            return (it->second);
    }
    throw Serv::ErrorException();
}

Client& Serv::getClientByName( const std::string& name ) {
    for (std::map<int, Client>::iterator it = this->_connections.begin(); it != this->_connections.end(); ++it) {
        if (it->second.getUser() == name)
            return (it->second);
    }
    throw Serv::ErrorException();
}

void Serv::run( void ) {
    createTcpServerSocket();
    fcntl(this->_socketfd, F_SETFL, O_NONBLOCK);
    struct epoll_event ev, events[MAX_EVENTS];
    int nfds, clientSocket;
    this->_connections.insert(std::make_pair(this->_socketfd, Client(this->_socketfd)));
    this->_epollfd = epoll_create1(0);
    if (this->_epollfd == -1) {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }
    ev.events = EPOLLIN;
    ev.data.fd = this->_socketfd;
    if (epoll_ctl(this->_epollfd, EPOLL_CTL_ADD, this->_socketfd, &ev) == -1) {
        perror("epoll_ctl: this->_socketfd");
        exit(EXIT_FAILURE);
    }
    for (;;) {
        nfds = epoll_wait(this->_epollfd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            perror("epoll_wait");
            exit(EXIT_FAILURE);
        }
        for (int n = 0; n < nfds; ++n) {
            if (events[n].data.fd == this->_socketfd) {
                sockaddr_in clientAddr;
                socklen_t addrlen = sizeof(clientAddr);
                clientSocket = accept(events[n].data.fd, (struct sockaddr*)&clientAddr, &addrlen);
                if (clientSocket == -1) {
                    perror("accept");
                    exit(EXIT_FAILURE);
                }
                fcntl(clientSocket, F_SETFL, O_NONBLOCK);
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = clientSocket;
                this->_connections.insert(std::make_pair(clientSocket, Client(clientSocket)));
                if (epoll_ctl(this->_epollfd, EPOLL_CTL_ADD, clientSocket, &ev) == -1) {
                    perror("epoll_ctl: clientSocket");
                    // exit(EXIT_FAILURE);
                    continue ;
                }
            }
            else {
                char buffer[MAX_EVENTS] = {0};
                ssize_t bytes = recv(events[n].data.fd, buffer, sizeof(buffer), 0);
                if (!bytes) {
                    std::cout << "Server closed connection." << std::endl;
                    close(events[n].data.fd);
                    epoll_ctl(this->_epollfd, EPOLL_CTL_DEL, events[n].data.fd, NULL);
                    this->_connections.erase(events[n].data.fd);
                }
                else if (bytes < 0) {
                    std::cout << "Client " << events[n].data.fd << " disconnected." << std::endl;
                    close(events[n].data.fd);
                    epoll_ctl(this->_epollfd, EPOLL_CTL_DEL, events[n].data.fd, NULL);
                    this->_connections.erase(events[n].data.fd);
                }
                else {
                    std::string input(buffer, bytes);
                    std::vector<std::string> commands = split(input, '\r');
                    for (size_t i = 0; i < commands.size(); ++i) {
                        if (commands[i].empty())
                            continue;
                        ACmd* cmd = NULL;
                        try {
                            Client& client = this->getClientByFd(events[n].data.fd);
                            cmd = Serv::getCmd(commands[i].c_str(), client);
                            cmd->executeCmd(client);
                            if (!client.getAuth()) {
                                client.setAuth(client.checkAuth());
                                if (client.getAuth()) {
                                    this->sendToClient(client, "001", "");
                                    this->sendToClient(client, "002", "");
                                }
                            }
                        }
                        catch (const std::exception& e) {
                            std::cout << e.what() << std::endl;
                        }
                        delete cmd;
                    }
                }
            }
        }
    }
}

void Serv::sendToClient( Client& client, const std::string& code, const std::string& message ) {
	std::string fullMsg = "";
    (void) message;
	if (code == "001")
		fullMsg = ":" + this->_name + " " + code + " " + client.getUser() + " :Welcome to the " + this->_name + " Network, " + client.getNick() + " [!" + client.getUser() + "@<host>]\n\r";
	else if (code == "002")
		fullMsg = ":" + this->_name + " " + code + " " + client.getUser() + " :Your host is " + this->_name + " , running version <version>\n\r";
	send(client.getFd(), fullMsg.c_str(), fullMsg.size(), 0);
}

int Serv::isValidPort( const std::string& port ) const {
    if (port.size() > 5 || port.size() < 4)
        throw Serv::FormatException();
    for (size_t i = 0; i < port.size(); i++) {
        if (!isdigit(port[i]))
            throw Serv::FormatException();
    }
    int p = std::atoi(port.c_str());
    if (p < 1025 || p > 65535)
        throw Serv::FormatException();
    return (p);
}

void Serv::isValidPass( const std::string& pass ) const {
    if (pass.size() > 10 || pass.size() < 4)
        throw Serv::FormatException();
}

const char* Serv::FormatException::what() const throw()
{
    return ("Error");
}

const char* Serv::ErrorException::what() const throw()
{
    return ("Error");
}

const char* Serv::CmdNotFoundException::what() const throw()
{
    return ("CMD NOT FOUND.");
}

const char* Serv::AlreadyAuthenticateException::what() const throw()
{
    return ("CLIENT ALREADY AUTHENTICATE.");
}

const char* Serv::NotAuthYetException::what() const throw()
{
    return ("CLIENT NOT AUTHENTICATE YET.");
}

const char* Serv::NotAnOperatorException::what() const throw()
{
    return ("NOT AN OPERATOR.");
}
