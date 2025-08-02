/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NickCmd.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 15:29:46 by ulmagner          #+#    #+#             */
/*   Updated: 2025/08/02 17:54:21 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NickCmd_hpp
#define NickCmd_hpp
#include "ACmd.hpp"

class NickCmd : public ACmd {
    private:
		std::vector<std::string> _tokens;
        Serv& _serv;
        NickCmd( void );
    public:
        NickCmd( std::vector<std::string> tokens, Serv& serv );
        ~NickCmd( void );
        void executeCmd( Client& client );
        class FormatException : public std::exception
        {
            public:
                virtual const char* what() const throw();
        };
};

#endif //NickCmd_hpp
