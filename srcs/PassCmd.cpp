/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PassCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 16:05:31 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/18 16:10:55 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PassCmd.hpp"

PassCmd::PassCmd( std::vector<std::string> tokens ) : ACmd(tokens[0]), _PassCmdRef("DEFAULT") {}

PassCmd::~PassCmd( void ) {}

void PassCmd::executeCmd( void ) const {
	
}
