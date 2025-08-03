/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KickCmd.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 14:30:07 by ulmagner          #+#    #+#             */
/*   Updated: 2025/08/02 17:54:14 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KickCmd_hpp
#define KickCmd_hpp
#include "ACmd.hpp"

class KickCmd : public ACmd{
    private:
		std::vector<std::string> _tokens;
        Serv& _serv;
        KickCmd( void );
    public:
        KickCmd( std::vector<std::string> tokens, Serv& serv );
        ~KickCmd( void );
        void executeCmd( Client& client );
        class FormatException : public std::exception
        {
            public:
                virtual const char* what() const throw();
        };
};

#endif //KickCmd_hpp
