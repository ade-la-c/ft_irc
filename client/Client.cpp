#include "../includes/ft_irc.hpp"

Client::Client() : _IoClient(), _IRCClient() {
}

Client::Client(int socket) : _IoClient(socket), _IRCClient() {
}

Client::Client(Client const & cpy) : _IoClient(cpy), _IRCClient(cpy) {
}

Client::~Client() {
}

