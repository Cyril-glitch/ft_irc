/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cycolonn <cycolonn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/17 17:02:59 by cycolonn          #+#    #+#             */
/*   Updated: 2026/11/17 17:15:58 by cycolonn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
#define COMMAND_HPP

class Command
{
    public:

        Command();
        Command(const Command &src);
        ~Command();

        Command& operator=(const Command &rhs);

    private:



};

#endif