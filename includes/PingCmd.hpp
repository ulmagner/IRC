/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PingCmd.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 19:21:15 by ulmagner          #+#    #+#             */
/*   Updated: 2025/08/03 19:31:24 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PingCmd_hpp
#define PingCmd_hpp
#include "ACmd.hpp"

class PingCmd : public ACmd {
    private:
		std::vector<std::string> _tokens;
        Serv& _serv;
        PingCmd( void );
    public:
        PingCmd( std::vector<std::string> tokens, Serv& serv );
        ~PingCmd( void );
        void executeCmd( Client& client );
        class FormatException : public std::exception
        {
            public:
                virtual const char* what() const throw();
        };
};

#endif //PingCmd_hpp
