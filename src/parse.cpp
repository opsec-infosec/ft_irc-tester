/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dfurneau <dfurneau@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 00:13:18 by dfurneau          #+#    #+#             */
/*   Updated: 2023/04/14 00:30:16 by dfurneau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parse.hpp"

//
// Parse Class
//
Parse::Parse( ) { }

Parse::Parse( const std::string& replaceConf,
                const std::string& connectConf,
                const std::string& afterConf,
                const std::string& disconnectConf,
                const std::string& loopConf ) {

    if ( replaceConf.empty() || connectConf.empty() || disconnectConf.empty() || loopConf.empty() )
        throw std::runtime_error( "Invalid Filename" );

    m_replaceConf = replaceConf;
    m_connectConf = connectConf;
    m_afterConf = afterConf;
    m_disconnectConf = disconnectConf;
    m_loopConf = loopConf;
    m_init = true;

 };

Parse::~Parse( ) {
    if ( !m_data->m_connect.empty() )
        m_data->m_connect.erase( m_data->m_connect.begin(), m_data->m_connect.end() );

    if ( !m_data->m_afterConnect.empty() )
        m_data->m_afterConnect.erase( m_data->m_afterConnect.begin(), m_data->m_afterConnect.end() );

    if ( !m_data->m_loop.empty() )
        m_data->m_loop.erase( m_data->m_loop.begin(), m_data->m_loop.end() );

    if ( !m_data->m_disconnect.empty() )
        m_data->m_disconnect.erase( m_data->m_disconnect.begin(), m_data->m_disconnect.end() );

    delete m_data;
 };

//
// Parse init
void Parse::init( ) {
    if ( !m_init )
        throw std::runtime_error( "Not Initialized" );

    std::ifstream fstream;

    fstream.open( m_replaceConf, std::fstream::in );
    if ( !fstream.is_open() )
        throw std::runtime_error( "Failed to read file" );

    while ( fstream ) {
        std::string buffer;

        std::getline( fstream, buffer );
        if ( !buffer.empty() ) {
            std::string key = buffer.substr( 0, buffer.find_first_of( " " ) );
            std::string val = buffer.substr( buffer.find_last_of( " " ) + 1 );

            m_replace.insert( std::pair( key, val ) );
        }
    }
    fstream.close();

    loadConf( m_connectConf, m_data->m_connect );
    loadConf( m_afterConf, m_data->m_afterConnect );
    loadConf( m_loopConf, m_data->m_loop );
    loadConf( m_disconnectConf, m_data->m_disconnect );
}

//
// Load configuration files
void Parse::loadConf( const std::string& filename, std::vector<std::string>& vector ) {
    if ( !m_init )
        throw std::runtime_error( "Not Initialized" );

    std::ifstream fstream;

    fstream.open( filename );
    if ( !fstream.is_open() )
        throw std::runtime_error( "Failted to read file " );

    while ( fstream ) {
        std::string buffer;

        std::getline( fstream, buffer );
        if ( !buffer.empty() ) {
            size_t spos = buffer.find( "<" );
            size_t epos = buffer.find( ">" );

            while ( spos != std::string::npos && epos != std::string::npos && spos - epos > 0 ) {
                std::map<std::string, std::string>::iterator it = m_replace.find( buffer.substr( spos, ( epos + 1 ) - spos ) );

                if ( it != m_replace.end() )
                    buffer.replace( spos, ( epos + 1 ) - spos, it->second );

                spos = buffer.find( "<" );
                epos = buffer.find( ">" );
            }
            vector.push_back( buffer + "\r\n" );
        }
    }
    fstream.close();
}
