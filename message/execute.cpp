#include "../includes/ft_irc.hpp"

void pass(Client & client, Message & msg) {
	
}

void execute(Client & client, Message & msg) {
	std::string & cmd = msg.get_command();

	switch (cmd) {
		case "PASS":
			pass(client, msg);
		case "NICK":
		case "USER":
		case "JOIN":
		case "PRIVMSG":
		case "OPER":
		case "KILL":
		case "REHASH":
		case "RESTART":
		case "DIE":
		default:
	}
}
