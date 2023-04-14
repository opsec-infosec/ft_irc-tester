/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client-tester.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dfurneau <dfurneau@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 10:12:49 by dfurneau          #+#    #+#             */
/*   Updated: 2023/04/15 00:40:11 by dfurneau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_TESTER_HPP
# define CLIENT_TESTER_HPP

# define BUFFSIZE 4095
# include "parse.hpp"

# include <cstring>
# include <string>
# include <sstream>
# include <iostream>
# include <random>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <variant>
# include <vector>
# include <map>

class clientTester {
    public:
        clientTester( );
        clientTester( const std::string ipAddress, const int port, const int clientId,  Parse::ParseData* data );
        ~clientTester( );

        void run( void );

    private:
        std::vector<std::string> m_vars;
        const std::string m_ipAddress;
        const int m_port;
        int m_seconds = 60;
        int m_fd = 0;
        int m_clientId;
        size_t m_delay;
        Parse::ParseData* m_data;

        void ircconnect( void );
        void ircdisconnect( void );
        void ircsend( const std::string msg );
        void sigInit( void ) const;
        bool running( void ) const;

        std::string replaceBuffer( const std::string& buffer );
};

#endif
