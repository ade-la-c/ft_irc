#include "../includes/ft_irc.hpp"

_IRCClient::_IRCClient() {
	registered = false;
	pass_set = false;
	nick_set = false;
	user_set = false;
	oper = false;
}

_IRCClient::_IRCClient(_IRCClient const & cpy) {
	password = cpy.password;
	nickname = cpy.nickname;
	username = cpy.username;
	hostname = cpy.hostname;
	servername = cpy.servername;
	registered = cpy.registered;
	pass_set = cpy.pass_set;
	nick_set = cpy.nick_set;
	user_set = cpy.user_set;
	oper = cpy.oper;
}

_IRCClient::~_IRCClient() {
}

void _IRCClient::parse_input() {
	Message msg;
	try { //TODO this is bad design, don't do this
		msg.parse_from_str(std::string(static_cast<Client *>(this)->getBuf(), 512));
		execute(*static_cast<Client *>(this), msg);
	} catch (std::exception & e) {
		std::cerr << e.what() << std::endl;
		//TODO what do
	}
}

void _IRCClient::reg() {
	if (pass_set && nick_set && user_set) {
		if (password == Database::get_instance()->password) {
			registered = true;
			Database * db = Database::get_instance();
			db->add_pclient(static_cast<Client *>(this));
			db->add_response(this->response(RPL_WELCOME, nickname.c_str(), username.c_str(), db->hostname.c_str()));
			db->add_response(this->response(RPL_YOURHOST, db->hostname.c_str(), "0.1"));
			db->add_response(this->response(RPL_CREATED, "a long time ago"));
			db->add_response(this->response(RPL_MYINFO, db->hostname.c_str(), "0.1", "none", "none"));
		} else {
			Database::get_instance()->add_response(this->response(ERR_PASSWDMISMATCH));
		}
	}
}

response_pair _IRCClient::response(int r, ...) {
	va_list arg;
	char r_text[513];
	char response[512];

	const char * nick = "*";
	if (!this->nickname.empty())
		nick = this->nickname.c_str();
	snprintf(response, 512, ":%s %03d %s ", Database::get_instance()->hostname.c_str(), r, nick);

	va_start(arg, r);
	vsnprintf(r_text, 513, replies.at(r).c_str(), arg);
	va_end(arg);

	strncat(response, r_text, 512);

	return response_pair(static_cast<Client *>(this), std::string(response));
}

response_pair _IRCClient::command(int r, ...) {
	va_list arg;
	char response[513];

	va_start(arg, r);
	vsnprintf(response, 513, replies.at(r).c_str(), arg);
	va_end(arg);

	return response_pair(static_cast<Client *>(this), std::string(response));
}
