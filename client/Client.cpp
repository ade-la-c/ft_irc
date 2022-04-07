#include "../includes/ft_irc.hpp"

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
		msg.parse_from_str(std::string(buf, length));
	} catch (std::exception & e) {
		//TODO what do
	}
}

void Client::reg() {
	if (pass_set && nick_set && user_set) {
		if (password == Database::get_instance()->password) {
			registered = true;
		}
	}
}
