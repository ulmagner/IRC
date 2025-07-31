/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrvCmd.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 19:07:48 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/30 19:08:01 by ulmagner         ###   ########.fr       */
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
