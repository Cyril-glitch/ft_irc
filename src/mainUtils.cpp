/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mainUtils.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cycolonn <cycolonn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/18 20:12:02 by cycolonn          #+#    #+#             */
/*   Updated: 2026/11/18 20:19:20 by cycolonn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/mainUtils.hpp"

bool    checkArgs(char **av)
{
    std::string port = av[1];
    std::string pass = av[2];

    return ((port.find_first_not_of("0123456789") || pass.empty()));
}

void    signalInit(struct sigaction *sa)
{
    std::memset(sa, 0, sizeof(sa));
    sigemptyset(&sa->sa_mask);
    sa->sa_flags = 0;

    // 1. Ignorer SIGPIPE
    sa->sa_handler = SIG_IGN;
    sigaction(SIGPIPE, sa, NULL);

    // 2. Attraper SIGINT et SIGQUIT
    sa->sa_handler = Server::signalHandler;
    sigaction(SIGINT, sa, NULL);
    sigaction(SIGQUIT, sa, NULL);
}

