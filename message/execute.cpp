#include "../includes/ft_irc.hpp"

void pass(Client & client, Message & msg) {
	Database * db = Database::get_instance();
	if (client.registered) {
		client.response(ERR_ALREADYREGISTERED);
		return ;
	}
	if (msg.get_params_count() == 0) {
		client.response(ERR_NEEDMOREPARAMS, msg.get_command().c_str());
		return;
	}

	client.password = msg.get_params()[0];
	client.pass_set = true;
	client.reg();
}

void nick(Client & client, Message & msg) {
	Database * db = Database::get_instance();
	if (msg.get_params_count() == 0) {
		client.response(ERR_NEEDMOREPARAMS, msg.get_command().c_str());
		return ;
	}
	std::string nick = msg.get_params()[0];
	if (!Message::is_nickname(msg.get_params()[0])) {
		client.response(ERR_ERRONEUSNICKNAME, nick.c_str());
		return ;
	}
	
	if (db->pclients.count(nick)) {
		client.response(ERR_NICKNAMEINUSE, nick.c_str());
		return ;
	}

	client.nickname = nick;
	client.nick_set = true;
	client.reg();
}

void user(Client & client, Message & msg) {
	Database * db = Database::get_instance();
	if (client.registered) {
		client.response(ERR_ALREADYREGISTERED);
		return ;
	}
	if (msg.get_params_count() < 4) {
		client.response(ERR_NEEDMOREPARAMS, msg.get_command().c_str());
		return ;
	}

	std::string user = msg.get_params()[0];
	std::string::size_type end = user.find_first_of("\0\r\n @");
	if (end != std::string::npos)
		user = user.substr(0, end);

	client.username = user;
	client.hostname = msg.get_params()[1];
	client.servername = db->hostname;
	client.realname = msg.get_params()[3];
	client.user_set = true;
	client.reg();
}

void join(Client & client, Message & msg) {
	Database * db = Database::get_instance();
	if (!client.registered) {
		client.response(ERR_NOTREGISTERED);
		return ;
	}
	if (msg.get_params_count() == 0) {
		client.response(ERR_NEEDMOREPARAMS, msg.get_command().c_str());
		return ;
	}

	std::string channels = msg.get_params()[0];

	if (channels == "0") {
		pchannel_map::iterator begin = client.subscribed_channels.begin();
		pchannel_map::iterator end = client.subscribed_channels.end();
		pclient_map::iterator clbegin;
		pclient_map::iterator clend;
		while (begin != end) {
			clbegin = begin->second->subscribed_clients.begin();
			clend = begin->second->subscribed_clients.end();
			while (clbegin != clend) {
				clbegin->second->command(CMD_PART, client.nickname.c_str(), client.username.c_str(), client.hostname.c_str(), begin->second->name.c_str());
				clbegin++;
			}
			begin->second->remove_client(client);
			if (begin->second->empty())
				db->remove_channel(begin->second);
			begin++;
		}
		client.subscribed_channels.clear();
		return ;
	}

	char * tok = strtok(const_cast<char *>(channels.c_str()), ",");
	Channel * chan;
	while (tok) {

		if ((tok[0] != '#' && tok[0] != '&') || !tok[1]) {
			client.response(ERR_NOSUCHCHANNEL, tok);
			tok = strtok(NULL, ",");
			continue;
		}

		chan = db->get_channel(tok);
		if (!chan)
			chan = db->add_channel(tok);

		if (chan->subscribed_clients.count(client.nickname)) {
			tok = strtok(NULL, ",");
			continue;
		}

		pclient_map::iterator begin = chan->subscribed_clients.begin();
		pclient_map::iterator end = chan->subscribed_clients.end();
		for (; begin != end; begin++) {
			begin->second->command(CMD_JOIN, client.nickname.c_str(), client.username.c_str(), client.hostname.c_str(), chan->name.c_str());
		}

		client.subscribed_channels[chan->name] = chan;
		chan->add_client(client);
		client.command(CMD_JOIN, client.nickname.c_str(), client.username.c_str(), client.hostname.c_str(), chan->name.c_str());
		client.response(RPL_TOPIC, chan->name.c_str(), "No topic set");

		begin = chan->subscribed_clients.begin();
		std::string names;
		for (; begin != end; begin++) {
			if (names.length() + begin->second->nickname.length() + chan->name.length() + 6 > 512) {
				client.response(RPL_NAMREPLY, chan->name.c_str(), names.c_str());
				names.clear();
			}
			names += " " + begin->second->nickname;
		}
		if (!names.empty())
			client.response(RPL_NAMREPLY, chan->name.c_str(), names.c_str());
		client.response(RPL_ENDOFNAMES, chan->name.c_str());
		tok = strtok(NULL, ",");
	}
}

bool send_to_channel(Client & client, Message & msg, Channel * chan) {
	Database * db = Database::get_instance();
	pclient_map::iterator begin = chan->subscribed_clients.begin();
	pclient_map::iterator end = chan->subscribed_clients.end();
	std::string recipient = msg.get_params()[0];
	while (begin != end) {
		if (begin->second->getSockFd() != client.getSockFd())
			begin->second->command(CMD_PRIVMSG, client.nickname.c_str(), client.username.c_str(), client.hostname.c_str(), recipient.c_str(), msg.get_params()[1].c_str());
		begin++;
	}
	return true;
}

