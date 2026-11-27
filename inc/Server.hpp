/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbenaroc <gbenaroc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/17 17:02:59 by cycolonn          #+#    #+#             */
/*   Updated: 2026/11/27 15:02:29 by gbenaroc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "Client.hpp"
#include "Channel.hpp"
#include "CommandHandler.hpp"
#include <poll.h>
#include <vector>
#include <map>


class Server
{
	public:

		Server();
		Server(int port, const std::string& password);
		Server(const Server &src);
		~Server();

		Server& operator=(const Server &rhs);

		void init();
		void run();

		std::string getPassword() const;
		std::string getClientByFd(int fd);
		std::string getClientByNickName(const std::string &nickname);
		Channel* getChannel(const std::string &name);

		void	createChannel(const std::string& name, Client* creator);
		void	removeChannelIfEmpty(const std::string& name);
		void	disconnectClient(int fd);
 
	private:

		int _port;
		int _serverFd;
		std::string _password;

		std::vector<struct pollfd> _pollFds;

		std::map<int, Client*> _Clients;
		std::map<std::string, Channel*> _channels;

		CommandHandler _cmdHandler;

		static bool _signalReceived;

		void acceptNewClient();
		void handleClientRead(int fd);
		void handleClientWrite(int fd);

};

#endif