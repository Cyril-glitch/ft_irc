/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cycolonn <cycolonn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/19 16:09:50 by cycolonn          #+#    #+#             */
/*   Updated: 2026/11/19 20:30:11 by cycolonn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

Server::Server() : _port(6667) , _serverFd(-1)
{
}

Server::Server(int port, const std::string &password) : _port(port), _serverFd(-1), _password(password)
{
}

Server::Server(const Server &src)
{
    (void)src;
}

void Server::closeClientsMap()
{
    client_iterator it = _clients.begin();

    while (it != _clients.end())
    {
        close(it->first);
        delete(it->second);
    }
    _clients.clear();
}

void Server::closeChannelsMap()
{
    channel_iterator it = _channels.begin();

    while (it != _channels.end())
        delete (it->second);
    _channels.clear();
}

void Server::closeFds()
{
    if (_serverFd >= 0)
    {
        close(_serverFd);
        _serverFd = -1;
    }
    _pollFds.clear();
}

Server::~Server()
{ 
    closeClientsMap();
    closeChannelsMap();
    closeFds();
}

Server& Server::operator=(const Server &rhs)
{
    (void)rhs;
    return *this;
}

/*-----------------------------------------------------------------------------------------------------------*/

void Server::createSocket()
{
    _serverFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (_serverFd < 0)
        throw(std::runtime_error("Error: failed to create socket."));
}

void Server::setSocket()
{
    int opt = 1;

    /*No time wait between server connections*/
    if (!setsockopt(_serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
        throw(std::runtime_error("Error: setsockopt failed."));

    /*Enable nonblock mode*/ 
    if (!fcntl(_serverFd, F_SETFL, O_NONBLOCK))
        throw(std::runtime_error("Error: fcntl failed."));
}

void Server::bindSocket()
{
    sockaddr_in serverAddr;

    std::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;                        //bind IPV4
    serverAddr.sin_addr.s_addr = INADDR_ANY;                //bind with any host IP
    serverAddr.sin_port = htons(_port);                     //bind port

    if (!bind(_serverFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)))
        throw(std::runtime_error("Error: bind failed."));
}

void Server::init()
{
    createSocket();
    setSocket();
    bindSocket();
}
    
void Server::run()
{
    size_t nfd = _pollFds.size();
    poll(&_pollFds[0], _pollFds.size(), -1);            //wait for an event
    if (_pollFds[0].revents & POLLIN)                   //check new client
    {
        acceptNewClient();
        nfd++;
    }
    for (size_t i = 0; i < nfd; i++)
    {
        if (_pollFds[i].revents & POLLIN)
            handleClientRead();
        else if (_pollFds[i].revents & POLLOUT)
            handleClientWrite();
    }
}


void Server::acceptNewClient()
{
    struct sockaddr_in clientAddr;
    std::memset(&clientAddr, 0, sizeof(clientAddr));
    socklen_t addrLen = sizeof(clientAddr);

    int clientFd = accept(_serverFd, (struct sockaddr *)&clientAddr, &addrLen);
    if (clientFd < 0) return;

    fcntl(clientFd, F_SETFL, O_NONBLOCK);

    new Client(clientFd, inet_ntoa(clientAddr.sin_addr));
}

/*
void handleClientRead(int fd);
void handleClientWrite(int fd);

std::string getPassword() const;
std::string getClientByFd(int fd);
std::string getClientByNickName(const std::string &nickname);
std::string getChannel(const std::string &name);

void creatChannel(const std::string &name, Client *creator);
void removeChannelIfEmpt(const std::string &name);
void disconnectClient(int fd);
*/