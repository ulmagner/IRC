/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PartCmd.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 18:43:06 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/29 18:43:35 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PartCmd_hpp
#define PartCmd_hpp
#include "ACmd.hpp"

class Serv;
class Client;

class PartCmd : public ACmd {
    private:
		std::vector<std::string> _tokens;
        Serv& _serv;
        PartCmd( void );
    public:
        PartCmd( std::vector<std::string> tokens, Serv& serv );
        ~PartCmd( void );
        void executeCmd( Client& client );
        class FormatException : public std::exception
        {
            public:
                virtual const char* what() const throw();
        };
};

#endif //PartCmd_hpp
