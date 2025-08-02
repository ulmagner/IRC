/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.tpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 03:03:40 by ulmagner          #+#    #+#             */
/*   Updated: 2025/08/02 03:11:44 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

template<typename T>
Client* Channel::getClientByName( const T& t ) {
    std::map<int, std::pair<Client*, int> >::const_iterator it = this->_clientConnected.begin();
    for (;it != this->_clientConnected.end(); ++it) {
        if (t == it->second.first->getNick() || t == it->second.first->getFd()) {
            return (it->second.first);
        }
    }
    return (NULL);
}
