#ifndef REPLIES_HPP
# define REPLIES_HPP

# define RPL_WELCOME			1
# define RPL_YOURHOST			2
# define RPL_CREATED			3
# define RPL_MYINFO				4
# define RPL_AWAY				301
# define RPL_TOPIC				332
# define RPL_NAMREPLY			353
# define RPL_ENDOFNAMES			366
# define RPL_YOUREOPER			381
# define RPL_REHASHING			382
# define ERR_NOSUCHNICK			401
# define ERR_NOSUCHSERVER		402
# define ERR_NOSUCHCHANNEL		403
# define ERR_CANNOTSENDTOCHAN	404
# define ERR_TOOMANYCHANNELS	405
# define ERR_TOOMANYTARGETS		407
# define ERR_NORECIPIENT		411
# define ERR_NOTEXTTOSEND		412
# define ERR_NOTOPLEVEL			413
# define ERR_WILDTOPLEVEL		414
# define ERR_UNKNOWNCOMMAND		421
# define ERR_NONICKNAMEGIVEN	431
# define ERR_ERRONEUSNICKNAME	432
# define ERR_NICKNAMEINUSE		433
# define ERR_NICKCOLLISION		436
# define ERR_UNAVAILRESOURCE	437
# define ERR_NOTONCHANNEL		442
# define ERR_NOTREGISTERED		451
# define ERR_NEEDMOREPARAMS		461
# define ERR_ALREADYREGISTERED	462
# define ERR_PASSWDMISMATCH		464
# define ERR_CHANNELISFULL		471
# define ERR_INVITEONLYCHAN		473
# define ERR_BANNEDFROMCHAN		474
# define ERR_BADCHANNELKEY		475
# define ERR_BADCHANMASK		476
# define ERR_NOPRIVILEGES		481
# define ERR_CANTKILLSERVER		483
# define ERR_NOOPERHOST			491
# define CMD_JOIN				1001
# define CMD_PRIVMSG			1002
# define CMD_PART				1003
# define CMD_QUIT				1004

