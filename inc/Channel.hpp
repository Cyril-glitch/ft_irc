/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cycolonn <cycolonn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/17 17:02:59 by cycolonn          #+#    #+#             */
/*   Updated: 2026/11/18 15:17:46 by cycolonn         ###   ########.fr       */
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
        bool isMember(int, Client *client) const;
        bool isOperator(int, Client *client) const;
        bool isInvited(int, Client *client) const;

        /*Add clients*/
        void addMember(Client *client);
        void removeMember(Client *client);

        void addInvited(int fd, Client *client);
        void removeInvited(int fd, Client *client);

        void addOperator(Client *client);
        void removeOperator(Client *client);

        /*Getters*/
        std::string getName() const;
        std::string getTopic() const;
        std::string getKey() const;
        std::string getModeString() const;
        size_t  getUserLimit() const;
        size_t getClientCount() const;        

        bool isInviteOnly() const;
        bool isTopicRestricted() const;

        /*Setters*/
        void setTopic();
        void setTopicRestricted(bool state);
        void setKey(const std::string key);
        void setUserLimit(size_t limit);

        /*Displays*/
        void broadcast(const std::string&message, int excludeFd = -1);

    private:

        /*Channel info*/
        std::string _name;
        std::string _topic;

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