/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cycolonn <cycolonn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/17 17:02:59 by cycolonn          #+#    #+#             */
/*   Updated: 2026/11/17 20:13:20 by cycolonn         ###   ########.fr       */
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

        void buf_append(const std::string data);
        bool buf_complete(void) const;

    private:

        int _fd;
        std::string _ip;

        std::string _nickname;
        std::string _username;
        std::string _realname;
        std::string _password;
        bool _hasPassword;
        bool _isRegistered;
        
        std::string _input_buf;
        std::string _output_buf;
};

#endif