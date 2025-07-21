/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PassCmd.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 15:29:46 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/21 13:52:58 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PassCmd_hpp
#define PassCmd_hpp
#include "ACmd.hpp"

class Serv;
class Client;

class PassCmd : public ACmd {
    private:
		std::string _PassCmdRef;
        Serv& _serv;
        PassCmd( void );
    public:
        PassCmd( std::vector<std::string> tokens, Serv& serv );
        ~PassCmd( void );
        void executeCmd( Client& client ) const;
        class FormatException : public std::exception
        {
            public:
                virtual const char* what() const throw();
        };
};

#endif //PassCmd_hpp
