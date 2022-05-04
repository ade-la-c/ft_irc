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

bool Message::parse_from_str(std::string const & msg) {
	this->msg = msg;
	return parse();
}

bool Message::parse() {
	std::string::size_type pos = 0;
	std::string::size_type end;

	msg = buffer + msg;
	buffer.clear();
	end = msg.find("\r\n");

	if (end > MAX_SIZE - 2 && end != std::string::npos) {
		msg = msg.substr(MAX_SIZE);
		return this->parse();
	} else if (end == std::string::npos) {
		buffer = msg;
		return false;
	} else if (!end) {
		return false;
	}

	complete = true;
	buffer = msg.substr(end + 2, msg.find('\0'));

	if (msg[0] == ':') {
		pos = parse_prefix(end);
		pos++;
	}

	pos = parse_command(pos, end);

	if (pos != end) {
		pos++;
		pos = parse_params(pos, end);
	} else {
		params_count = 0;
	}

	msg.clear();

	return true;
}

bool Message::is_complete() {
	return complete;
}

std::string Message::get_msg() {
	return msg;
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

bool Message::match_wildcard(std::string const & pattern, std::string const & str) {
	std::string::size_type i = 0, j = 0, k = 0;

	while (i < pattern.length() && j < str.length()) {
		if (pattern[i] == '*') {
			while (pattern[i] == '*' || pattern[i] == '?')
				i++;
			k = str.find_last_of(pattern[i]);
			while (k != std::string::npos && k >= j) {
				if (match_wildcard(pattern.substr(i), str.substr(k)))
					return true;
				k = str.find_last_of(pattern[i], k - 1);
			}
			return false;
		} else if (pattern[i] != str[j] && pattern[i] != '?') {
			return false;
		}

		i++;
		j++;
	}

	if (i == pattern.length() && j == str.length())
		return true;
	else
		return false;
}

std::string::size_type Message::parse_prefix(std::string::size_type end) {
	std::string::size_type sp;

	sp = this->msg.find(" ");
	//TODO just ignore prefix anyway? -> never send errors
	if (sp >= end)
		throw IllFormedMessageException();
	prefix = this->msg.substr(1, sp - 1);
	return sp;
}

std::string::size_type Message::parse_command(std::string::size_type pos, std::string::size_type end) {
	std::string::size_type sp;

	sp = this->msg.find(" ", pos);
	if (sp > end)
		sp = end;
	command = msg.substr(pos, sp - pos);
	return sp;
}

std::string::size_type Message::parse_params(std::string::size_type pos, std::string::size_type end) {
	std::string::size_type count = 0;
	std::string::size_type sp;

	sp = this->msg.find(" ", pos);
	
	while (pos < end && msg[pos] != ':' && count < 14) {
		if (sp > end)
			sp = end;
		params[count] = msg.substr(pos, sp - pos);
		count++;
		pos = sp + 1;
		while (msg[pos] == ' ')
			pos++;
		sp = this->msg.find(" ", pos);
	}

	if (msg[pos] == ':')
		pos++;
	params_count = count;
	if (pos < end){
		++params_count;
		params[count] = msg.substr(pos, end - pos);
	}
	return end;
}

Message & Message::operator=(const Message & rhs) {
	if (this == &rhs)
		return *this;
	msg = rhs.msg;
	return *this;
}
