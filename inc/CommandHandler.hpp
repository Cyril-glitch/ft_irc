/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cycolonn <cycolonn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/17 17:02:59 by cycolonn          #+#    #+#             */
/*   Updated: 2026/11/18 17:32:35 by cycolonn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "Client.hpp"
#include <map>

class Server;

struct t_msg
        {
            std::string prefix;
            std::string command;
            std::string params;
        };


class CommandHandler
{
    public:

        CommandHandler();
        CommandHandler(const CommandHandler &src);
        ~CommandHandler();

        CommandHandler& operator=(const CommandHandler&rhs);

        void execute(Server& Server, Client &client, const std::string input);
        
        typedef void (CommandHandler::*CommandFunc)(Server& server, Client& client, const t_msg& msg);

    private:

        
        t_msg _msg;
        std::map<std::string, CommandFunc> _cmds;
};

#endif