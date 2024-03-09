#include "MessageHandler.hpp"

MessageHandler::MessageHandler() {}
MessageHandler::MessageHandler(const MessageHandler& rhs) {(void)rhs;}
MessageHandler& MessageHandler::operator=(const MessageHandler& rhs) {(void)rhs; return *this;}
MessageHandler::~MessageHandler() {}

void MessageHandler::handleMessage(std::vector<Message>& messages, const int& fd, std::string buffer) {
	std::vector<std::string>	splitedBuffer;

	splitBuffer(splitedBuffer, buffer);
	for (size_t i = 0; i < splitedBuffer.size(); i++) {
		if (!splitedBuffer[i].size()) continue;
		Message	message;
		splitToParam(splitedBuffer[i], message);
		message.setClientFd(fd);
		messages.push_back(message);
	}
}

void	MessageHandler::splitBuffer(std::vector<std::string>& splitedBuffer, std::string& buffer) {
	std::string	tmp;
	size_t		index;

	while ((index = buffer.find("\r\n")) != std::string::npos) {
		tmp = buffer.substr(0, index);
		splitedBuffer.push_back(tmp);
		buffer.erase(0, index + 2);
	}
}

void	MessageHandler::splitToParam(std::string& splitedBuffer, Message& message) {
	std::string	tmp;
	std::vector<std::string>	params;
	size_t					index;
	size_t					colonIndex;

	message.setOriginalMessage(splitedBuffer);
	while (true) {
		index = splitedBuffer.find(' ');
		colonIndex = splitedBuffer.find(':');
		if (colonIndex != std::string::npos && colonIndex < index) {
			tmp = splitedBuffer.substr(0, colonIndex);
			params.push_back(tmp);
			splitedBuffer.erase(0, colonIndex);
			break ;
		} else if (index == std::string::npos) {
			break ;
		}
		tmp = splitedBuffer.substr(0, index);
		params.push_back(tmp);
		splitedBuffer.erase(0, index + 1);
	}
	if (splitedBuffer.size()) {
		if (splitedBuffer[0] == ':') splitedBuffer.erase(0, 1);
		params.push_back(splitedBuffer);
	}
	message.setParam(params);
	message.setFirstParam(params[0]);
	if (isCommand(params[0])) message.setIsCommand(true);
}

bool	MessageHandler::isCommand(const std::string& param) {
	const std::string commands[9] = {
			"NICK",
			"INVITE",
			"JOIN",
			"KICK",
			"MODE",
			"PASS",
			"TOPIC",
			"USER",
			"PRIVMSG"
	};
	for (int i = 0; i < 9; i++) {
		if (param == commands[i]) return true;
	}
	return false;
}
