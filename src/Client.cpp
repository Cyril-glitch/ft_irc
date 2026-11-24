/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbenaroc <gbenaroc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/24 19:31:15 by gbenaroc          #+#    #+#             */
/*   Updated: 2026/11/24 19:33:00 by gbenaroc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Client.hpp"

Client::Client(int fd, const std::string &ip) : _fd(fd), _ip(ip), _hasPass(false), _hasNick(false), _hasUser(false)
{
}

Client::Client() : _fd(-1), _hasPass(false), _hasNick(false), _hasUser(false)
{
}

Client &Client::operator=(const Client &rhs)
{
	if (this != &rhs)
	{
		_fd = rhs._fd;
		_ip = rhs._ip;
		_nickname = rhs._nickname;
		_username = rhs._username;
		_realname = rhs._realname;
		_password = rhs._password;
		_hasPass = rhs._hasPass;
		_hasNick = rhs._hasNick;
		_hasUser = rhs._hasUser;
		_input_buf = rhs._input_buf;
		_output_buf = rhs._output_buf;
	}
	return *this;
}

Client::~Client()
{
}

Client::Client(const Client &src) : _fd(src._fd), _ip(src._ip), _nickname(src._nickname),
	_username(src._username), _realname(src._realname), _password(src._password),
	_hasPass(src._hasPass), _hasNick(src._hasNick), _hasUser(src._hasUser),
	_input_buf(src._input_buf), _output_buf(src._output_buf)
{
}

int Client::get_fd() const
{
	return _fd;
}

std::string Client::get_nickname() const
{
	return _nickname;
}

std::string Client::get_ip() const
{
	return _ip;
}

std::string Client::get_username() const
{
	return _username;
}

std::string Client::get_password() const
{
	return _password;
}


void Client::set_nickname(const std::string &nickname) 
{
	_nickname = nickname;
	_hasNick = true;
}

void Client::set_password(const std::string &password) 
{
	_password = password;
}

void Client::set_username(const std::string &username) 
{
	_username = username;
	_hasUser = true;
}

void Client::set_realname(const std::string &realname) 
{
	_realname = realname;
}

void Client::setPassValid(bool valid)
{
    _hasPass = valid;
}

bool Client::isRegistered() const
{
	return _hasPass && _hasNick && _hasUser;
}

void Client::buf_append(const std::string &data)
{
	_input_buf += data;
}

bool Client::buf_complete(void) const
{
	return _input_buf.find("\r\n") != std::string::npos;
}

std::string Client::getNextCommand()
{
	size_t pos = _input_buf.find("\r\n");
	std::string cmd = _input_buf.substr(0, pos);
	_input_buf.erase(0, pos + 2);
	return cmd;	
}

void  Client::addToSendBuffer(const std::string &msg)
{
	_output_buf += msg;
}

std::string &Client::getSendBuffer()
{
	return _output_buf;
}

void Client::clearSentBytes(size_t n)
{
	_output_buf.erase(0, n);
}