## ft\_irc documentation

### Requirements

* Be able to authenticate, set a nickname, a username, join a channel and send and receive private messages
	* PASS, NICK, USER, JOIN, PRIVMSG
* All the messages have to be properly forwarded
* There must be operators and regular users
	* OPER
* The must be the commands specific to the operators
	* SQUIT?, PRIVMSG masks, KILL, ERROR?, REHASH, RESTART
