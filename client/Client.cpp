#include "Client.hpp"

Client::Client() : socket(-1), registered(false), oper(false) {
}

Client::Client(int socket) : socket(socket), registered(false), oper(false) {
}

Client::Client(Client const & cpy) : socket(cpy.socket), registered(cpy.registered), oper(cpy.oper) {
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
