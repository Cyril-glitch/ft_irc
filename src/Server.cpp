/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cycolonn <cycolonn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/19 16:09:50 by cycolonn          #+#    #+#             */
/*   Updated: 2026/11/24 20:56:00 by cycolonn         ###   ########.fr       */
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
    /*
        AF_INET = IPV4
        SOCK_STREAM = Socket de type TCP
        IPPROTO_TCP = Protocole de type TCP
    */
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

    /*Creer un struct pour recuperer l'adresse du server*/
    sockaddr_in serverAddr;

    /*On defini le type d'adresse du server*/
    std::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;                        //bind IPV4
    serverAddr.sin_addr.s_addr = INADDR_ANY;                //bind with any host IP
    serverAddr.sin_port = htons(_port);                     //bind port

    /*On le lie a son socket*/
    if (!bind(_serverFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)))
        throw(std::runtime_error("Error: bind failed."));
}

void Server::init()
{
    createSocket();
    setSocket();
    bindSocket();
}

void Server::setPollEvents()
{
    /*active ou desactive la surveillance de l'evenement POLLOUT*/
    for (size_t i = 1; i < _pollFds.size(); ++i)
    {
        int fd = _pollFds[i].fd;
        if (_clients.count(fd) && !_clients[fd]->getSendBuffer().empty())
            _pollFds[i].events |= POLLOUT;
        else
            _pollFds[i].events &= ~POLLOUT;
    }
}

void Server::run()
{
    size_t nfd = _pollFds.size();

    /*active ou desactive la surveillance de l'evenement POLLOUT*/
    setPollEvents();

    /*Programmme en attente d'evenement sur son socket*/
    poll(&_pollFds[0], _pollFds.size(), -1);

    /*Gere l'arrivee d'un nouveaux client*/
    if (_pollFds[0].revents & POLLIN)
    {
        acceptNewClient();
        nfd++;
    }
    
    /*Reception ou envoie des donnees*/
    for (size_t i = 0; i < nfd; i++)
    {
        if (_pollFds[i].revents & POLLIN)
            receiveFromClient(_pollFds[i].fd, _clients[_pollFds[i].fd]);
        else if (_pollFds[i].revents & POLLOUT)
            SendToClient();
    }
}


void Server::acceptNewClient()
{
    /*Creer une structure pour recuperer les infos sur l'adresse du client*/
    struct sockaddr_in clientAddr;                  
    std::memset(&clientAddr, 0, sizeof(clientAddr));
    socklen_t addrLen = sizeof(clientAddr);

    /*Recupere l'adresse du client*/
    int clientFd = accept(_serverFd, (struct sockaddr *)&clientAddr, &addrLen);
    if (clientFd < 0) return;

    /*Set le Client en mode non bloquant*/
    fcntl(clientFd, F_SETFL, O_NONBLOCK);

    /*Ajoute le nouveaux client dans la map(fd, client)*/
    _clients[clientFd] = new Client(clientFd, inet_ntoa(clientAddr.sin_addr));
}

void Server::receiveFromClient(int fd, Client *client)
{
    errno = 0;

    char buf[1024];
    std::memset(&buf, 0, sizeof(buf));
    
    /*recurpere la taille du message recu*/
    ssize_t bytesRead = recv(fd, buf, sizeof(buf) -1, 0);

    /*Check les deconnection ou les erreur de reseau*/
    if (bytesRead <= 0 && (errno != EAGAIN || errno != EWOULDBLOCK))
        disonnectClient(fd);

    /*stock les donnees recu*/
    client->buf_append(buf);

    /*On extrait et execute chaque lignes*/
    while (client->buf_complete())
    {
        std::string line = client->getNextCommand();
        _cmdHandler.execute(*client , line);
    }
}


void SendToClient(int fd,Client *client)
{
    /*recuper le buffer de sorti du client*/
    std::string out = client->getSendBuffer();
    
    if (out.empty()) return ;

    /*envoie des donnees*/
    ssize_t bytesWrite = send(fd, out.c_str(), out.size(), 0);

    /*Check les deconnection ou les erreur de reseau*/
    if (bytesWrite <= 0 && (errno != EAGAIN || errno != EWOULDBLOCK))
    {
        disonnectClient(fd);
        return ;
    }

    /*efface la partie du buffer qui a ete envoye*/
    client->clearSentBytes(bytesWrite);
}

void disconnectClient(int fd)
{

}

std::string Server::getClientPassword(Client &client) const
{
    return client.get_password();
}

Client* Server::getClientByFd(int fd)
{
    client_iterator it = _clients.find(fd);
    if (it != _clients.end())
        return it->second;
    return NULL;
}

Client* Server::getClientByNickName(const std::string &nickname)
{
    client_iterator it = _clients.begin();
    while (it != _clients.end())
    {
        if (it->second->get_nickname() == nickname)
            return it->second;
    }
    return NULL;
}

Channel* Server::getChannel(const std::string &name)
{
    channel_iterator it = _channels.begin();
    while (it != _channels.end())
    {
        if (it->second->getName() == name)
            return it->second;
    }
    return NULL;
}

void creatChannel(const std::string &name, Client *creator);
void removeChannelIfEmpt(const std::string &name);