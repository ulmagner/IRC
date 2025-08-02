/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TopicCmd.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 12:06:01 by ulmagner          #+#    #+#             */
/*   Updated: 2025/08/02 17:54:39 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TopicCmd_hpp
#define TopicCmd_hpp
#include "ACmd.hpp"

class TopicCmd : public ACmd {
    private:
		std::vector<std::string> _tokens;
        Serv& _serv;
        TopicCmd( void );
    public:
        TopicCmd( std::vector<std::string> tokens, Serv& serv );
        ~TopicCmd( void );
        void executeCmd( Client& client );
        class FormatException : public std::exception
        {
            public:
                virtual const char* what() const throw();
        };
};

#endif //TopicCmd_hpp
