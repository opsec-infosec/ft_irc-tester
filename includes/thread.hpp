/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dfurneau <dfurneau@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 02:57:16 by dfurneau          #+#    #+#             */
/*   Updated: 2023/04/15 00:47:12 by dfurneau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef THREAD_HPP
# define THREAD_HPP

# include "client-tester.hpp"
# include "parse.hpp"

# include <mutex>
# include <atomic>
# include <thread>
# include <string>
# include <iostream>
# include <csignal>

inline std::mutex mtx;

//
// Signal Handler
//
namespace {
    inline volatile std::atomic<int> gSignalStatus( 0 );
    static_assert( std::atomic<int>::is_always_lock_free );
}

inline void signalHandler( int signal ) {
    gSignalStatus.store( signal );
}

//
// Thread Program
//
class threadObj {
    public:
        void operator()( std::string ipAddress, int port, int clientId, Parse::ParseData* data )
        {
            clientTester client = clientTester( ipAddress, port, clientId, data );
            mtx.lock();
            std::cout << "Client " << clientId << ":* " << "Thread exiting" << std::endl;
            mtx.unlock();
        }
};

#endif
