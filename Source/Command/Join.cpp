#include "Join.hpp"

Join::Join() {}
Join::Join(const Join& rhs) : Command(rhs) {(void)rhs;}
Join& Join::operator=(const Join& rhs) {(void)rhs; return *this;}
Join::~Join() {}

void Join::execute(Resource& resource, Message message) {
	Client*	client = resource.findClient(message.getClientFd());

	if (!client->getRegistered()) return;
	if (message.getParam().size() < 2) {
		reply.errNeedMoreParams(client, message.getFirstParam());
		return;
	}
	std::vector<std::string>	channels;
	std::vector<std::string>	keys;

	splitByComma(channels, message.getParam()[1]);
	if (message.getParam().size() >= 3)
		splitByComma(keys, message.getParam()[2]);
	for (std::size_t i = 0; i < channels.size(); i++) {
		Channel* channel = resource.findChannel(channels[i]);
		if (channel == 0) {
			if ((channels[i][0] != '#' && channels[i][0] != '&') || hasControlG(channels[i])) {
				reply.errBadChanMask(client);
				continue;
			}
			resource.addChannel(channels[i]);
			channel = resource.findChannel(channels[i]);
			channel->addOperator(client);
			channel->addMode('t');
		} else {
			if (channel->hasMode('k') && (keys.size() <= i || (i < keys.size() && channel->getKey() != keys[i]))) {
				reply.errBadChannelKey(client, channel);
				continue;
			}
			if (channel->hasMode('l') && channel->getUserLimit() <= channel->getClientList().size()) {
				reply.errChannelIsFull(client, channel);
				continue;
			}
			if (channel->hasMode('i') && !channel->checkInvited(client)) {
				reply.errInviteOnlyChan(client, channel);
				continue;
			}
			if ((channels[i][0] != '#' && channels[i][0] != '&') || hasControlG(channels[i])) {
				reply.errBadChanMask(client);
				continue;
			}
		}
		channel->addClient(client);
		client->addJoinedChannel(channel);
		sendMessageToChannel(channel, client);
		if (channel->getTopic().size()) {
			reply.rplTopic(client, channel);
			reply.rplTopicWhoTime(client, channel);
		}
	}
}

void	Join::splitByComma(std::vector<std::string>& target, std::string param) {
	std::string	tmp;
	std::size_t	index;

	while ((index = param.find(',')) != std::string::npos) {
		tmp = param.substr(0, index);
		target.push_back(tmp);
		param.erase(0, index + 1);
	}
	if (param.size()) target.push_back(param);
}

void	Join::sendMessageToChannel(Channel* channel, Client* client) {
	const std::set<Client*>& clientList = channel->getClientList();
	std::string message;
	std::set<Client*>::iterator iter;
	message = "";
	
	for (iter = clientList.begin(); iter != clientList.end(); iter++) {
		(*iter)->addWriteBuffer(":" + client->getClientInfo() + " JOIN " + channel->getName() + "\r\n");
		if (channel->hasOperator((*iter))) message += '@';
		message += (*iter)->getNickname() + " ";
	}
	client->addWriteBuffer(":IRC_Server 353 " + client->getNickname() + " @ " + channel->getName() + " :");
	client->addWriteBuffer(message + "\r\n");
	client->addWriteBuffer(":IRC_Server 366 " + client->getNickname() + " " + channel->getName() + " :End of /NAMES list\r\n");
	std::cout << ":IRC_Server 353 " + client->getNickname() + " @ " + channel->getName() + " :" + message << '\n';
	std::cout << ":IRC_Server 366 " + client->getNickname() + " " + channel->getName() + " :End of /NAMES list\n";
}

 bool	Join::hasControlG(std::string param) {
 	for (std::size_t i = 0; i < param.size(); i++)
 		if (param[i] == 7) return true;
 	return false;
 }