void privmsg(Client & client, Message & msg) {
	if (!client.registered) {
		client.response(ERR_NOTREGISTERED);
		return ;
	}
	if (msg.get_params_count() == 0) {
		client.response(ERR_NORECIPIENT, msg.get_command().c_str());
		return ;
	}
	if (msg.get_params_count() == 1) {
		client.response(ERR_NOTEXTTOSEND, msg.get_command().c_str());
		return ;
	}
	
	Database * db = Database::get_instance();
	Channel * chan;
	std::string recipient = msg.get_params()[0];
	if (Message::is_nickname(recipient)) {
		pclient_map::iterator cl = db->pclients.find(recipient);
		if (cl == db->pclients.end())
			client.response(ERR_NOSUCHNICK, recipient.c_str());
		else
			cl->second->command(CMD_PRIVMSG, client.nickname.c_str(), client.username.c_str(), client.hostname.c_str(), recipient.c_str(), msg.get_params()[1].c_str());
	} else if ((chan = db->get_channel(recipient))) {
		send_to_channel(client, msg, chan);
	} else if ((recipient[0] == '#' || recipient[0] == '$')
			&& recipient.find_last_of("*?") != std::string::npos) {
		if (!client.oper) {
			client.response(ERR_NOPRIVILEGES);
			return ;
		}

		if (recipient.find_last_of('.') < recipient.find_last_of("*?")) {
			client.response(ERR_WILDTOPLEVEL, recipient.c_str());
			return ;
		} else if (recipient.find_last_of('.') == std::string::npos) {
			client.response(ERR_NOTOPLEVEL, recipient.c_str());
			return ;
		}

		if (recipient[0] == '$'
				&& Message::match_wildcard(recipient.substr(1), db->hostname)) {
			pclient_map::iterator begin = db->pclients.begin();
			pclient_map::iterator end = db->pclients.end();
			while (begin != end) {
				if (client.getSockFd() != begin->second->getSockFd()) {
					begin->second->command(CMD_PRIVMSG, client.nickname.c_str(), client.username.c_str(), client.hostname.c_str(), recipient.c_str(), msg.get_params()[1].c_str());
				}
				begin++;
			}
		} else {
			pclient_map::iterator begin = db->pclients.begin();
			pclient_map::iterator end = db->pclients.end();
			while (begin != end) {
				if (Message::match_wildcard(recipient, begin->second->hostname)
						&& client.getSockFd() != begin->second->getSockFd()) {
					begin->second->command(CMD_PRIVMSG, client.nickname.c_str(), client.username.c_str(), client.hostname.c_str(), recipient.c_str(), msg.get_params()[1].c_str());
				}
				begin++;
			}
		}
	} else {
		client.response(ERR_NOSUCHNICK, recipient.c_str());
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
		pclient_map::iterator cl = db->pclients.find(recipient);
		if (cl != db->pclients.end())
			cl->second->command(CMD_PRIVMSG, client.nickname.c_str(), client.username.c_str(), client.hostname.c_str(), recipient.c_str(), msg.get_params()[1].c_str());
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
			pclient_map::iterator begin = db->pclients.begin();
			pclient_map::iterator end = db->pclients.end();
			while (begin != end) {
				if (client.getSockFd() != begin->second->getSockFd()) {
					begin->second->command(CMD_PRIVMSG, client.nickname.c_str(), client.username.c_str(), client.hostname.c_str(), recipient.c_str(), msg.get_params()[1].c_str());
				}
				begin++;
			}
		} else {
			pclient_map::iterator begin = db->pclients.begin();
			pclient_map::iterator end = db->pclients.end();
			while (begin != end) {
				if (Message::match_wildcard(recipient, begin->second->hostname)
						&& client.getSockFd() != begin->second->getSockFd()) {
					begin->second->command(CMD_PRIVMSG, client.nickname.c_str(), client.username.c_str(), client.hostname.c_str(), recipient.c_str(), msg.get_params()[1].c_str());
				}
				begin++;
			}
		}
	}
}

void oper(Client & client, Message & msg) {
	if (!client.registered) {
		client.response(ERR_NOTREGISTERED);
		return ;
	}
	if (msg.get_params_count() < 2) {
		client.response(ERR_NEEDMOREPARAMS, msg.get_command().c_str());
		return ;
	}

	std::string name = msg.get_params()[0];
	std::string pwd = msg.get_params()[1];
	if (name != "admin" || pwd != "password") {
		client.response(ERR_PASSWDMISMATCH);
		return ;
	}

	client.oper = true;
	client.response(RPL_YOUREOPER);
}

void kill(Client & client, Message & msg) {
	if (!client.registered) {
		client.response(ERR_NOTREGISTERED);
		return ;
	}
	if (!client.oper) {
		client.response(ERR_NOPRIVILEGES);
		return ;
	}
	if (msg.get_params_count() == 0) {
		client.response(ERR_NEEDMOREPARAMS, msg.get_command().c_str());
		return ;
	}
	//TODO how to disconnect user?
}

void rehash(Client & client, Message & msg) {
	if (!client.registered) {
		client.response(ERR_NOTREGISTERED);
		return ;
	}
	if (!client.oper) {
		client.response(ERR_NOPRIVILEGES);
		return ;
	}
	(void) msg;
	//TODO we may not have a config file -> not implement ?
}

void restart(Client & client, Message & msg) {
	if (!client.registered) {
		client.response(ERR_NOTREGISTERED);
		return ;
	}
	if (!client.oper) {
		client.response(ERR_NOPRIVILEGES);
		return ;
	}
	(void) msg;
	//TODO how?
}

void die(Client & client, Message & msg) {
	if (!client.registered) {
		client.response(ERR_NOTREGISTERED);
		return ;
	}
	if (!client.oper) {
		client.response(ERR_NOPRIVILEGES);
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
		client.response(ERR_UNKNOWNCOMMAND, msg.get_command().c_str());
}
