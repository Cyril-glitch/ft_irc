/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cycolonn <cycolonn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/18 19:04:44 by cycolonn          #+#    #+#             */
/*   Updated: 2026/11/19 13:23:07 by cycolonn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "../inc/colors.hpp"
#include "../inc/Server.hpp"
#include "../inc/mainUtils.hpp"

int main(int ac, char **av)
{
    if (ac != 3)
        return std::cerr << BL_RED << "Error: Wrong Argument.\n" << RESET, 0;  

    struct sigaction sa;
    signalInit(&sa);
}