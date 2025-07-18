/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PassCmd.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 15:29:46 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/18 16:13:51 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PassCmd_hpp
#define PassCmd_hpp
#include "ACmd.hpp"

class ACmd;

class PassCmd : public ACmd {
    private:
		std::string _PassCmdRef;
        // std::vector<std::string&> _PassCmd;
		// std::vector<void(*)()> _PassCmds;
        PassCmd( void );
    public:
        PassCmd( std::vector<std::string> tokens );
        ~PassCmd( void );
        void executeCmd() const;
};

#endif //PassCmd_hpp
