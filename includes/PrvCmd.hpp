/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrvCmd.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 19:07:48 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/31 18:52:47 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PrvCmd_hpp
#define PrvCmd_hpp
#include "ACmd.hpp"

class Serv;
class Client;

class PrvCmd : public ACmd {
    private:
		std::vector<std::string> _tokens;
        Serv& _serv;
        PrvCmd( void );
        Poker* _poker;
    public:
        PrvCmd( std::vector<std::string> tokens, Serv& serv );
        ~PrvCmd( void );
        void executeCmd( Client& client );
        class FormatException : public std::exception
        {
            public:
                virtual const char* what() const throw();
        };
};

#endif //PrvCmd_hpp
