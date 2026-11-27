/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commandhandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbenaroc <gbenaroc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/26 13:18:25 by gbenaroc          #+#    #+#             */
/*   Updated: 2026/11/27 18:30:25 by gbenaroc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/CommandHandler.hpp"
#include "../inc/Server.hpp"

static void sendError(Client &client, const std::string &code, const std::string &msg)
{
	std::string nick = client.get_nickname();
	if (nick.empty())
		nick = "*";
	client.addToSendBuffer(":ircserv " + code + " " + nick + " " + msg + "\r\n");
}

t_msg CommandHandler::parse(const std::string &input)
{
	t_msg msg;
	size_t space = input.find(' ');
	
	if (space == std::string::npos)
	{
		msg.command = input;
		msg.params = "";
	}
	else
	{
		msg.command = input.substr(0, space);
		msg.params = input.substr(space + 1);
	}
	return msg;
}
	
void CommandHandler::execute(Server& server, Client& client, const std::string &input)
{
	if (input.empty())
		return;
	t_msg msg = parse(input);
	if (!client.isRegistered() && msg.command != "PASS" && msg.command != "NICK" && msg.command != "USER")
	{
		sendError(client, "451", ":You have not registered");
		return;
	}
	bool wasRegistered = client.isRegistered();
	if (msg.command == "PASS")
		handlePass(server, client, msg);
	else if (msg.command == "NICK")
		handleNick(server, client, msg);
	else if (msg.command == "USER")
		handleUser(server, client, msg);
	else if (msg.command == "JOIN")
		handleJoin(server, client, msg);
	else if (msg.command == "PRIVMSG")
		handlePrivmsg(server, client, msg);
	else if (msg.command == "KICK")
		handleKick(server, client, msg);
	else if (msg.command == "INVITE")
		handleInvite(server, client, msg);
	else if (msg.command == "TOPIC")
		handleTopic(server, client, msg);
else if (msg.command == "MODE")
;  // TODO
else if (msg.command == "QUIT")
;  // TODO
else if (msg.command == "PING")
;  // TODO
	/*else if (msg.command == "MODE")
		handleMode(server, client, msg);
	else if (msg.command == "QUIT")
		// appeler fonction QUIT
	else if (msg.command == "PING")
		// appeler fonction PING*/
	else
		sendError(client, "421", msg.command + " :Unknown command");
	if (!wasRegistered && client.isRegistered())
		client.addToSendBuffer(":ircserv 001 " + client.get_nickname() + " :Welcome to the IRC Network\r\n");
}

void CommandHandler::handlePass(Server& server, Client& client, const t_msg &msg)
{
	if (client.isRegistered())
	{
		sendError(client, "462", ":You may not reregister");
		return;
	}
	if (msg.params == "")
	{
		sendError(client, "461", "PASS :Not enough parameters");
		return;
	}
	if (server.getPassword() != msg.params)
	{
		sendError(client, "464", ":Password incorrect");
		return;
	}		
	client.set_password(msg.params);
	client.setPassValid(true);
}

void CommandHandler::handleNick(Server& server, Client& client, const t_msg &msg)
{	
	if (msg.params == "")
	{
		sendError(client, "431", ":No nickname given");
		return;
	}
	if(!server.isValideNickname(msg.params))
	{
		sendError(client, "432", msg.params + " :Erroneous nickname");
		return;
	}	
	if (server.isNickInUse(msg.params))
	{
		sendError(client, "433", msg.params + " :Nickname is already in use");		return;
	}
	//informe les autres users d'un changement de nickname
	if (client.isRegistered())
	{
		std::string oldNick = client.get_nickname();
		client.set_nickname(msg.params);
		client.addToSendBuffer(":" + oldNick + " NICK " + msg.params + "\r\n");
	}
	else
		client.set_nickname(msg.params);
}

void CommandHandler::handleUser(Server& server, Client& client, const t_msg &msg)
{	
	if (msg.params == "")
	{
		sendError(client, "461", "USER :Not enough parameters");
		return;
	}
	if (client.isRegistered())
	{
		sendError(client, "462", ":You may not reregister");
		return;
	}

	size_t space = msg.params.find(" ");
	size_t colon = msg.params.find(":");
	std::string username = msg.params.substr(0, space);
	client.set_username(username);
	if (colon != std::string::npos)
	{
		std::string realname = msg.params.substr(colon + 1);
		client.set_realname(realname);
	}
}

