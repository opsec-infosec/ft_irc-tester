/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client-tester.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dfurneau <dfurneau@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 10:12:49 by dfurneau          #+#    #+#             */
/*   Updated: 2023/03/14 11:20:01 by dfurneau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_TESTER_HPP
# define CLIENT_TESTER_HPP

# include <cstring>
# include <sstream>
# include <iostream>
# include <random>
# include <thread>
# include <csignal>
# include <mutex>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <vector>

class clientTester {
    public:
        clientTester( const std::string ipAddress, const int port, const int clientId );
        ~clientTester( );

        void run( void );

    private:
        const std::string m_ipAddress;
        const int m_clientId;
        const int m_port;
        int m_seconds = 60;
        int m_fd;

        void ircconnect( void );
        void ircdisconnect( void );
        void ircsend( const std::string msg );
        void sigInit( void ) const;
        bool running( void ) const;
};

#endif
