/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ACmd.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 14:29:26 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/18 15:04:25 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ACmd.hpp"

ACmd::ACmd( std::string& ACmdref ) : _ACmdRef(ACmdref) {}

ACmd::~ACmd( void ) {}

const char* ACmd::ErrorException::what() const throw()
{
    return ("Error");
}