void CommandHandler::handleJoin(Server& server, Client& client, const t_msg &msg)
{
	if (msg.params == "")
	{
		sendError(client, "461", "JOIN :Not enough parameters");
		return;
	}
	if (msg.params[0] != '#')
	{
		sendError(client, "403", msg.params + " :No such channel");
		return;
	}
	
	//Extrait le nom du channel et le mdp le cas echeant
	std::string channelName;
	std::string key;
	size_t space = msg.params.find(" ");
	if (space != std::string::npos)
	{
		channelName = msg.params.substr(0, space);
		key = msg.params.substr(space + 1);
	}
	else
		channelName = msg.params;
	
	//Existance channel?
	Channel* channel = server.getChannel(channelName);
		/*Si n'existe pas on le cree*/
	if (!channel)
	{
		server.createChannel(channelName, &client);
		channel = server.getChannel(channelName);
	}
		/*Si existe on verfie les restrictions*/
	else
	{
		/*vérifier si le mode i(nvite) est actif*/
		if(channel->isInviteOnly() && !channel->isInvited(client.get_fd(), &client))
		{
			sendError(client, "473", "JOIN :Invite only");
			return;
		}
		/*vérifier la key*/
		if(!channel->getKey().empty() && channel->getKey() != key)
		{
			sendError(client, "475", "JOIN :Wrong key");
			return;
		}
		/*vérifier la limite users*/
		if(channel->getUserLimit() > 0 && channel->getClientCount() >= channel->getUserLimit())
		{
			sendError(client, "471", "JOIN :Channel is full");
			return;
		}
		channel->addMember(&client);
	}
	/*récupérer le topic*/
	channel->broadcast(":" + client.get_nickname() + " JOIN " + channelName + "\r\n");
	if (channel->getTopic().empty())
		client.addToSendBuffer(":ircserv 331 " + client.get_nickname() + " " + channelName + " :No topic is set\r\n");
	else
		client.addToSendBuffer(":ircserv 332 " + client.get_nickname() + " " + channelName + " :" + channel->getTopic() + "\r\n");

	/*récupérer la liste des membres (pour envoyer NAMES)*/
	client.addToSendBuffer(":ircserv 353 " + client.get_nickname() + " = " + channelName + " :" + channel->getNamesList() + "\r\n");
	client.addToSendBuffer(":ircserv 366 " + client.get_nickname() + " " + channelName + " :End of /NAMES list\r\n");
}

void CommandHandler::handlePart(Server& server, Client& client, const t_msg &msg)
{
	if (msg.params == "")
	{
		sendError(client, "461", "PART :Not enough parameters");
		return;
	}
	if (msg.params[0] != '#')
	{
		sendError(client, "403", msg.params + " :No such channel");
		return;
	}
	
	//Extrait le nom du channel
	std::string channelName;
	std::string message;
	size_t space = msg.params.find(" ");
	if (space != std::string::npos)
	{
		channelName = msg.params.substr(0, space);
		message = msg.params.substr(space + 1);
	}
	else
		channelName = msg.params;
	
	Channel* channel = server.getChannel(channelName);
	
	/*Si channel n'existe pas, message d'erreur*/
	if (!channel)
	{
		sendError(client, "403", channelName + " :No such channel");
		return;
	}
	/*Si client pas membre , message d'erreur*/
	if (!channel->isMember(client.get_fd()))
	{
		sendError(client, "442", channelName + " :You're not on that channel");
		return;
	}
	/*Presence d un message dans la commande PART?*/
	if (!message.empty())
    	channel->broadcast(":" + client.get_nickname() + " PART " + channelName + " " + message + "\r\n");
	else
    	channel->broadcast(":" + client.get_nickname() + " PART " + channelName + "\r\n");
	
	/*Retrait membre et, le cas echeant le channel*/
	channel->removeMember(&client);
	server.removeChannelIfEmpty(channelName);
}

