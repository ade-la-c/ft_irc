#include "../includes/ft_irc.hpp"

void pass(Client & client, Message & msg) {
	if (msg.get_params_count() == 0)
		response(replies[ERR_NEEDMOREPARAMS], msg.command); //TODO
	if (client.registered)
		response(replies[ERR_ALREADYREGISTERED]); //TODO
	client.password = msg.get_params[0];
}

void nick(Client & client, Message & msg) {
	if (msg.get_params_count() == 0)
		response(replies[ERR_NEEDMOREPARAMS], msg.command); //TODO
	if (!
}

void user(Client & client, Message & msg) {
}

void join(Client & client, Message & msg) {
}

void privmsg(Client & client, Message & msg) {
}

void oper(Client & client, Message & msg) {
}

void kill(Client & client, Message & msg) {
}

void rehash(Client & client, Message & msg) {
}

void restart(Client & client, Message & msg) {
}

void die(Client & client, Message & msg) {
}

void execute(Client & client, Message & msg) {
	if (!msg.is_complete())
		return; //TODO enough?

	std::string & cmd = msg.get_command();

	switch (cmd) {
		case "PASS":
			pass(client, msg);
		case "NICK":
			nick(client, msg);
		case "USER":
			user(client, msg);
		case "JOIN":
			join(client, msg);
		case "PRIVMSG":
			privmsg(client, msg);
		case "OPER":
			oper(client, msg);
		case "KILL":
			kill(client, msg);
		case "REHASH":
			rehash(client, msg);
		case "RESTART":
			restart(client, msg);
		case "DIE":
			die(client, msg);
		default:
			return ;
			//TODO ERR_UNKNOWNCOMMAND
	}
}
