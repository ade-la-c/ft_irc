#include "Client.hpp"

Client::Client() : _socket(-1) {
}

Client::Client(int socket) : _socket(socket) {
}

Client::Client(Client const & cpy) : _socket(cpy._socket) {
}

Client::~Client() {
}

void Client::parse_input() {
	//TODO
}
