# include "../includes/ft_irc.hpp"

Message::Message() {
	params.reserve(15);
	parse();
}

Message::Message(std::string & message) : msg(message) {
	params.reserve(15);
	parse();
}

Message::Message(const Message & copy) : msg(copy.message) {
	params.reserve(15);
	parse();
}

Message::~Message() {
}

void Message::parse() {
	if (msg[0] == ':') {
		
	}
}

Message & Message::operator=(const Message & rhs) {
	if (this == &rhs)
		return *this;
	msg = rhs.msg;
	return *this;
}
