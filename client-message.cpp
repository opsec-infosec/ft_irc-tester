
#include "client-message.hpp"

void clientMessage::push(const std::string & message) {
    this->messages.push(message + "\r\n");
}

clientMessage::clientMessage(const int clientId, const std::string pass) : password(pass) {
    this->nickname = std::to_string(clientId);
}

clientMessage::messageQueue & clientMessage::getMessages() {
    return this->messages;
}

void clientMessage::authorizeClient() {
    this->setPassword(this->password);
    this->setNickname("T" + this->nickname);
    this->setUserName("T" + this->nickname);
}

void clientMessage::joinChannel(const std::string & channelName) {
    this->push("JOIN " + channelName);
}

void clientMessage::inviteToChannel(const std::string & nickname, const std::string & channelName) {
    this->push("INVITE " + nickname + " " + channelName);
}

void clientMessage::setChannelTopic(
    const std::string & channelName, const std::string & channelTopic
) {
    this->push("TOPIC " + channelName + " " + channelTopic);
}

void clientMessage::getChannelTopic(const std::string & channelName) {
    this->push("TOPIC " + channelName);
}

void clientMessage::joinOwnChannel() {
    this->joinChannel("#" + this->nickname);
}

void clientMessage::inviteToOwnChannel(const std::string & nickname) {
    this->inviteToChannel(nickname, "#" + this->nickname);
}

void clientMessage::setTopicToOwnName(const std::string & channelName) {
    this->setChannelTopic(channelName, this->nickname);
}

void clientMessage::setNickname(const std::string & nickname) {
    this->push("NICK " + nickname);
}

void clientMessage::setUserName(const std::string & username) {
    this->push("USER " + username);
}

void clientMessage::setPassword(const std::string & password) {
    this->push("PASS " + password);
}

void clientMessage::setUserMode(const std::string & nickname, const std::string & modeString) {
    this->push("MODE " + nickname + " " + modeString);
}

/**
* @param quitReason - \b should be passed if you don't want to provide a quitReason.
**/
void clientMessage::quitIrc(const std::string & quitReason) {
    this->push("QUIT " + quitReason);
}

void clientMessage::killClientConnection(const std::string & clientName, const std::string & killComment) {
    this->push("KILL " + clientName + " " + killComment);
}

void clientMessage::sendExtremeMessage() {
    std::string extremelyLongMessage;

    extremelyLongMessage.resize(std::numeric_limits<int>::max());

    this->push(extremelyLongMessage);
}
