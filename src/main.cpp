/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dfurneau <dfurneau@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 10:12:40 by dfurneau          #+#    #+#             */
/*   Updated: 2023/04/15 00:51:59 by dfurneau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/client-tester.hpp"
#include "../includes/parse.hpp"
#include "../includes/thread.hpp"

#ifndef RECONNECT_ON_MSG
# define RECONNECT_ON_MSG false
#endif

//
// Check if IP is valid called from main
//
bool isValidIp( std::string ip ) {
    if ( ip.size() < 7 )
        return false;

    std::vector<std::string> v;
    std::stringstream ss( ip );

    while ( ss.good() ) {
        std::string substr;
        getline( ss, substr, '.' );
        v.push_back( substr );
    }

    if ( v.size() != 4 )
        return false;

    for ( std::vector<std::string>::const_iterator cit = v.cbegin(); cit != v.cend(); cit++ ) {
        if ( cit->size() > 1 && (*cit)[0] == '0' )
            return false;

        if ( std::count_if( cit->begin(), cit->end(), isalpha ) )
            return false;

        if ( stoi( *cit ) > 255 || ( std::distance( v.cbegin(), cit ) == 3 && stoi( *cit ) == 0 ) )
            return false;
    }
    return true;
}

//
// Usage called from main
//
int usage( std::string prg ) {
    std::cout << std::endl << "USAGE: ";
    std::cout << prg.substr( prg.find_last_of( "/" ) + 1 ) << " <clients> <serverip> " << std::endl;
    std::cout << "  <clients> = Number of clients to connect to server > 0" << std::endl;
    std::cout << "  <serverip> = IP address of IRC server to connect" << std::endl;
    std::cout << "  <port> = Port number of IRC server to connect on" << std::endl;
    std::cout << std::endl << "https://github.com/opsec-infosec/ft_irc-tester" << std::endl << std::endl;
    return EXIT_FAILURE;
}

//
// Main Entrypoint
//
int main( int ac, char** av ) {
    std::string ipAddress;
    int threadCount;
    int port;
    std::vector<std::thread> th;

    if ( ac != 4 )
        return usage( av[0] );

    threadCount = std::stoi( av[1] );
    ipAddress = av[2];
    port = std::stoi( av[3] );

    if ( threadCount <= 0 || !isValidIp( ipAddress ) || ( port <= 0 || port >= 65535 ) )
        return usage( av[0] );

    Parse* p = nullptr;
    try {
        p = new Parse( "./conf/replace.conf", "./conf/connect.conf", "./conf/after-connect.conf", "./conf/disconnect.conf", "./conf/loop.conf" );
        p->init( );
    }
    catch ( std::runtime_error& ex ) {
        std::cout << "Conf files missing from ./conf directory" << std::endl;
        if ( p )
            delete p;
        return EXIT_FAILURE;
    }

    for ( int i = 0; i < threadCount; i++ ) {
        th.push_back( std::thread( threadObj(), ipAddress, port, i + 1, p->m_data ) );
    }

    for ( size_t i = 0; i < th.size(); i++ ) {
        th[i].join();
    }

    std::cout << "Program Terminated" << std::endl;
    delete p;
    return EXIT_SUCCESS;
}
