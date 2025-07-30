/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 10:41:18 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/30 17:58:37 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Serv.hpp"

Serv* g_serv = NULL;

void signalHandler( int signum ) {
    std::cout << "Interrupt signal (" << signum << ") received.\n";
    if (g_serv)
        g_serv->shutdown();
    exit(signum);
}

int main( int ac, char **av ) {
    if (ac != 3)
        return (1);
    try {
        Serv S(av);
        g_serv = &S;
        signal(SIGINT, signalHandler);
        S.run();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return (0);
}
