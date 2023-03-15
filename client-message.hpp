
#ifndef CLIENT_MESSAGE_HPP
# define CLIENT_MESSAGE_HPP

# include <queue>
# include <string>
# include <limits>

class clientMessage {
    private:
        typedef std::queue<std::string> messageQueue;

        const std::string password;

        messageQueue messages;
        std::string nickname;

        void push(const std::string & message);

    public:
        clientMessage(const int clientId, const std::string pass);

        messageQueue & getMessages();

        void authorizeClient();

        void joinChannel(const std::string & channelName);
        void inviteToChannel(const std::string & nickname, const std::string & channelName);
        void setChannelTopic(const std::string & channelName, const std::string & channelTopic);

        void getChannelTopic(const std::string & channelName);

        void joinOwnChannel();
        void inviteToOwnChannel(const std::string & nickname);
        void setTopicToOwnName(const std::string & channelName);

        void setNickname(const std::string & nickname);
        void setUserName(const std::string & username);
        void setPassword(const std::string & password);
        void setUserMode(const std::string & nickname, const std::string & modeString);

        void quitIrc(const std::string & quitReason);
        void killClientConnection(const std::string & clientName, const std::string & killComment);

        void sendExtremeMessage();
};

#endif
