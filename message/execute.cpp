#include "../includes/ft_irc.hpp"

void pass(Client & client, Message & msg) {
	if (client.registered) {
		Database::get_instance()->add_response(client.response(ERR_ALREADYREGISTERED));
		return ;
	}
	if (msg.get_params_count() == 0) {
		Database::get_instance()->add_response(client.response(ERR_NEEDMOREPARAMS, msg.get_command().c_str()));
		return;
	}

	client.password = msg.get_params()[0];
	client.pass_set = true;
	client.reg();
}

void nick(Client & client, Message & msg) {
	if (msg.get_params_count() == 0) {
		Database::get_instance()->add_response(client.response(ERR_NEEDMOREPARAMS, msg.get_command().c_str()));
		return ;
	}
	std::string nick = msg.get_params()[0];
	if (!Message::is_nickname(msg.get_params()[0])) {
		Database::get_instance()->add_response(client.response(ERR_ERRONEUSNICKNAME, nick.c_str()));
		return ;
	}
	
	Database * db = Database::get_instance();
	client_map::iterator begin = db->clients.begin();
	client_map::iterator end = db->clients.end();
	while (begin != end) {
		if (begin->second.nickname == nick) {
			Database::get_instance()->add_response(client.response(ERR_NICKNAMEINUSE, nick.c_str()));
			return ;
		}
		begin++;
	}

	client.nickname = nick;
	client.nick_set = true;
	client.reg();
}

void user(Client & client, Message & msg) {
	if (client.registered) {
		Database::get_instance()->add_response(client.response(ERR_ALREADYREGISTERED));
		return ;
	}
	if (msg.get_params_count() < 4) {
		Database::get_instance()->add_response(client.response(ERR_NEEDMOREPARAMS, msg.get_command().c_str()));
		return ;
	}

	std::string user = msg.get_params()[0];
	std::string::size_type end = user.find_first_of("\0\r\n @");
	if (end != std::string::npos)
		user = user.substr(0, end);

	client.username = user;
	client.hostname = msg.get_params()[1];
	client.servername = Database::get_instance()->hostname;
	client.realname = msg.get_params()[3];
	client.user_set = true;
	client.reg();
}

void join(Client & client, Message & msg) {
	if (!client.registered) {
		Database::get_instance()->add_response(client.response(ERR_NOTREGISTERED));
		return ;
	}
	if (msg.get_params_count() == 0) {
		Database::get_instance()->add_response(client.response(ERR_NEEDMOREPARAMS, msg.get_command().c_str()));
		return ;
	}

	Database * db = Database::get_instance();
	std::string channels = msg.get_params()[0];

	if (channels == "0") {
		channel_map::iterator begin = client.subscribed_channels.begin();
		channel_map::iterator end = client.subscribed_channels.end();
		while (begin != end) {
			begin->second.remove_client(client);
			begin++;
		}
		client.subscribed_channels.clear();
		return ;
		//TODO send part messages
	}

	char * tok = strtok(const_cast<char *>(channels.c_str()), ",");
	Channel * chan;
	while (tok) {

		if ((tok[0] != '#' && tok[0] != '&') || !tok[1]) {
			db->add_response(client.response(ERR_NOSUCHCHANNEL, tok));
			tok = strtok(NULL, ",");
			continue;
		}

		chan = db->get_channel(tok);
		if (!chan)
			chan = db->add_channel(tok);

		client_map::iterator begin = chan->subscribed_clients.begin();
		client_map::iterator end = chan->subscribed_clients.end();
		for (; begin != end; begin++) {
			db->add_response(begin->second.command(CMD_JOIN, client.nickname.c_str(), client.username.c_str(), client.hostname.c_str(), chan->name.c_str()));
		}

		chan->add_client(client);
		db->add_response(client.command(CMD_JOIN, client.nickname.c_str(), client.username.c_str(), client.hostname.c_str(), chan->name.c_str()));
		db->add_response(client.response(RPL_TOPIC, chan->name.c_str(), "No topic set"));

		begin = chan->subscribed_clients.begin();
		std::string names;
		for (; begin != end; begin++) {
			if (names.length() + begin->second.nickname.length() + chan->name.length() + 6 > 512) {
				db->add_response(client.response(RPL_NAMREPLY, chan->name.c_str(), names.c_str()));
				names.clear();
			}
			names += " " + begin->second.nickname;
		}
		if (!names.empty())
			db->add_response(client.response(RPL_NAMREPLY, chan->name.c_str(), names.c_str()));
		db->add_response(client.response(RPL_ENDOFNAMES, chan->name.c_str()));
		tok = strtok(NULL, ",");
	}
}

