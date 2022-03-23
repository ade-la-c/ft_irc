# ft\_irc documentation

### A (very) small irc server made in c++98 with sockets during the 42 cursus
### The first one ever made at 19
### Follows the most recent RFC specification, when applicable given the subject

## Requirements

Launched as follows:
```
./ircserv <password> <port>
```
* Clients must be able to authenticate, set a nickname, a username, join a channel and send and receive private messages
```
PASS, NICK, USER, JOIN, PRIVMSG
```
* All the messages have to be properly forwarded
* There must be operators and regular users
```
OPER
```
* There must be the commands specific to the operators
```
PRIVMSG masks, KILL, REHASH, RESTART, DIE
```
* We must **NOT** develop a client
* We must **NOT** handle server to server connections
* While a configuration file is recommended by the rfc standard, it is not *required*

## Command Details

### Regular users

#### PASS

```
Parameters : <password>
```

Must be entered before the latter of the USER/NICK pair.\
Password must match the one from the server arguments.\
Can send multiple ones but only the last one is used.

Recommended order:
1. PASS
2. NICK
3. USER

* Replies:
```
ERR_NEEDMOREPARAMS              ERR_ALREADYREGISTRED
```

#### NICK
```
Parameters : <nickname>
```

Give a nickname or change the existing one.\
It has a maximum of 9 characters.\
If it is already in use, a nickname collision occurs. As a result, all instances of the nickname are removed & if it comes from a nickname change, the old one is also removed.\
If the collision comes from a directly connected client (always the case for us), the server may simply drop the command and respond with ERR\_NICKCOLLISION.

* Replies:
```
ERR_NONICKNAMEGIVEN             ERR_ERRONEUSNICKNAME
ERR_NICKNAMEINUSE               ERR_NICKCOLLISION
```

#### USER

```
Parameters: <username> <mode> <unused> <realname>
```

Used at the beginning of connection to specify info about the user.\
\<mode\> should be numeric. It is a bitmask with 2 significant bits:
* bit 2 for user mode 'w' (user receives wallops)
* bit 3 for user mode 'i' (user becomse invisible)

\<unused\> is, well, unused. It's there for legacy purposes.\
Realname may contain space character but the parameter must then be prefix by a colon (:).

* Replies:
```
ERR_NEEDMOREPARAMS              ERR_ALREADYREGISTRED
```

#### JOIN

```
Parameters: ( <channel> *( "," <channel> ) [ <key> *( "," <key> ) ] ) / "0"
```

Used to join a channel or a list of channels.\
If they don't exist, it creates them.\
Servers must be able to parse lists but never send lists to clients.\
Users that have joined a channel receive information about all the command affecting the channel (JOIN, PRIVMSG).

On a succesful JOIN, The user first receives a JOIN message back as confirmation, then the topic with RPL\_TOPIC, then the list of users on the channel with RPL\_NAMREPLY which must include the user himself.

"0" is a special argument which is a request to leave all channels a user is a member of.\
It is processed as if the user had sent a PART command for every channel.

* Replies:
```
ERR_NEEDMOREPARAMS              ERR_BANNEDFROMCHAN
ERR_INVITEONLYCHAN              ERR_BADCHANNELKEY
ERR_CHANNELISFULL               ERR_BADCHANMASK
ERR_NOSUCHCHANNEL               ERR_TOOMANYCHANNELS
ERR_TOOMANYTARGETS              ERR_UNAVAILRESOURCE
RPL_TOPIC                       RPL_NAMREPLY
```

#### PRIVMSG

```
Parameters: <msgtarget> <text to be sent>
```

Used to send messages to users & channels.

