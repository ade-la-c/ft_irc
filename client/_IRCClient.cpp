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
		Client * c = static_cast<Client *>(this);
		char * buf = c->getBuf();
		if (Database::get_instance()->debug)
			std::cerr << "<- " << this->nickname << ": [" << std::string(buf) << "]" << std::endl;
		if (msg.parse_from_str(c->buffer + std::string(buf))) {
			execute(*c, msg);
			while (msg.parse())
				execute(*c, msg);
		}
		c->buffer.clear();
		if (!msg.is_complete())
			c->buffer = msg.get_msg();
		bzero(buf, 512);
	} catch (std::exception & e) {
		std::cerr << "parse input: " <<  e.what() << std::endl;
	}
}

void _IRCClient::reg() {
	if (pass_set && nick_set && user_set) {
		if (password == Database::get_instance()->password) {
			Database * db = Database::get_instance();
			if (db->pclients.count(nickname)) {
				this->response(ERR_NICKNAMEINUSE, nickname.c_str());
				return ;
			}
			registered = true;
			db->add_pclient(static_cast<Client *>(this));
			this->response(RPL_WELCOME, nickname.c_str(), username.c_str(), hostname.c_str());
			this->response(RPL_YOURHOST, db->hostname.c_str(), "0.1");
			this->response(RPL_CREATED, "a long time ago");
			this->response(RPL_MYINFO, hostname.c_str(), "0.1", "none", "none");
		} else {
			this->response(ERR_PASSWDMISMATCH);
		}
	}
}

void _IRCClient::response(int r, ...) {
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

	if (Database::get_instance()->debug)
		std::cerr << "-> " << this->nickname << ": [" << response << "]" << std::endl;
	responses.push_back(std::string(response));
}

void _IRCClient::command(int r, ...) {
	va_list arg;
	char response[513];

	va_start(arg, r);
	vsnprintf(response, 513, replies.at(r).c_str(), arg);
	va_end(arg);

	if (Database::get_instance()->debug)
		std::cerr << "-> " << this->nickname << ": [" << response << "]" << std::endl;
	responses.push_back(std::string(response));
}

bool _IRCClient::should_send() const {
	return !responses.empty();
}

std::string _IRCClient::response() const {
	if (responses.empty())
		throw NoResponseException();
	return responses.front();
}

void _IRCClient::sent_bytes(ssize_t bytes) {
	std::string & r = responses.front();
	if (bytes < static_cast<ssize_t>(r.length()))
		r = r.substr(bytes);
	else
		responses.pop_front();
}
