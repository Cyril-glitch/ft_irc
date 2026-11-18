/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mainUtils.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cycolonn <cycolonn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/18 20:15:13 by cycolonn          #+#    #+#             */
/*   Updated: 2026/11/18 20:17:02 by cycolonn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef MAIN_UTILS_H
#define MAIN_UTILS_H

#include "../inc/Server.hpp"
#include <string>
#include <csignal>
#include <cstring>

void    signalInit(struct sigaction *sa);
bool    checkArgs(char **av);

#endif