typedef std::pair<uint16_t, std::string> reply;
reply const _rpl_arr[] = {
	reply(RPL_WELCOME, ":Welcome to the Internet Relay Network %s!%s@%s\r\n"), //"Welcome to the Internet Relay Network <nick>!<user>@<host>"
	reply(RPL_YOURHOST, ":Your host is %s, running version %s\r\n"), //"Your host is <servername>, running version <ver>"
	reply(RPL_CREATED, ":This server was created %s\r\n"), //"This server was created <date>"
	reply(RPL_MYINFO, "%s %s %s %s\r\n"), //"<servername> <version> <available user modes> <available channel modes>"
	reply(RPL_AWAY, "%s :%s\r\n"), //"<nick> :<away message>"
	reply(RPL_TOPIC, "%s :%s\r\n"), //"<channel> :<topic>"
	reply(RPL_NAMREPLY, "= %s :%s\r\n"), //"<channel> :<names>"
	reply(RPL_ENDOFNAMES, "%s :End of NAMES list\r\n"), //"<channel> :End of NAMES list>"
	reply(RPL_YOUREOPER, ":You are now an IRC operator\r\n"), //":You are now an IRC operator"
	reply(RPL_REHASHING, "%s :Rehashing\r\n"), //"<config file> :Rehashing"
	reply(ERR_NOSUCHNICK, "%s :No such nick/channel\r\n"), //"<nickname> :No such nick/channel"
	reply(ERR_NOSUCHSERVER, "%s :No such server\r\n"), //"<server name> :No such server"
	reply(ERR_NOSUCHCHANNEL, "%s :No such channel\r\n"), //"<channel name> :No such channel"
	reply(ERR_CANNOTSENDTOCHAN, "%s :Cannot send to channel\r\n" ), //"<channel name> :Cannot send to channel"
	reply(ERR_TOOMANYCHANNELS, "%s :You have joined too many channels\r\n"), //"<channel name> :You have joined too many channels"
	reply(ERR_TOOMANYTARGETS, "%s :%s recipients. %s\r\n"), //"<target> :<error code> recipients. <abort message>"
	reply(ERR_NORECIPIENT, ":No recipient given (%s)\r\n"), //":No recipient given (<command>)"
	reply(ERR_NOTEXTTOSEND, ":No text to send\r\n"), //":No text to send"
	reply(ERR_NOTOPLEVEL, "%s :No toplevel domain specified\r\n"), //"<mask> :No toplevel domain specified"
	reply(ERR_WILDTOPLEVEL, "%s :Wildcard in toplevel domain\r\n"), //"<mask> :Wildcard in toplevel domain"
	reply(ERR_UNKNOWNCOMMAND, "%s :Unknown command\r\n"), //"<command> :Unknown command"
	reply(ERR_NONICKNAMEGIVEN, ":No nickname given\r\n"), //":No nickname given"
	reply(ERR_ERRONEUSNICKNAME, "%s :Erroneous nickname\r\n"), //"<nick> :Erroneus nickname"
	reply(ERR_NICKNAMEINUSE, "%s :Nickname is already in use\r\n"), //"<nick> :Nickname is already in use"
	reply(ERR_NICKCOLLISION, "%s :Nickname collision KILL\r\n"), //"<nick> :Nickname collision KILL"
	reply(ERR_UNAVAILRESOURCE, "%s :Nick/channel is temporarily unavailable\r\n"), //"<nick/channel> :Nick/channel is temporarily unavailable"
	reply(ERR_NOTONCHANNEL, "%s :You're not on that channel\r\n"), //"<channel> :You're not on that channel"
	reply(ERR_NOTREGISTERED, ":You have not registered:\r\n"), //":You have not registered"
	reply(ERR_NEEDMOREPARAMS, "%s :Not enough parameters\r\n"), //"<command> :Not enough parameters"
	reply(ERR_ALREADYREGISTERED, ":You may not reregister\r\n"), //":You may not reregister"
	reply(ERR_PASSWDMISMATCH, ":Password incorrect\r\n"), //":Password incorrect"
	reply(ERR_CHANNELISFULL, "%s :Cannot join channel (+l)\r\n"), //"<channel> :Cannot join channel (+l)"
	reply(ERR_INVITEONLYCHAN, "%s :Cannot join channel (+i)\r\n"), //"<channel> :Cannot join channel (+i)"
	reply(ERR_BANNEDFROMCHAN, "%s :Cannot join channel (+b)\r\n"), //"<channel> :Cannot join channel (+b)"
	reply(ERR_BADCHANNELKEY, "%s :Cannot join channel (+i)\r\n"), //"<channel> :Cannot join channel (+i)"
	reply(ERR_BADCHANMASK, "%s :Bad Channel Mask\r\n"), //"<channel> :Bad Channel Mask"
	reply(ERR_NOPRIVILEGES, ":Permission Denied- You're not an IRC operator\r\n"), //":Permission Denied- You're not an IRC operator"
	reply(ERR_CANTKILLSERVER, ":You cant kill a server!\r\n"), //":You cant kill a server!"
	reply(ERR_NOOPERHOST, ":No O-lines for your host\r\n"), //":No O-lines for your host"
	reply(CMD_JOIN, ":%s!%s@%s JOIN %s\r\n"), //":<nick>!<user>@<host> JOIN <channel>"
	reply(CMD_PRIVMSG, ":%s!%s@%s PRIVMSG %s :%s\r\n"), //":<nick>!<user>@<host> PRIVMSG <recipient> <message>"
	reply(CMD_PART, ":%s!%s@%s PART %s\r\n"), //":<nick>!<user>@<host> PART <channel>"
	reply(CMD_QUIT, ":%s!%s@%s QUIT %s\r\n"), //":<nick>!<user>@<host> QUIT [<message>]"
};

std::map<uint16_t, std::string> const replies(_rpl_arr, _rpl_arr + sizeof(_rpl_arr) / sizeof(_rpl_arr[0]));

#endif