For opertors:
\<msgtarget\> may be a host mask (#\<mask\>) or server mask ($\<mask\>). It will then send to user users having a host or server matching the mask.\
The mask must have at least 1 '.' and no wildcard after the last one to prevent "$\*" or "#\*".\
Wildcards are '\*' and '?'.\

* Replies:
```
ERR_NORECIPIENT                 ERR_NOTEXTTOSEND
ERR_CANNOTSENDTOCHAN            ERR_NOTOPLEVEL
ERR_WILDTOPLEVEL                ERR_TOOMANYTARGETS
ERR_NOSUCHNICK
RPL_AWAY
```

#### OPER

```
Parameters: <name> <password>
```

Used to obtain operator privileges.
Upon success, the user receives a MODE message woth the new modes.

* Replies:
```
ERR_NEEDMOREPARAMS              RPL_YOUREOPER
ERR_NOOPERHOST                  ERR_PASSWDMISMATCH
```

### Operators

#### KILL

```
Parameters: <nickname> <comment>
```

Used to close a client-server connection.\
Also sent on nickname collisions.\
It is advised that the nickname be put in a list of temporary unavailaible nicknames to prevent kill loops.\
It is recommended to only be allowed to operators.

* Replies:
```
ERR_NOPRIVILEGES              ERR_NEEDMOREPARAMS
ERR_NOSUCHNICK                ERR_CANTKILLSERVER
```

### Optional features

In the absence of these options, an error reply message must be generated or an unknown command error.

#### REHASH

```
Parameters: None
```

Used by operators to make a server re-read and process its configuration file.

* Replies:
```
RPL_REHASHING                 ERR_NOPRIVILEGES
```

#### RESTART

```
Parameters: None
```

Used by operators to restart the server.

* Replies:
```
ERR_NOPRIVILEGES
```

#### DIE

```
Parameters: None
```

Used by operators to shut down the server.

* Replies:
```
ERR_NOPRIVILEGES
```

## REPLIES

#### Error replies


* 401 - ERR\_NOSUCHNICK
```
"<nickname> :No such nick/channel"
Used to indicate the nickname parameter supplied to a command is currently unused.
```
* 402 - ERR\_NOSUCHSERVER
```
"<server name> :No such server"
Used to indicate the server name given currently does not exist.
```
* 403 - ERR\_NOSUCHCHANNEL
```
"<channel name> :No such channel"
Used to indicate that a channel name is invalid
```
* 404 - ERR\_CANNOTSENDTOCHAN
```
"<channel name> :Cannot send to channel"
Sent to a user who is either (a) not on a channel which is mode +n or (b) not a chanop (or mode +v) on a channel which has mode +m set or where the user is
banned and is trying to send a PRIVMSG message to that channel.
```
* 405 - ERR\_TOOMANYCHANNELS
```
"<channel name> :You have joined too many channels"
Sent to a user when they have joined the maximum number of allowed channels and they try to join another channel.
```
* 407 - ERR\_TOOMANYTARGETS
```
"<target> :<error code> recipients. <abort message>"
Returned to a client which is attempting to send a PRIVMSG/NOTICE using the user@host destination format and for a user@host which has several occurrences.
Returned to a client which trying to send a PRIVMSG/NOTICE to too many recipients.
Returned to a client which is attempting to JOIN a safe channel using the shortname when there are more than one such channel.
```
* 411 - ERR\_NORECIPIENT
```
":No recipient given (<command>)"
Returned by PRIVMSG to indicate that the message wasn't delivered.
```
* 412 - ERR\_NOTEXTTOSEND
```
":No text to send"
Returned by PRIVMSG to indicate that the message wasn't delivered.
```
* 413 - ERR\_NOTOPLEVEL
```
"<mask> :No toplevel domain specified"
Returned when an invalid use of "PRIVMSG $<server>" or "PRIVMSG #<host>" is attempted.
```
* 414 - ERR\_WILDTOPLEVEL
```
"<mask> :Wildcard in toplevel domain"
Returned when an invalid use of "PRIVMSG $<server>" or "PRIVMSG #<host>" is attempted.
```
* 431 - ERR\_NONICKNAMEGIVEN
```
":No nickname given"
Returned when a nickname parameter is expected for a command and isn't found.
```
* 432 - ERR\_ERRONEUSNICKNAME
```
"<nick> :Erroneus nickname"
Returned after receiving a NICK message which contains characters which do not fall in the defined set:
( letter / special ) *8( letter / digit / special / "-" )
```
* 433 - ERR\_NICKNAMEINUSE
```
"<nick> :Nickname is already in use"
Returned when a NICK message is processed that results in an attempt to change to a currently existing nickname.
```
* 436 - ERR\_NICKCOLLISION
```
"<nick> :Nickname collision KILL"
Returned by a server to a client when it detects a nickname collision
```
* 437 - ERR\_UNAVAILRESOURCE
```
"<nick/channel> :Nick/channel is temporarily unavailable"
Returned by a server to a user trying to join a channel currently blocked by the channel delay mechanism.
Returned by a server to a user trying to change nickname when the desired nickname is blocked by the nick delay mechanism.
```
* 461 - ERR\_NEEDMOREPARAMS
```
"<command> :Not enough parameters"
Returned when there aren't enough parameters.
```
* 462 - ERR\_ALREADYREGISTERED
```
":You may not reregister"
Returned when there is an attempt to change part of the registered details (e.g. second PASS or USER command).
```
* 464 - ERR\_PASSWDMISMATCH
```
":Password incorrect"
Returned to indicate a failed attempt at registering a connection for which a password was required and was either not given or incorrect.
```
* 471 - ERR\_CHANNELISFULL
```
"<channel> :Cannot join channel (+l)"
```
* 473 - ERR\_INVITEONLYCHAN
```
<channel> :Cannot join channel (+i)"
```
* 474 - ERR\_BANNEDFROMCHAN
```
"<channel> :Cannot join channel (+b)"
```
* 475 - ERR\_BADCHANNELKEY
```
<channel> :Cannot join channel (+i)"
```
* 476 - ERR\_BADCHANMASK
```
"<channel> :Bad Channel Mask"
```
* 481 - ERR\_NOPRIVILEGES
```
":Permission Denied- You're not an IRC operator"
Any command requiring operator privileges to operate MUST return this error to indicate the attempt was unsuccessful.
```
* 483 - ERR\_CANTKILLSERVER
```
":You cant kill a server!"
Returned when attempting to kill a server
```
* 491 - ERR\_NOOPERHOST
```
":No O-lines for your host"
If a client sends an OPER message and the server has not been configured to allow connections from the
client's host as an operator, this error MUST be returned.
```

#### Command responses


* 301 - RPL\_AWAY
```
"<nick> :<away message>"
Sent when sending a message to a user that is away
```
* 332 - RPL\_TOPIC
```
"<channel> :<topic>"
When sending a TOPIC message to determine the channel topic, one of two replies is sent.  If
the topic is set, RPL_TOPIC is sent back else RPL_NOTOPIC.
```
* 381 - RPL\_YOUREOPER
```
":You are now an IRC operator"
Sent back to a client which has just successfully issued an OPER message and gained operator status.
```
* 382 - RPL\_REHASHING
```
"<config file> :Rehashing"
If the REHASH option is used and an operator sends a REHASH message, an RPL_REHASHING is sent back to the operator.
```
