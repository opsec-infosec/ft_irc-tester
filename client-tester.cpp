/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client-tester.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dfurneau <dfurneau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 10:12:40 by dfurneau          #+#    #+#             */
/*   Updated: 2023/03/15 15:18:31 by dfurneau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client-tester.hpp"
#include "client-message.hpp"

#ifndef RECONNECT_ON_MSG
# define RECONNECT_ON_MSG false
#endif

std::mutex mtx;

//
// Signal Handler
//
namespace {
    volatile std::atomic<int> gSignalStatus( 0 );
    static_assert( std::atomic<int>::is_always_lock_free );
}

void signalHandler( int signal ) {
    gSignalStatus.store( signal );
}

//
// Thread Program
//
class threadObj {
public:
    void operator()( std::string ipAddress, int port, int clientId )
    {
        clientTester client = clientTester( ipAddress, port, clientId );
        mtx.lock();
        std::cout << "Client " << clientId << " exited" << std::endl;
        mtx.unlock();
    }
};

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
    std::cout << "USAGE: ";
    std::cout << prg.substr( prg.find_last_of( "/" ) - 1 ) << " <clients> <serverip> " << std::endl;
    std::cout << "  <clients> = Number of clients to connect to server > 0" << std::endl;
    std::cout << "  <serverip> = IP address of IRC server to connect" << std::endl;
    std::cout << "  <port> = Port number of IRC server to connect on" << std::endl;
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

    for ( int i = 0; i < threadCount; i++ ) {
        th.push_back( std::thread( threadObj(), ipAddress, port, i + 1 ) );
    }

    for ( size_t i = 0; i < th.size(); i++ ) {
        th[i].join();
    }

    std::cout << "Program Terminating" << std::endl;
    return EXIT_SUCCESS;
}

//
// Class Client Tester
//
clientTester::clientTester( const std::string ipAddress, const int port, const int clientId ) : m_ipAddress( ipAddress ),  m_clientId( clientId ), m_port( port ) {
    sigInit();
    this->run();
}

clientTester::~clientTester( ) {

}

//
// Sequence Loop
void clientTester::run( void ) {
    std::stringstream ss;
    std::linear_congruential_engine<unsigned int , 193703 , 0 , 83474882> lce;
    lce.seed( std::chrono::system_clock::now().time_since_epoch().count() );

    m_seconds = lce() % 61;
    if ( m_seconds <= 2 )
        m_seconds = 3;

    ss << "Hello msg from client: " << m_clientId << std::endl;

    for ( int i = 0; i <= m_seconds && running(); i++ )
        std::this_thread::sleep_for( std::chrono::seconds( 1 ) );

    if ( !RECONNECT_ON_MSG )
        ircconnect();

    clientMessage comms = * new clientMessage(m_clientId, "pass");

    comms.authorizeClient();
    comms.joinOwnChannel();
    comms.joinChannel(std::to_string(m_seconds));
    comms.joinOwnChannel();
    comms.inviteToOwnChannel(std::to_string(m_seconds));
    comms.setTopicToOwnName("#" + std::to_string(m_clientId));
    comms.setChannelTopic("#" +  std::to_string(m_seconds), "derp derp derp");
    comms.inviteToOwnChannel("1");
    comms.quitIrc("I'm an idiot...");

    while ( running() && comms.getMessages().size() ) {
        try {
            if ( RECONNECT_ON_MSG )
                ircconnect();

            std::this_thread::sleep_for( std::chrono::milliseconds( 250 ) );
            ircsend( comms.getMessages().front() );
            std::this_thread::sleep_for( std::chrono::milliseconds( 250 ));

            if ( RECONNECT_ON_MSG )
                ircdisconnect();

            comms.getMessages().pop();

            for ( int i = 0; i <= m_seconds && running(); i++ )
                std::this_thread::sleep_for( std::chrono::seconds( 1 ) );

            if ( !RECONNECT_ON_MSG && !comms.getMessages().size())
                break;
        }
        catch ( std::ios_base::failure& ex ) {
            mtx.lock();
            std::cout << "Client " << m_clientId << " failed to send message, retry in 30 seconds" << std::endl;
            mtx.unlock();
            for ( int i = 0; i <= 30 && running(); i++ )
                std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
        }
        catch ( std::runtime_error& ex ) {
            mtx.lock();
            std::cout << "Client " << m_clientId << " " << ex.what() << std::endl;
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

    std::memset( &address, 0, sizeof( address ) );
    address.sin_family = AF_INET;
    address.sin_port = htons( m_port );

    if ( inet_pton( AF_INET, m_ipAddress.c_str(), &address.sin_addr ) <= 0 )
        throw std::runtime_error( "Conversion to sockaddr failed" );

    if ( ( connect( m_fd, (struct sockaddr*)&address, sizeof( address ) ) ) < 0 ) {
        throw std::runtime_error( "Failed to connect to " + m_ipAddress + " on port " + std::to_string( m_port ) );
    }
}

//
// Client disconnect
void clientTester::ircdisconnect( void ) {
    close( m_fd );
    mtx.lock();
    std::cout << "Client " << m_clientId << " closing socket" << std::endl;
    mtx.unlock();
}

//
// Client send message
void clientTester::ircsend( const std::string msg ) {
    if ( !running() )
        return ;

    const char* buffer;

    buffer = msg.data();

    if ( send( m_fd, buffer, msg.size(), 0 ) < 0 )
        throw std::ios_base::failure("Failed to send message");
    mtx.lock();
    std::cout << "Client sending msg: " + msg;
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
