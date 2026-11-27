/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cycolonn <cycolonn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/27 13:03:48 by cycolonn          #+#    #+#             */
/*   Updated: 2026/11/27 18:27:42 by cycolonn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Channel.hpp"

//Cannonic Form
Channel::Channel() : _name("") , _topic(""), _inviteOnly(false) , _topicRestricted(false), _key(""), _userLimit(0)
{
}

Channel::Channel(const std::string &name) : Channel()
{
    _name = name;
}

Channel::Channel(const Channel &src)
{
    *this = src;
}

Channel::~Channel()
{
}

Channel& Channel::operator=(const Channel &rhs)
{
    _name = rhs._name;
    _topic = rhs._topic;
    _members = rhs._members;
    _operators = rhs._operators;
    _guests = rhs._guests;
    _inviteOnly = rhs._inviteOnly;
    _key = rhs._key;
    _userLimit = rhs._userLimit;
}

//Get member statut
bool Channel::isMember(int fd) const
{    
    return _members.find(fd) != _members.end();
}

bool Channel::isOperator(int fd) const
{
    return _operators.find(fd) != _operators.end();
}

bool Channel::isInvited(int fd) const
{
    return _guests.find(fd) != _guests.end();
}

//Add clients
void Channel::addMember(int fd, Client *client)
{
    _members[fd] = client;
}

void Channel::removeMember(int fd)
{
    _members.erase(fd);
    
}

void Channel::addInvited(int fd, Client *client)
{
   _guests[fd] = client;
}

void Channel::removeInvited(int fd)
{
    _guests.erase(fd);
}

void Channel::addOperator(int fd, Client *client)
{
    _operators[fd] = client;
}

void Channel::removeOperator(int fd)
{
    _operators.erase(fd);
}

void Channel::removeUser(int fd, Client *client)
{
    if (isMember(fd)) removeMember(fd);
    if (isInvited(fd)) removeInvited(fd);
    if (isOperator(fd)) removeOperator(fd);
}

//Getters
std::string Channel::getName() const
{
    return _name;
}

std::string Channel::getTopic() const
{
    return _topic;
}

std::string Channel::getKey() const
{
    return _key;
}

std::string Channel::getModeString() const
{
    std::string mode = "+"; 

    if (_inviteOnly) mode += "i";
    if (_topicRestricted) mode += "t";
    if (!_key.empty()) mode += "k";
    if (_userLimit) mode += "l"; 
    if (mode.size() > 2) return "";
    return mode;
}

size_t Channel::getUserLimit() const
{
    return _userLimit;
}
size_t Channel::getClientCount() const
{
    return  _members.size();
}

bool Channel::isInviteOnly() const
{
    return (_inviteOnly);
}

bool Channel::isTopicRestricted() const
{
    return _topicRestricted;
}

bool Channel::isEmpty() const
{
    return (_members.size() == 0);
}

//Setters
void Channel::setTopic(std::string topic)
{
    _topic = topic; 
}

void Channel::setTopicRestricted(bool state)
{
    _topicRestricted = state;
}

void Channel::setKey(const std::string key)
{
    _key = key;
}

void Channel::setUserLimit(size_t limit)
{
    _userLimit = limit;
}

//Displays
void Channel::broadcast(const std::string &message, int excludeFd = -1)
{
    client_iterator it = _members.begin();

    while (it != _members.end())
    {
        if (it->first != excludeFd)
            it->second->addToSendBuffer(message);
        it++;
    }
}