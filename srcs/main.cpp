/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 10:41:18 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/15 14:23:01 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Serv.hpp"

int main( int ac, char **av ) {
    if (ac != 3)
        return (1);
    try {
        Serv S(av);
        S.run();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return (0);    
}
