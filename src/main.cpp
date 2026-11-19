/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cycolonn <cycolonn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/18 19:04:44 by cycolonn          #+#    #+#             */
/*   Updated: 2026/11/19 19:41:28 by cycolonn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "../inc/colors.hpp"
#include "../inc/Server.hpp"
#include "../inc/mainUtils.hpp"

volatile sig_atomic_t g_stop = 0;

int main(int ac, char **av)
{
    if (ac != 3)
        return std::cerr << BL_RED << "Error: Wrong Argument.\n" << RESET, 0;  

    struct sigaction sa;
    signalInit(&sa);

    try
    {
        Server server(atoi(av[1]), av[2]);
        server.init();
        while(!g_stop)
            server.run();
    }
    catch(std::exception &e)
    {
        std::cerr << BL_RED << e.what() << RESET << "\n";
    }
}