/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mainUtils.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cycolonn <cycolonn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/18 20:15:13 by cycolonn          #+#    #+#             */
/*   Updated: 2026/11/19 13:16:26 by cycolonn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef MAIN_UTILS_H
#define MAIN_UTILS_H

#include "../inc/Server.hpp"
#include <string>
#include <csignal>
#include <cstring>

bool    checkArgs(char **av);
void    signalInit(struct sigaction *sa);
void signalHandler(int signum);

#endif