bool send_to_client(Client & client, Message & msg) {
	Database * db = Database::get_instance();
	client_map::iterator begin = db->clients.begin();
	client_map::iterator end = db->clients.end();
	std::string recipient = msg.get_params()[0];
	while (begin != end) {
		if (recipient == begin->second.nickname) {
			db->add_response(begin->second.command(CMD_PRIVMSG, client.nickname.c_str(), client.username.c_str(), client.hostname.c_str(), recipient.c_str(), msg.get_params()[1].c_str()));
			return true;
		}
		begin++;
	}
	return false;
}

bool send_to_channel(Client & client, Message & msg, Channel * chan) {
	Database * db = Database::get_instance();
	client_map::iterator begin = chan->subscribed_clients.begin();
	client_map::iterator end = chan->subscribed_clients.end();
	std::string recipient = msg.get_params()[0];
	while (begin != end) {
		if (begin->first != client.getSockFd())
			db->add_response(begin->second.command(CMD_PRIVMSG, client.nickname.c_str(), client.username.c_str(), client.hostname.c_str(), recipient.c_str(), msg.get_params()[1].c_str()));
		begin++;
	}
	return true;
}

void privmsg(Client & client, Message & msg) {
	if (!client.registered) {
		Database::get_instance()->add_response(client.response(ERR_NOTREGISTERED));
		return ;
	}
	if (msg.get_params_count() == 0) {
		Database::get_instance()->add_response(client.response(ERR_NORECIPIENT, msg.get_command().c_str()));
		return ;
	}
	if (msg.get_params_count() == 1) {
		Database::get_instance()->add_response(client.response(ERR_NOTEXTTOSEND, msg.get_command().c_str()));
		return ;
	}
	
	Database * db = Database::get_instance();
	Channel * chan;
	std::string recipient = msg.get_params()[0];
	if (Message::is_nickname(recipient)) {
		if (!send_to_client(client, msg))
			db->add_response(client.response(ERR_NOSUCHNICK, recipient.c_str()));
	} else if ((chan = db->get_channel(recipient))) {
		send_to_channel(client, msg, chan);
	} else if ((recipient[0] == '#' || recipient[0] == '$')
			&& recipient.find_last_of("*?") != std::string::npos) {
		if (!client.oper) {
			db->add_response(client.response(ERR_NOPRIVILEGES));
			return ;
		}

		if (recipient.find_last_of('.') < recipient.find_last_of("*?")) {
			db->add_response(client.response(ERR_WILDTOPLEVEL, recipient.c_str()));
			return ;
		} else if (recipient.find_last_of('.') == std::string::npos) {
			db->add_response(client.response(ERR_NOTOPLEVEL, recipient.c_str()));
			return ;
		}

		if (recipient[0] == '$'
				&& Message::match_wildcard(recipient.substr(1), db->hostname)) {
			client_map::iterator begin = db->clients.begin();
			client_map::iterator end = db->clients.end();
			while (begin != end) {
				if (client.getSockFd() != begin->second.getSockFd()) {
					db->add_response(begin->second.command(CMD_PRIVMSG, client.nickname.c_str(), client.username.c_str(), client.hostname.c_str(), recipient.c_str(), msg.get_params()[1].c_str()));
				}
				begin++;
			}
		} else {
			client_map::iterator begin = db->clients.begin();
			client_map::iterator end = db->clients.end();
			while (begin != end) {
				if (Message::match_wildcard(recipient, begin->second.hostname)
						&& client.getSockFd() != begin->second.getSockFd()) {
					db->add_response(begin->second.command(CMD_PRIVMSG, client.nickname.c_str(), client.username.c_str(), client.hostname.c_str(), recipient.c_str(), msg.get_params()[1].c_str()));
				}
				begin++;
			}
		}
	} else {
		db->add_response(client.response(ERR_NOSUCHNICK, recipient.c_str()));
	}
}

