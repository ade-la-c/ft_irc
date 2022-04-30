#include "../includes/ft_irc.hpp"

_IRCClient::_IRCClient() {
	registered = false;
	pass_set = false;
	nick_set = false;
	user_set = false;
	mode = 0;
	oper = false;
}

_IRCClient::_IRCClient(_IRCClient const & cpy) {
	password = cpy.password;
	nickname = cpy.nickname;
	username = cpy.username;
	registered = cpy.registered;
	pass_set = cpy.pass_set;
	nick_set = cpy.nick_set;
	user_set = cpy.user_set;
	mode = cpy.mode;
	oper = cpy.oper;
}

_IRCClient::~_IRCClient() {
}

void _IRCClient::parse_input() {
	Message msg;
		msg.parse_from_str(std::string(static_cast<Client *>(this)->getBuf(), 512));
	try {
	} catch (std::exception & e) {
		//TODO what do
	}
	execute(*static_cast<Client *>(this), msg);
}

void _IRCClient::reg() {
	if (pass_set && nick_set && user_set) {
		if (password == Database::get_instance()->password) {
			registered = true;
			Database::get_instance()->add_response(this->response(RPL_WELCOME, nickname.c_str(), username.c_str(), Database::get_instance()->hostname.c_str()));
			Database::get_instance()->add_response(this->response(RPL_YOURHOST, Database::get_instance()->hostname.c_str(), "0.1"));
			Database::get_instance()->add_response(this->response(RPL_CREATED, "a long time ago"));
			Database::get_instance()->add_response(this->response(RPL_MYINFO, Database::get_instance()->hostname.c_str(), "0.1", "none", "none"));
		}
	}
}

response_pair _IRCClient::response(int r, ...) {
	va_list arg;
	char r_text[512];
	char response[512];

	char * nick = "*";
	if (!this->nickname.empty())
		nick = this->nickname.c_str();
	sprintf(response, ":%s %03d %s ", Database::get_instance()->hostname, r, nick);

	va_start(arg, r);
	vsprintf(r_text, replies.at(r).c_str(), arg);
	va_end(arg);

	strcat(response, r_text);

	return response_pair(static_cast<Client *>(this), response);
}

response_pair _IRCClient::command(int r, ...) {
	va_list arg;
	char response[512];

	va_start(arg, r);
	vsprintf(response, replies.at(r).c_str(), arg);
	va_end(arg);

	return response_pair(static_cast<Client *>(this), response);
}
