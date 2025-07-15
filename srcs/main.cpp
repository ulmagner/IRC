/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 10:41:18 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/15 12:20:04 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Serv.hpp"

int main( int ac, char **av ) {
    if (ac != 3)
        return (1);
    try {
        Serv S(av);
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return (0);    
}
