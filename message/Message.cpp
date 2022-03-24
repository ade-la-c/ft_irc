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
	std::string::size_type pos;

	if (!msg.find("\r\n"))
		//TODO no crlf
	if (msg[0] == ':') {
		pos = prefix();
	}
	pos = command(pos);
	if (msg[pos] != '\r' && msg[pos + 1] != '\n') {
		pos = params(pos);
	}
}

std::string::size_type prefix(std::string::size_type pos) {

}

std::string::size_type command(std::string::size_type pos) {

}

std::string::size_type params(std::string::size_type pos) {

}

Message & Message::operator=(const Message & rhs) {
	if (this == &rhs)
		return *this;
	msg = rhs.msg;
	return *this;
}
