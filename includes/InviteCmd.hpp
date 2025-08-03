/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InviteCmd.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 17:20:56 by ulmagner          #+#    #+#             */
/*   Updated: 2025/08/02 17:54:01 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef InviteCmd_hpp
#define InviteCmd_hpp
#include "ACmd.hpp"

class InviteCmd : public ACmd{
    private:
		std::vector<std::string> _tokens;
        Serv& _serv;
        InviteCmd( void );
    public:
        InviteCmd( std::vector<std::string> tokens, Serv& serv );
        ~InviteCmd( void );
        void executeCmd( Client& client );
        class FormatException : public std::exception
        {
            public:
                virtual const char* what() const throw();
        };
};

#endif //InviteCmd_hpp
