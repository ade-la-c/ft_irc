#include "Channel.hpp"

Channel::Channel() : name("") {
}

Channel::Channel(std::string const & name) : name(name) {
}

Channel::Channel(Channel const & cpy) : name(cpy.name) {
}

Channel::~Channel() {
}

void Channel::add_client(Client & client) {
	if (!subscribed_clients.count(client.getSockFd()))
		subscribed_clients[client.getSockFd()] = client;
}

void Channel::remove_client(Client & client) {
	subscribed_clients.erase(client.getSockFd());
}
