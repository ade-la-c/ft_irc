# include "../includes/ft_irc.hpp"

Message::Message() {
}

Message::Message(const Message & copy) {
}

Message::~Message() {
}

Message & Message::operator=(const Message & rhs) {
	if (this == &rhs)
		return *this;
}
