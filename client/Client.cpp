#include "Client.hpp"

Client::Client() : socket(-1) {
}

Client::Client(int socket) : socket(socket) {
}

Client::Client(Client const & cpy) : socket(cpy.socket) {
}

Client::~Client() {
}

void Client::parse_input() {
	//TODO
}
