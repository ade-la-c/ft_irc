#include "../includes/ft_irc.hpp"

//TODO responses

void pass(Client & client, Message & msg) {
	if (client.registered) {
		response(ERR_ALREADYREGISTERED);
		return ;
	}
	if (msg.get_params_count() == 0) {
		response(ERR_NEEDMOREPARAMS, msg.get_command().c_str());
		return;
	}

	client.password = msg.get_params()[0];
	client.pass_set = true;
	client.reg();
}

void nick(Client & client, Message & msg) {
	if (msg.get_params_count() == 0) {
		response(ERR_NEEDMOREPARAMS, msg.get_command().c_str());
		return ;
	}
	std::string nick = msg.get_params()[0];
	if (!Message::is_nickname(msg.get_params()[0])) {
		response(ERR_ERRONEUSNICKNAME, nick.c_str());
		return ;
	}
	
	Database * db = Database::get_instance();
	client_map::iterator begin = db->clients.begin();
	client_map::iterator end = db->clients.end();
	while (begin != end) {
		if (begin->second.nickname == nick) {
			response(ERR_ERRONEUSNICKNAME, nick.c_str());
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
		response(ERR_ALREADYREGISTERED);
		return ;
	}
	if (msg.get_params_count() < 4) {
		response(ERR_NEEDMOREPARAMS, msg.get_command().c_str());
		return ;
	}

	std::string user = msg.get_params()[0];
	std::string::size_type end = user.find_first_of("\0\r\n @");
	if (end != std::string::npos)
		user = user.substr(0, end);

	client.username = user;
	int mode_param = atoi(msg.get_params()[1].c_str());
	client.mode = mode_param & 0b1100;
	client.realname = msg.get_params()[3];
	client.user_set = true;
	client.reg();
}

void join(Client & client, Message & msg) {
	if (!client.registered) {
		response(ERR_NOTREGISTERED);
		return ;
	}
	if (msg.get_params_count() == 0) {
		response(ERR_NEEDMOREPARAMS, msg.get_command().c_str());
		return ;
	}

	//TODO many replies missing

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
	}

	char * tok = strtok(const_cast<char *>(channels.c_str()), ",");
	Channel * chan;
	while (tok) {
		chan = db->get_channel(tok);
		if (!chan)
			chan = db->add_channel(tok);
		chan->add_client(client);
		tok = strtok(NULL, ",");
	}
}

void privmsg(Client & client, Message & msg) {
	if (!client.registered) {
		response(ERR_NOTREGISTERED);
		return ;
	}
	(void) msg;
	//TODO
}

void notice(Client & client, Message & msg) {
	if (!client.registered) {
		response(ERR_NOTREGISTERED);
		return ;
	}
	(void) msg;
	//TODO
}

void oper(Client & client, Message & msg) {
	if (!client.registered) {
		response(ERR_NOTREGISTERED);
		return ;
	}
	if (msg.get_params_count() < 2) {
		response(ERR_NEEDMOREPARAMS, msg.get_command().c_str());
		return ;
	}

	std::string name = msg.get_params()[0];
	std::string pwd = msg.get_params()[1];
	if (name != "admin" || pwd != "password") {
		response(ERR_PASSWDMISMATCH);
		return ;
	}

	client.oper = true;
	response(RPL_YOUREOPER);
}

void kill(Client & client, Message & msg) {
	if (!client.registered) {
		response(ERR_NOTREGISTERED);
		return ;
	}
	if (!client.oper) {
		response(ERR_NOPRIVILEGES);
		return ;
	}
	if (msg.get_params_count() == 0) {
		response(ERR_NEEDMOREPARAMS, msg.get_command().c_str());
		return ;
	}
	//TODO how to disconnect user?
}

void rehash(Client & client, Message & msg) {
	if (!client.registered) {
		response(ERR_NOTREGISTERED);
		return ;
	}
	if (!client.oper) {
		response(ERR_NOPRIVILEGES);
		return ;
	}
	(void) msg;
	//TODO we may not have a config file -> not implement ?
}

void restart(Client & client, Message & msg) {
	if (!client.registered) {
		response(ERR_NOTREGISTERED);
		return ;
	}
	if (!client.oper) {
		response(ERR_NOPRIVILEGES);
		return ;
	}
	(void) msg;
	//TODO how?
}

void die(Client & client, Message & msg) {
	if (!client.registered) {
		response(ERR_NOTREGISTERED);
		return ;
	}
	if (!client.oper) {
		response(ERR_NOPRIVILEGES);
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
		response(ERR_UNKNOWNCOMMAND, msg.get_command().c_str()); //TODO
}
