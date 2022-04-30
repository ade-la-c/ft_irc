# include "../includes/ft_irc.hpp"

Message::Message() : params(15) {
	complete = false;
	parse();
}

Message::Message(std::string const & message) : msg(message), params(15) {
	complete = false;
	parse();
}

Message::Message(const Message & copy) : msg(copy.msg), params(15) {
	complete = false;
	parse();
}

Message::~Message() {
}

void Message::parse_from_str(std::string const & msg) {
	this->msg = msg;
	parse();
}

void Message::parse() {
	std::string::size_type pos = 0;
	std::string::size_type end;

	msg = buffer + msg;
	// end = msg.find("\r\n");
	end = msg.find("\n");

	if (end > MAX_SIZE - 2 && end != std::string::npos) {
		throw IllFormedMessageException();
	} else if (end == std::string::npos) {
		buffer = msg;
		return;
	}

	complete = true;

	if (msg[0] == ':') {
		pos = parse_prefix(end);
		pos++;
	}

	pos = parse_command(pos, end);

	if (pos != end) {
		pos++;
		pos = parse_params(pos, end);
	}
}

bool Message::is_complete() {
	return complete;
}

std::string Message::get_prefix() const {
	return prefix;
}

std::string Message::get_command() const {
	return command;
}

std::vector<std::string> Message::get_params() const {
	return params;
}

std::vector<std::string>::size_type Message::get_params_count() const {
	return params_count;
}

bool Message::is_numeric(std::string str, std::string::size_type pos, std::string::size_type count) {
	for (std::string::size_type i = pos; i < str.size() && i < pos + count; i++) {
		if (!std::isdigit(str[i]))
			return false;
	}
	return true;
}

bool Message::is_special(int c) {
	return ((c >= '{' && c <= '}') || (c >= '[' && c <= '`'));
}

bool Message::is_nickname(std::string str, std::string::size_type pos, std::string::size_type count) {
	if (!std::isalpha(str[pos]) && !is_special(str[pos]))
		return false;
	for (std::string::size_type i = pos; str[i] != ' ' && i < str.size() && i < pos + count; i++) {
		if ((!std::isalpha(str[i]) && !is_special(str[i]) && str[i] != '-') || i - pos > 8)
			return false;
	}
	return true;
}

bool Message::is_channel(std::string str, std::string::size_type pos, std::string::size_type count) {
	(void)str, (void)pos, (void)count;
	return true;
}

std::string::size_type Message::parse_prefix(std::string::size_type end) {
	std::string::size_type sp;

	sp = this->msg.find(" ");
	if (sp >= end)
		throw IllFormedMessageException();
	if (!is_nickname(msg, 1, sp)) {
		throw IllFormedMessageException();
	}
	prefix = this->msg.substr(1, sp - 1);
	return sp;
}

std::string::size_type Message::parse_command(std::string::size_type pos, std::string::size_type end) {
	std::string::size_type sp;

	sp = this->msg.find(" ", pos);
	if (sp > end)
		sp = end;
	if (sp - pos == 3 && is_numeric(msg, pos, 3)) {
		command = msg.substr(pos, 3);
	} else {
		for (std::string::size_type i = pos; i < sp; i++) {
			if (!std::isalpha(msg[i]))
				throw IllFormedMessageException();
			command += msg[i];
		}
	}
	return sp;
}

std::string::size_type Message::parse_params(std::string::size_type pos, std::string::size_type end) {
	size_t count = 0;
	std::string::size_type i = pos;
	
	while (i < end && msg[i] != ':' && count < 14) {
		while (msg[i] != ' ' && i < end) {
			if (msg[i] == '\0' || msg[i] == '\r' || msg[i] == '\n')
				throw IllFormedMessageException();
			params[count] += msg[i];
			i++;
		}
		i++;
		count++;
	}
	if (msg[i] == ':')
		i++;
	params_count = count;
	if (i < end)
		++params_count;
	while (i < end) {
		if (msg[i] == '\0' || msg[i] == '\r' || msg[i] == '\n')
			throw IllFormedMessageException();
		params[count] += msg[i];
		i++;
	}
	return end;
}

Message & Message::operator=(const Message & rhs) {
	if (this == &rhs)
		return *this;
	msg = rhs.msg;
	return *this;
}