void notice(Client & client, Message & msg) {
	if (!client.registered) {
		return ;
	}
	if (msg.get_params_count() == 0) {
		return ;
	}
	if (msg.get_params_count() == 1) {
		return ;
	}
	
	Database * db = Database::get_instance();
	Channel * chan;
	std::string recipient = msg.get_params()[0];
	if (Message::is_nickname(recipient)) {
		send_to_client(client, msg);
	} else if ((chan = db->get_channel(recipient))) {
		send_to_channel(client, msg, chan);
	} else if ((recipient[0] == '#' || recipient[0] == '$')
			&& recipient.find_last_of("*?") != std::string::npos) {
		if (!client.oper) {
			return ;
		}

		if (recipient.find_last_of('.') < recipient.find_last_of("*?")) {
			return ;
		} else if (recipient.find_last_of('.') == std::string::npos) {
			return ;
		}

		if (recipient[0] == '$'
				&& Message::match_wildcard(recipient.substr(1), db->hostname)) {
			client_map::iterator begin = db->clients.begin();
			client_map::iterator end = db->clients.end();
			while (begin != end) {
				if (client.getSockFd() != begin->second.getSockFd()) {
					db->add_response(begin->second.command(CMD_PRIVMSG, client.nickname.c_str(), client.username.c_str(), client.hostname.c_str(), recipient.c_str(), msg.get_params()[1].c_str()));
				}
				begin++;
			}
		} else {
			client_map::iterator begin = db->clients.begin();
			client_map::iterator end = db->clients.end();
			while (begin != end) {
				if (Message::match_wildcard(recipient, begin->second.hostname)
						&& client.getSockFd() != begin->second.getSockFd()) {
					db->add_response(begin->second.command(CMD_PRIVMSG, client.nickname.c_str(), client.username.c_str(), client.hostname.c_str(), recipient.c_str(), msg.get_params()[1].c_str()));
				}
				begin++;
			}
		}
	}
}

void oper(Client & client, Message & msg) {
	if (!client.registered) {
		Database::get_instance()->add_response(client.response(ERR_NOTREGISTERED));
		return ;
	}
	if (msg.get_params_count() < 2) {
		Database::get_instance()->add_response(client.response(ERR_NEEDMOREPARAMS, msg.get_command().c_str()));
		return ;
	}

	std::string name = msg.get_params()[0];
	std::string pwd = msg.get_params()[1];
	if (name != "admin" || pwd != "password") {
		Database::get_instance()->add_response(client.response(ERR_PASSWDMISMATCH));
		return ;
	}

	client.oper = true;
	Database::get_instance()->add_response(client.response(RPL_YOUREOPER));
}

void kill(Client & client, Message & msg) {
	if (!client.registered) {
		Database::get_instance()->add_response(client.response(ERR_NOTREGISTERED));
		return ;
	}
	if (!client.oper) {
		Database::get_instance()->add_response(client.response(ERR_NOPRIVILEGES));
		return ;
	}
	if (msg.get_params_count() == 0) {
		Database::get_instance()->add_response(client.response(ERR_NEEDMOREPARAMS, msg.get_command().c_str()));
		return ;
	}
	//TODO how to disconnect user?
}

void rehash(Client & client, Message & msg) {
	if (!client.registered) {
		Database::get_instance()->add_response(client.response(ERR_NOTREGISTERED));
		return ;
	}
	if (!client.oper) {
		Database::get_instance()->add_response(client.response(ERR_NOPRIVILEGES));
		return ;
	}
	(void) msg;
	//TODO we may not have a config file -> not implement ?
}

void restart(Client & client, Message & msg) {
	if (!client.registered) {
		Database::get_instance()->add_response(client.response(ERR_NOTREGISTERED));
		return ;
	}
	if (!client.oper) {
		Database::get_instance()->add_response(client.response(ERR_NOPRIVILEGES));
		return ;
	}
	(void) msg;
	//TODO how?
}

void die(Client & client, Message & msg) {
	if (!client.registered) {
		Database::get_instance()->add_response(client.response(ERR_NOTREGISTERED));
		return ;
	}
	if (!client.oper) {
		Database::get_instance()->add_response(client.response(ERR_NOPRIVILEGES));
		return ;
	}
	(void) msg;
	exit(0);
}

void execute(Client & client, Message & msg) {
	if (!msg.is_complete())
		return; //TODO enough?

	std::string cmd = msg.get_command();

	if (cmd == "PASS")
		pass(client, msg);
	else if (cmd == "NICK")
		nick(client, msg);
	else if (cmd == "USER")
		user(client, msg);
	else if (cmd == "JOIN")
		join(client, msg);
	else if (cmd == "PRIVMSG")
		privmsg(client, msg);
	else if (cmd == "NOTICE")
		notice(client, msg);
	else if (cmd == "OPER")
		oper(client, msg);
	else if (cmd == "KILL")
		kill(client, msg);
	else if (cmd == "REHASH")
		rehash(client, msg);
	else if (cmd == "RESTART")
		restart(client, msg);
	else if (cmd == "DIE")
		die(client, msg);
	else
		Database::get_instance()->add_response(client.response(ERR_UNKNOWNCOMMAND, msg.get_command().c_str()));
}
