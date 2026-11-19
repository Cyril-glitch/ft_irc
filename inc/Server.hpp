/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cycolonn <cycolonn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/17 17:02:59 by cycolonn          #+#    #+#             */
/*   Updated: 2026/11/19 20:28:08 by cycolonn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "Client.hpp"
#include "Channel.hpp"
#include "CommandHandler.hpp"

#include <poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdexcept>
#include <cstring>
#include <cstdlib>

#include <vector>
#include <map>
#include <unistd.h>

class Server
{
    public:

        Server();
        Server(int port, const std::string& password);
        ~Server();


        void init();
        void run();

        std::string getPassword() const;
        std::string getClientByFd(int fd);
        std::string getClientByNickName(const std::string &nickname);
        std::string getChannel(const std::string& name);

        void    creatChannel(const std::string& name, Client* creator);
        void    removeChannelIfEmpt(const std::string& name);
        void    disconnectClient(int fd);

        typedef typename std::map<int, Client*>::iterator client_iterator;
        typedef typename std::map<std::string, Channel*>::iterator channel_iterator;
 
    private:

        int _port;
        int _serverFd;
        std::string _password;
        
        std::vector<struct pollfd> _pollFds;
        std::map<int, Client*> _clients;
        std::map<std::string, Channel*> _channels;
        CommandHandler _cmdHandler;

        Server(const Server &src);
        Server& operator=(const Server &rhs);

        void acceptNewClient();
        void handleClientRead(int fd);
        void handleClientWrite(int fd);

        void closeClientsMap();
        void closeChannelsMap();
        void closeFds();

        void createSocket();
        void setSocket();
        void bindSocket();
};

#endif