/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cycolonn <cycolonn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/17 17:02:59 by cycolonn          #+#    #+#             */
/*   Updated: 2026/11/17 20:26:32 by cycolonn         ###   ########.fr       */
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

        Channel();
        Channel(const Channel &src);
        ~Channel();

        Channel& operator=(const Channel &rhs);

    private:

    std::string _name;
    std::string _topic;

    std::map<int, Client*> _members;
    std::map<int, Client*> _operators;
    std::map<int, Client*> _guests;

};

#endif