void CommandHandler::handlePrivmsg(Server& server, Client& client, const t_msg &msg)
{
	if (msg.params == "")
	{
		sendError(client, "411", "PRIVMSG :No recipient given (PRIVMSG)");
		return;
	}
	
	//Extrait le destinataire
	std::string targetName;
	std::string message;
	size_t space = msg.params.find(" ");
	if (space != std::string::npos)
	{
		targetName = msg.params.substr(0, space);
		message = msg.params.substr(space + 1);
	}
	else
		targetName = msg.params;
	//Message vide
	if (message.empty())
	{
		sendError(client, "412", ":No text to send");
		return;
	}
	/*destinataire channel*/
	if (targetName[0] == '#')
	{	
		Channel* channel = server.getChannel(targetName);
		
		/*Si channel n'existe pas, message d'erreur*/
		if (!channel)
		{
			sendError(client, "403", targetName + " :No such channel");
			return;
		}

		/*Si client pas membre, message d'erreur*/
		if (!channel->isMember(client.get_fd()))
		{
			sendError(client, "442", targetName + " :You're not on that channel");
			return;
		}
		//Envoi du message au channel hors emetreur
		channel->broadcast(":" + client.get_nickname() + " PRIVMSG " + targetName + " " + message + "\r\n", client.get_fd());
	}
	/*MP*/
	else
	{
		Client* target = server.getClientByNickName(targetName);
		if(!target)
		{
			sendError(client, "401", targetName + " :No such nick");
			return;
		}
		target->addToSendBuffer(":" + client.get_nickname() + " PRIVMSG " + targetName + " " + message + "\r\n");
	}
}

void CommandHandler::handleKick(Server& server, Client& client, const t_msg &msg)
{
	if (msg.params == "")
	{
		sendError(client, "461", "KICK :Not enough parameters");
		return;
	}
	if (msg.params[0] != '#')
	{
		sendError(client, "403", msg.params + " :No such channel");
		return;
	}
	
	//Extrait le nom du channel
	std::string channelName;
	std::string targetName;
	std::string reason;
	
	size_t spaceOne = msg.params.find(" ");
	if (spaceOne == std::string::npos)
	{
		sendError(client, "461", "KICK :Not enough parameters");
		return;
	}
	
	channelName = msg.params.substr(0, spaceOne);
	std::string restOne = msg.params.substr(spaceOne + 1);
	
	size_t spaceTwo = restOne.find(" ");
	if (spaceTwo != std::string::npos)
	{
		targetName = restOne.substr(0, spaceTwo);
		std::string restTwo = restOne.substr(spaceTwo + 1);
		if (!restTwo.empty() && restTwo[0] == ':')
			reason = restTwo.substr(1);
	}
	else
		targetName = restOne;

	Channel* channel = server.getChannel(channelName);
	
	/*Si channel n'existe pas, message d'erreur*/
	if (!channel)
	{
		sendError(client, "403", channelName + " :No such channel");
		return;
	}
	/*Si client pas membre, message d'erreur*/
	if (!channel->isMember(client.get_fd()))
	{
		sendError(client, "442", channelName + " :You're not on that channel");
		return;
	}
	/*Si client membre mais pas operateur, message d'erreur*/
	if (!channel->isOperator(client.get_fd()))
	{
		sendError(client, "482", channelName + " :You're not channel operator");
		return;
	}
	/*Si cible n est pas membre, message d'erreur*/
	Client* target = server.getClientByNickName(targetName);
	if(!target)
	{
		sendError(client, "401", targetName + " :No such nick");
		return;
	}
	if (!channel->isMember(target->get_fd()))
	{
		sendError(client, "441", targetName + " " + channelName + " :They aren't on that channel");
		return;
	}
	
	/*Message de Kick*/
	if(reason != "")
		channel->broadcast(":" + client.get_nickname() + " KICK " + channelName + " " + targetName + " :" + reason + "\r\n");
	else
		channel->broadcast(":" + client.get_nickname() + " KICK " + channelName + " " + targetName + "\r\n");
	
	/*Retrait membre et, le cas echeant le channel*/
	channel->removeMember(target);
	server.removeChannelIfEmpty(channelName);
}

