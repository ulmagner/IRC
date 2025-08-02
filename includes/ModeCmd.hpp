/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeCmd.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 11:03:46 by ulmagner          #+#    #+#             */
/*   Updated: 2025/08/02 17:54:17 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ModeCmd_hpp
#define ModeCmd_hpp
#include "ACmd.hpp"

class ModeCmd : public ACmd {
    private:
		std::vector<std::string> _tokens;
        Serv& _serv;
        ModeCmd( void );
    public:
        ModeCmd( std::vector<std::string> tokens, Serv& serv );
        ~ModeCmd( void );
        void executeCmd( Client& client );
        class FormatException : public std::exception
        {
            public:
                virtual const char* what() const throw();
        };
};

#endif //ModeCmd_hpp
