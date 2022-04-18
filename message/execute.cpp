#include "../includes/ft_irc.hpp"

//TODO responses

void pass(Client & client, Message & msg) {
	if (client.registered) {
		response(replies.at(ERR_ALREADYREGISTERED));
		return ;
	}
	if (msg.get_params_count() == 0) {
		response(replies.at(ERR_NEEDMOREPARAMS), msg.get_command().c_str());
		return;
	}

	client.password = msg.get_params()[0];
	client.pass_set = true;
	client.reg();
}

void nick(Client & client, Message & msg) {
	if (msg.get_params_count() == 0) {
		response(replies.at(ERR_NEEDMOREPARAMS), msg.get_command().c_str());
		return ;
	}
	std::string nick = msg.get_params()[0]
	if (!Message::is_nickname(msg.get_params()[0])) {
		response(replies.at(ERR_ERRONEUSNICKNAME), nick.c_str());
		return ;
	}
	
	Database * db = Database::get_instance();
	client_map::iterator begin = db->get_client()->begin();
	client_map::iterator end = db->get_client()->end();
	while (begin != end) {
		if (begin->nickname == nick) {
			response(replies.at(ERR_ERRONEUSNICKNAME), nick.c_str());
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
		response(replies.at(ERR_ALREADYREGISTERED));
		return ;
	}
	if (msg.get_params_count() < 4) {
		response(replies.at(ERR_NEEDMOREPARAMS), msg.get_command().c_str());
		return ;
	}

	//TODO check username syntax (no NUL, \r, \n, " " or @) -> what do if wrong?

	client.username = msg.get_params()[0];
	int mode_param = stoi(msg.get_params()[1]);
	client.mode = mode_param & 0b1100;
	client.realname = msg.get_params()[3];
	client.user_set = true;
	client.reg();
}

void join(Client & client, Message & msg) {
	if (!client.registered) {
		reponse(replies.at(ERR_NOTREGISTERED));
		return ;
	}
	if (msg.get_params_count() == 0) {
		response(replies.at(ERR_NEEDMOREPARAMS), msg.get_command().c_str());
		return ;
	}

	Database * db = Database::get_instance();
	std::string channels = msg.get_command()[0];

	char * tok = strtok(channels.c_str(), ",");
	Channel * chan;
	while (tok) {
		chan = db.get_channel(tok);
		if (!chan)
			chan = db.add_channel(tok);
		chan.add_client(client);
		tok = strtok(NULL, ",");
	}
}

void privmsg(Client & client, Message & msg) {
	if (!client.registered) {
		reponse(replies.at(ERR_NOTREGISTERED));
		return ;
	}
	//TODO
}

void oper(Client & client, Message & msg) {
	if (!client.registered) {
		reponse(replies.at(ERR_NOTREGISTERED));
		return ;
	}
	if (msg.get_params_count() < 2) {
		response(replies.at(ERR_NEEDMOREPARAMS), msg.get_command().c_str());
		return ;
	}
	//TODO define oper account through config file or hard code.
}

void kill(Client & client, Message & msg) {
	if (!client.registered) {
		reponse(replies.at(ERR_NOTREGISTERED));
		return ;
	}
	if (!client.oper) {
		reponse(replies.at(ERR_NOPRIVILEGES));
		return ;
	}
	if (msg.get_params_count() == 0) {
		response(replies.at(ERR_NEEDMOREPARAMS), msg.get_command().c_str());
		return ;
	}
	//TODO how to disconnect user?
}

void rehash(Client & client, Message & msg) {
	if (!client.registered) {
		reponse(replies.at(ERR_NOTREGISTERED));
		return ;
	}
	if (!client.oper) {
		reponse(replies.at(ERR_NOPRIVILEGES));
		return ;
	}
	//TODO we may not have a config file
}

void restart(Client & client, Message & msg) {
	if (!client.registered) {
		reponse(replies.at(ERR_NOTREGISTERED));
		return ;
	}
	if (!client.oper) {
		reponse(replies.at(ERR_NOPRIVILEGES));
		return ;
	}
	//TODO how?
}

void die(Client & client, Message & msg) {
	if (!client.registered) {
		reponse(replies.at(ERR_NOTREGISTERED));
		return ;
	}
	if (!client.oper) {
		reponse(replies.at(ERR_NOPRIVILEGES));
		return ;
	}
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
		response(replies.at(ERR_UNKNOWNCOMMAND), msg.get_command().c_str()); //TODO
}