void CommandHandler::handleInvite(Server& server, Client& client, const t_msg &msg)
{
	if (msg.params == "")
	{
		sendError(client, "461", "INVITE :Not enough parameters");
		return;
	}

	/*Extrait le Nickname*/
	std::string targetName;
	std::string channelName;
	size_t space = msg.params.find(" ");

	if (space == std::string::npos)
	{
		sendError(client, "461", "INVITE :Not enough parameters");
		return;
	}
	targetName = msg.params.substr(0, space);
	channelName = msg.params.substr(space + 1);
	
	Channel* channel = server.getChannel(channelName);
	
	/*Si channel n'existe pas, message d'erreur*/
	if (!channel)
	{
		sendError(client, "403", channelName + " :No such channel");
		return;
	}
	/*Si client pas membre, message d'erreur*/
	if (!channel->isMember(client.get_fd()))
	{
		sendError(client, "442", channelName + " :You're not on that channel");
		return;
	}
	/*Si client membre mais pas operateur, message d'erreur*/
	if (channel->isInviteOnly() && !channel->isOperator(client.get_fd()))
	{
		sendError(client, "482", channelName + " :You're not channel operator");
		return;
	}
	Client* target = server.getClientByNickName(targetName);
	if(!target)
	{
		sendError(client, "401", targetName + " :No such nick");
		return;
	}
	if (channel->isMember(target->get_fd()))
	{
		sendError(client, "443", targetName + " " + channelName + " :is already on channel");
		return;
	}
	
	/*Message de INVITE*/
	channel->addInvited(target->get_fd(), target);
	client.addToSendBuffer(":ircserv 341 " + client.get_nickname() + " " + targetName + " " + channelName + "\r\n");
	target->addToSendBuffer(":" + client.get_nickname() + " INVITE " + targetName + " " + channelName + "\r\n");
}

void CommandHandler::handleTopic(Server& server, Client& client, const t_msg &msg)
{
	if (msg.params == "")
	{
		sendError(client, "461", "TOPIC :Not enough parameters");
		return;
	}

	/*Extrait le Nickname*/
	std::string channelName;
	std::string newTopic;
	size_t space = msg.params.find(" ");
	if (space == std::string::npos)
		channelName = msg.params;
	else 
	{	
		channelName = msg.params.substr(0, space);
		newTopic = msg.params.substr(space + 1);
	}
	/*retrait des : pour le topic*/
	if (!newTopic.empty() && newTopic[0] == ':')
        newTopic = newTopic.substr(1);
	
	/*Si channel n'existe pas, message d'erreur*/
	Channel* channel = server.getChannel(channelName);
	if (!channel)
	{
		sendError(client, "403", channelName + " :No such channel");
		return;
	}
	/*Si client pas membre, message d'erreur*/
	if (!channel->isMember(client.get_fd()))
	{
		sendError(client, "442", channelName + " :You're not on that channel");
		return;
	}
	if(newTopic == "")
	{
		if (channel->getTopic().empty())
			client.addToSendBuffer(":ircserv 331 " + client.get_nickname() + " " + channelName + " :No topic is set\r\n");
		else
			client.addToSendBuffer(":ircserv 332 " + client.get_nickname() + " " + channelName + " :" + channel->getTopic() + "\r\n");
		return;
	}
	
	if(channel->isTopicRestricted() && !channel->isOperator(client.get_fd()))
	{
		sendError(client, "482", channelName + " :You're not channel operator");
		return;
	}
	channel->setTopic(newTopic);
	/*Message de TOPIC*/
	channel->broadcast(":" + client.get_nickname() + " TOPIC " + channelName + " :" + newTopic + "\r\n");
}
/*
void CommandHandler::handleMode(Server& server, Client& client, const t_msg &msg)
{
	if (msg.params == "")
	{
		sendError(client, "461", "MODE :Not enough parameters");
		return;
	}
	if (msg.params[0] != '#')
	{
		sendError(client, "403", msg.params + " :No such channel");
		return;
	}
	
	size_t spaceOne = msg.params.find(" ");
	if (spaceOne == std::string::npos)
	{
		std::string channelName;
		channelName = msg.params;
		client.addToSendBuffer(":ircserv 324 " + client.get_nickname() + " " + channelName + " " + channel->getModeString() + "\r\n");
	}
	//Extrait le nom du channel
	
	
	std::string mode;
	std::string param;
	
	std::string restOne = msg.params.substr(spaceOne + 1);
	
	size_t spaceTwo = restOne.find(" ");
	if (spaceTwo != std::string::npos)
	{
		targetName = restOne.substr(0, spaceTwo);
		std::string restTwo = restOne.substr(spaceTwo + 1);
		if (!restTwo.empty() && restTwo[0] == ':')
			reason = restTwo.substr(1);
	}
	else
		targetName = restOne;
*/

		