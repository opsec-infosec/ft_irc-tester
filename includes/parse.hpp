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

        int m_port;
        int m_fd;

        std::map<std::string, std::string> m_replace;
        bool m_init = false;

        void loadConf( const std::string& filename, std::vector<std::string>& vector );
};

#endif
