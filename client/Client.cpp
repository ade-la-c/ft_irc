#include "Client.hpp"

Client::Client() : ip(0) {
}

Client::Client(uint32_t ip) : ip(ip) {
}

Client::Client(Client const & cpy) : ip(cpy.ip) {
}

Client::~Client() {
}

void Client::parse_input() {
	//TODO
}
