/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dfurneau <dfurneau@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 02:57:31 by dfurneau          #+#    #+#             */
/*   Updated: 2023/04/14 01:06:49 by dfurneau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_HPP
# define PARSE_HPP

# include <string>
# include <stdexcept>
# include <fstream>
# include <iostream>
# include <sstream>
# include <vector>
# include <map>


class Parse {
    public:
        Parse( );
        Parse( const std::string& replaceConf, const std::string& connectConf, const std::string& afterConf, const std::string& disconnectConf, const std::string& loopConf );
        ~Parse( );

        class ParseData {
            public:
                std::vector<std::string> m_connect;
                std::vector<std::string> m_afterConnect;
                std::vector<std::string> m_disconnect;
                std::vector<std::string> m_loop;
        };

        ParseData* m_data = new ParseData();
        void init( );

    private:
        std::string m_replaceConf;
        std::string m_connectConf;
        std::string m_afterConf;
        std::string m_disconnectConf;
        std::string m_loopConf;

        std::map<std::string, std::string> m_replace;
        bool m_init = false;

        void loadConf( const std::string& filename, std::vector<std::string>& vector );
};

#endif
