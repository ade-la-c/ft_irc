#ifndef REPLIES_HPP
# define REPLIES_HPP

# define RPL_AWAY				301
# define RPL_TOPIC				332
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

typedef std::pair<uint16_t, std::string> reply;
reply const _rpl_arr[] = {
	reply(RPL_AWAY, "%s :%s"), //"<nick> :<away message>"
	reply(RPL_TOPIC, "%s :%s"), //"<channel> :<topic>"
	reply(RPL_YOUREOPER, ":You are now an IRC operator"), //":You are now an IRC operator"
	reply(RPL_REHASHING, "%s :Rehashing"), //"<config file> :Rehashing"
	reply(ERR_NOSUCHNICK, "%s :No such nick/channel"), //"<nickname> :No such nick/channel"
	reply(ERR_NOSUCHSERVER, "%s :No such server"), //"<server name> :No such server"
	reply(ERR_NOSUCHCHANNEL, "%s :No such channel"), //"<channel name> :No such channel"
	reply(ERR_CANNOTSENDTOCHAN, "%s :Cannot send to channel" ), //"<channel name> :Cannot send to channel"
	reply(ERR_TOOMANYCHANNELS, "%s :You have joined too many channels"), //"<channel name> :You have joined too many channels"
	reply(ERR_TOOMANYTARGETS, "%s :%s recipients. %s"), //"<target> :<error code> recipients. <abort message>"
	reply(ERR_NORECIPIENT, ":No recipient given (%s)"), //":No recipient given (<command>)"
	reply(ERR_NOTEXTTOSEND, ":No text to send"), //":No text to send"
	reply(ERR_NOTOPLEVEL, "%s :No toplevel domain specified"), //"<mask> :No toplevel domain specified"
	reply(ERR_WILDTOPLEVEL, "%s :Wildcard in toplevel domain"), //"<mask> :Wildcard in toplevel domain"
	reply(ERR_UNKNOWNCOMMAND, "%s :Unknown command"), //"<command> :Unknown command"
	reply(ERR_NONICKNAMEGIVEN, ":No nickname given"), //":No nickname given"
	reply(ERR_ERRONEUSNICKNAME, "%s :Erroneous nickname"), //"<nick> :Erroneus nickname"
	reply(ERR_NICKNAMEINUSE, "%s :Nickname is already in use"), //"<nick> :Nickname is already in use"
	reply(ERR_NICKCOLLISION, "%s :Nickname collision KILL"), //"<nick> :Nickname collision KILL"
	reply(ERR_UNAVAILRESOURCE, "%s :Nick/channel is temporarily unavailable"), //"<nick/channel> :Nick/channel is temporarily unavailable"
	reply(ERR_NOTREGISTERED, ":You have not registered:"), //":You have not registered"
	reply(ERR_NEEDMOREPARAMS, "%s :Not enough parameters"), //"<command> :Not enough parameters"
	reply(ERR_ALREADYREGISTERED, ":You may not reregister"), //":You may not reregister"
	reply(ERR_PASSWDMISMATCH, ":Password incorrect"), //":Password incorrect"
	reply(ERR_CHANNELISFULL, "%s :Cannot join channel (+l)"), //"<channel> :Cannot join channel (+l)"
	reply(ERR_INVITEONLYCHAN, "%s :Cannot join channel (+i)"), //"<channel> :Cannot join channel (+i)"
	reply(ERR_BANNEDFROMCHAN, "%s :Cannot join channel (+b)"), //"<channel> :Cannot join channel (+b)"
	reply(ERR_BADCHANNELKEY, "%s :Cannot join channel (+i)"), //"<channel> :Cannot join channel (+i)"
	reply(ERR_BADCHANMASK, "%s :Bad Channel Mask"), //"<channel> :Bad Channel Mask"
	reply(ERR_NOPRIVILEGES, ":Permission Denied- You're not an IRC operator"), //":Permission Denied- You're not an IRC operator"
	reply(ERR_CANTKILLSERVER, ":You cant kill a server!"), //":You cant kill a server!"
	reply(ERR_NOOPERHOST, ":No O-lines for your host") //":No O-lines for your host"
};

std::map<uint16_t, std::string> const replies(_rpl_arr, _rpl_arr + sizeof(_rpl_arr) / sizeof(_rpl_arr[0]));

#endif
