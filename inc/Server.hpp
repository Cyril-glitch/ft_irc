/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cycolonn <cycolonn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/17 17:02:59 by cycolonn          #+#    #+#             */
/*   Updated: 2026/11/26 16:15:26 by cycolonn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "Client.hpp"
#include "Channel.hpp"
#include "CommandHandler.hpp"
#include "colors.hpp"

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
#include <iostream>


#include <errno.h>

class Server
{
    public:

        Server();
        Server(int port, const std::string& password);
        ~Server();


        void init();
        void run();

        typedef typename std::map<int, Client*>::iterator client_iterator;
        typedef typename std::vector<struct pollfd>::iterator pollfd_iterator;
        typedef typename std::map<std::string, Channel*>::iterator channel_iterator;
 
        std::string getClientPassword(Client &client) const;
        Client* getClientByFd(int fd);
        Client* getClientByNickName(const std::string &nickname);
        //Channel* getChannel(const std::string& name);

        //void    createChannel(const std::string& name, Client* creator);
        //void    removeEmptyChan(const std::string& name);
        //void    disconnectClient(int fd);

        private:

        int _port;
        int _serverFd;
        std::string _password;
        
        std::vector<struct pollfd> _pollFds;
        std::map<int, Client*> _clients;
        std::map<std::string, Channel*> _channels;
        //CommandHandler _cmdHandler;

        Server(const Server &src);
        Server& operator=(const Server &rhs);

        void acceptNewClient();
        int receiveFromClient(int fd, Client *client);
        int sendToClient(int fd, Client *client);
        
        bool isConnectedClient(int fd);
        void disconnectClient(int fd, Client *client);

        /*
        void closeClientsMap();
        void closeChannelsMap();
        void closeFds();
        */

        void createSocket();
        void setSocket();
        void bindSocket();
        void addPollFds(int fd);

        void setPollEvents();
};

#endif