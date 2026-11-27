/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cycolonn <cycolonn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/17 17:02:59 by cycolonn          #+#    #+#             */
/*   Updated: 2026/11/27 18:12:02 by cycolonn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Client.hpp"
#include <string>
#include <map>

class Channel
{
    public:

        /*Cannonic Form*/
        Channel();
        Channel(const std::string &name);
        Channel(const Channel &src);
        ~Channel();

        Channel &operator=(const Channel &rhs);

        /*Get member statut*/
        bool isMember(int fd) const;
        bool isOperator(int fd) const;
        bool isInvited(int fd) const;

        /*Add clients*/
        void addMember(int fd, Client *client);
        void removeMember(int fd);

        void addInvited(int fd, Client *client);
        void removeInvited(int fd);

        void addOperator(int fd, Client *client);
        void removeOperator(int fd);

        void removeUser(int fd, Client *client);

        /*Getters*/
        std::string getName() const;
        std::string getTopic() const;
        std::string getKey() const;
        std::string getModeString() const;
        size_t  getUserLimit() const;
        size_t getClientCount() const;        

        bool isInviteOnly() const;
        bool isTopicRestricted() const;
        bool isEmpty() const;

        /*Setters*/
        void setTopic(std::string topic);
        void setTopicRestricted(bool state);
        void setKey(const std::string key);
        void setUserLimit(size_t limit);

        /*Displays*/
        void broadcast(const std::string&message, int excludeFd = -1);

        typedef typename std::map<std::string, Channel*>::iterator channel_iterator;
        typedef typename std::map<int, Client*>::iterator client_iterator;


    private:

        /*Channel info*/
        std::string _name;
        std::string _topic;
        std::string _count;

        /*Users list*/
        std::map<int, Client *> _members;
        std::map<int, Client *> _operators;
        std::map<int, Client *> _guests;

        /*Modes*/
        bool _inviteOnly;      // mode i
        bool _topicRestricted; // mode t
        std::string _key;      // mode k
        size_t _userLimit;     // mode l
};

#endif