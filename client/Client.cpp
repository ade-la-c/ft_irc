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
	Message msg;
	try {
		msg.parse_from_str(string(buf, length));
	} catch (std::exception & e) {
		//TODO what do
	}
}
