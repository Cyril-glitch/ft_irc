/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbenaroc <gbenaroc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/17 17:02:59 by cycolonn          #+#    #+#             */
/*   Updated: 2026/11/24 19:32:38 by gbenaroc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

class Client
{
    public:

        Client();
        Client(int fd, const std::string &ip);
        Client(const Client &src);
        ~Client();

        Client& operator=(const Client &rhs);

        int get_fd() const;
        std::string get_ip() const;

        std::string get_nickname() const;
        std::string get_username() const;
        std::string get_password() const;

		void set_nickname(const std::string &nickname);
		void set_username(const std::string &username);
		void set_realname(const std::string &realname);
		void set_password(const std::string &password);

        bool isRegistered() const;
		void setPassValid(bool valid);
		
		/*Tampon d'entree*/
		void buf_append(const std::string &data);
        bool buf_complete(void) const;
		std::string getNextCommand();

		/*Tampon de sortie*/
		void addToSendBuffer(const std::string &msg);
		std::string &getSendBuffer();
		void clearSentBytes(size_t n);

    private:

        int _fd;
        std::string _ip;

        std::string _nickname;
        std::string _username;
        std::string _realname;
        std::string _password;
        bool _hasPass;
		bool _hasNick;
		bool _hasUser;
        
        std::string _input_buf;
        std::string _output_buf;
};

#endif