/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client-tester.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dfurneau <dfurneau@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 10:12:40 by dfurneau          #+#    #+#             */
/*   Updated: 2023/04/27 13:13:10 by dfurneau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/client-tester.hpp"
#include "../includes/parse.hpp"
#include "../includes/thread.hpp"

#ifndef RECONNECT_ON_MSG
# define RECONNECT_ON_MSG false
#endif

//
// Class Client Tester
//
clientTester::clientTester( ) : m_port( -1 ) { }

// m_vars 0 = ipAddress, 1 = port, 2 = clientId, 3 = time intervale (m_seconds), 4 = client fd (m_fd)
clientTester::clientTester( const std::string ipAddress,
                            const int port,
                            const int clientId,
                            Parse::ParseData* data ) : m_ipAddress( ipAddress ), m_port( port ), m_clientId( clientId ), m_data( data ) {
    m_vars.push_back( ipAddress );
    m_vars.push_back( std::to_string( port ) );
    m_vars.push_back( std::to_string( clientId ) );
    sigInit();
    this->run();
}

clientTester::~clientTester( ) {
    mtx.lock();
    std::cout << "Client " <<  m_clientId << ":" << ( m_fd ? std::to_string( m_fd ) : "*" ) << " Closing socket" << std::endl;
    mtx.unlock();

    if ( m_fd )
        close( m_fd );
}

//
// Sequence Loop
void clientTester::run( void ) {
    std::linear_congruential_engine<unsigned int , 193703 , 0 , 83474882> lce;
    lce.seed( std::chrono::system_clock::now().time_since_epoch().count() );

    m_seconds = lce() % 61;
    if ( m_seconds <= 2 )
        m_seconds = 3;

    m_vars.push_back( std::to_string( m_seconds ) );

    mtx.lock();
    std::cout << "Client " << m_clientId << ":* Waiting to Connect in " << m_seconds << " seconds" << std::endl;
    mtx.unlock();

    for ( int i = 0; i <= m_seconds && running(); i++ )
        std::this_thread::sleep_for( std::chrono::seconds( 1 ) );

    try {
        if ( !RECONNECT_ON_MSG )
            ircconnect();
    }
    catch ( std::runtime_error& ex ) {
        mtx.lock();
        std::cout << "Client " << m_clientId << ":" << ( m_fd ? std::to_string( m_fd ) : "*" ) << " " << ex.what() << std::endl;
        mtx.unlock();
        return ;
    }

    while ( running() ) {
        try {
            if ( RECONNECT_ON_MSG )
                ircconnect();

            std::this_thread::sleep_for( std::chrono::milliseconds( 250 ) );

            for ( std::vector<std::string>::const_iterator it = m_data->m_loop.begin(); it != m_data->m_loop.end(); ++it ) {
                ircsend( replaceBuffer( *it ) );
                for ( size_t i = 0; i < m_delay; i++ )
                    std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
            }

            std::this_thread::sleep_for( std::chrono::milliseconds( 250 ));

            if ( RECONNECT_ON_MSG )
                ircdisconnect();

            for ( int i = 0; i <= m_seconds && running(); i++ )
                std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
        }
        catch ( std::ios_base::failure& ex ) {
            mtx.lock();
            std::cout << "Client "<<  m_clientId  << ":" <<  m_fd  << " failed to send message, retry in 30 seconds" << std::endl;
            mtx.unlock();
            for ( int i = 0; i <= 30 && running(); i++ )
                std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
        }
        catch ( std::runtime_error& ex ) {
            mtx.lock();
            std::cout << "Client " << m_clientId << ":" <<  m_fd << " " << ex.what() << std::endl;
            mtx.unlock();
            break ;
        }
    }

    if ( !RECONNECT_ON_MSG )
        ircdisconnect();
}

//
// Signal handler init
void clientTester::sigInit( void ) const {
    std::signal( SIGINT, signalHandler );
    std::signal( SIGTERM, signalHandler );
    std::signal( SIGQUIT, signalHandler );
    std::signal( SIGHUP, signalHandler );
}

//
// Client connect
void clientTester::ircconnect( void ) {
    if ( !running() )
        return;

    struct sockaddr_in address;

    if ( (m_fd = socket( AF_INET, SOCK_STREAM, 0 ) ) < 0 )
        throw std::runtime_error( "Unable to create socket" );

    mtx.lock();
    std::cout << "Client " << m_clientId << ":" << m_fd << " Connecting" << std::endl;
    mtx.unlock();

    m_vars.push_back( std::to_string( m_fd ) );

    std::memset( &address, 0, sizeof( address ) );
    address.sin_family = AF_INET;
    address.sin_port = htons( m_port );

    if ( inet_pton( AF_INET, m_ipAddress.c_str(), &address.sin_addr ) <= 0 )
        throw std::runtime_error( "Conversion to sockaddr failed" );

    if ( ( connect( m_fd, (struct sockaddr*)&address, sizeof( address ) ) ) < 0 )
        throw std::runtime_error( "Failed to connect to " +  m_ipAddress + " on port " + std::to_string( m_port ) );


    for ( std::vector<std::string>::const_iterator it = m_data->m_connect.begin(); it != m_data->m_connect.end(); ++it ) {
        ircsend( replaceBuffer( *it ) );
        for ( size_t i = 0; i < m_delay; i++ )
            std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
    }

    char buffer[BUFFSIZE];
    ssize_t byteRec;

    size_t timeOut = 10;
    while ( ( byteRec = recv( m_fd, &buffer, BUFFSIZE, 0 ) ) <= 0 ) {
        std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
        if ( --timeOut <= 0 )
            throw std::runtime_error( "Failed to receive reply" );
    }

    for ( std::vector<std::string>::const_iterator it = m_data->m_afterConnect.begin(); it != m_data->m_afterConnect.end(); ++it ) {
        ircsend( replaceBuffer( *it ) );
        for ( size_t i = 0; i < m_delay; i++ )
            std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
    }
}

//
// Client disconnect
void clientTester::ircdisconnect( void ) {
    for ( std::vector<std::string>::const_iterator it = m_data->m_disconnect.begin(); it != m_data->m_disconnect.end(); ++it )
        ircsend( replaceBuffer( *it ) );
}

//
// Client send message
void clientTester::ircsend( const std::string msg ) {
    if ( msg.empty() )
        return ;

    if ( !running() )
        return ;

    const char* buffer;

    buffer = msg.data();

    if ( send( m_fd, buffer, msg.size(), 0 ) < 0 )
        throw std::ios_base::failure("Failed to send message");

    mtx.lock();
    std::cout << "Client " << m_clientId << ":" << m_fd << " sending msg: " << msg;
    mtx.unlock();
}

//
// Client check for signal
bool clientTester::running( void ) const {
    int signal = gSignalStatus.load();

    if (signal == SIGTERM || signal == SIGINT \
     || signal == SIGQUIT || signal == SIGHUP )
        return false;

    return true;
}

//
// Client runtime substitution of args
std::string clientTester::replaceBuffer( const std::string& buffer ) {
    std::string buff = std::string( buffer );

    size_t spos = buffer.find( "{" );
    size_t epos = buffer.find( "}" );

    while ( spos != std::string::npos && epos != std::string::npos && spos - epos > 0 ) {
        size_t vecLoc;

        try { vecLoc = std::stoi( buff.substr( spos + 1, ( epos - 1 ) - spos ) ); }
        catch ( std::invalid_argument& ex ) { return buffer; }

        if ( vecLoc > m_vars.size() - 1 )
            return buffer;

        buff.replace( spos, ( epos + 1 ) - spos,  m_vars[vecLoc] );

        spos = buff.find( "{" );
        epos = buff.find( "}" );
    }

    m_delay = 0;
    spos = buff.find( "[" );
    epos = buff.find( "]" );

    while ( spos != std::string::npos && epos != std::string::npos && spos - epos > 0 ) {
        try { m_delay = std::stoi( buff.substr( spos + 1, ( epos - 1) - spos ) ); }
        catch ( std::invalid_argument& ex ) { }

        buff.erase( spos );

        spos = buff.find( "[" );
        epos = buff.find( "]" );
    }

    return buff;
}
