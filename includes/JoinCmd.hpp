/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JoinCmd.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 10:16:48 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/22 10:17:31 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef JoinCmd_hpp
#define JoinCmd_hpp
#include "ACmd.hpp"

class Serv;
class Client;

class JoinCmd : public ACmd {
    private:
		std::string _JoinCmdRef;
        Serv& _serv;
        JoinCmd( void );
    public:
        JoinCmd( std::vector<std::string> tokens, Serv& serv );
        ~JoinCmd( void );
        void executeCmd( Client& client ) const;
        class FormatException : public std::exception
        {
            public:
                virtual const char* what() const throw();
        };
};

#endif //JoinCmd_hpp
