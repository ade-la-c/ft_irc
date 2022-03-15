# ft\_irc documentation

### A (very) small irc server made in c++98 with sockets during the 42 cursus
### Follows the most recent RFC specification

## Requirements

launched as follows
```
./ircserv <password> <port>
```
* Client must be able to authenticate, set a nickname, a username, join a channel and send and receive private messages
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
PRIVMSG masks, KILL, REHASH, RESTART
```
* We must **NOT** develop a client
* We must **NOT** handle server to server connections

## Command Details

### Regular users

#### PASS

```
parameters : <password>
```

Must be entered before the latter of the USER/NICK pair\
Password must match the one from the server arguments\
Can send multiple ones but only the last one is used

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
parameters : <nickname>
```

Give a nickname or change the existing one\
It has a maximum of 9 characters\
If it is already in use, a nickname collision occurs. As a result, all instances of the nickname are removed & if it comes from a nickname change, the old one is also removed\
If the collusion comes from a directly connected client (always the case for us), the server may simply drop the command and respond with ERR\_NICKCOLLISION

* Replies:
```
ERR_NONICKNAMEGIVEN             ERR_ERRONEUSNICKNAME
ERR_NICKNAMEINUSE               ERR_NICKCOLLISION
```

#### USER

```
Parameters: <username> <mode> <unused> <realname>
```

Used at the beginning of connection to specify info about the user\
\<mode\> should be numeric. It is a bitmask with 2 significant bits:
* bit 2 for user mode 'w' (user receives wallops)
* bit 3 for user mode 'i' (user becomse invisible)

\<unused\> is, well, unused. It's there for legacy purposes.\
Realname may contain space character but the parameter must then be prefix by a colon (:)

* Replies:
```
ERR_NEEDMOREPARAMS              ERR_ALREADYREGISTRED
```

#### JOIN

```
Parameters: ( <channel> *( "," <channel> ) [ <key> *( "," <key> ) ] ) / "0"
```

Used to join a channel or a list of channels.\
If they don't exist, it creates them\
Servers must be able to parse lists but never send lists to clients\
Users that have joined a channel receive information about all the command affecting the channel (JOIN, PRIVMSG)

On a succesful JOIN, The user first receives a JOIN message back as confirmation, then the topic with RPL\_TOPIC, then the list of users on the channel with RPL\_NAMREPLY which must include the user himself

"0" is a special argument which is a request to leave all channels a user is a member of.\
It is processed as if the user had sent a PART command for every channel

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

Used by operators to restart the server

* Replies:
```
ERR_NOPRIVILEGES
```

## REPLIES

#### Error replies

RPL\_REHASHING
ERR\_CANTKILLSERVER
ERR\_NOPRIVILEGES
ERR\_NOSUCHSERVER
RPL\_YOUREOPER
ERR\_NOOPERHOST
ERR\_PASSWDMISMATCH
ERR\_NORECIPIENT
ERR\_NOTEXTTOSEND
ERR\_CANNOTSENDTOCHAN
ERR\_NOTOPLEVEL
ERR\_WILDTOPLEVEL
ERR\_NOSUCHNICK
RPL\_AWAY
ERR\_UNAVAILRESOURCE
ERR\_TOOMANYTARGETS
ERR\_TOOMANYCHANNELS
ERR\_NOSUCHCHANNEL
ERR\_BADCHANMASK
ERR\_CHANNELISFULL
ERR\_BADCHANNELKEY
ERR\_INVITEONLYCHAN
ERR\_BANNEDFROMCHAN
ERR\_ERRONEUSNICKNAME
ERR\_NICKNAMEINUSE
ERR\_NICKCOLLISION
* 431 - ERR\_NONICKNAMEGIVEN
* 461 - ERR\_NEEDMOREPARAMS
```
"<command> :Not enough parameters"
returned when there aren't enough parameters
```
* 462 - ERR\_ALREADYREGISTERED
```
":You may not reregister"
returned when there is an attempt to change part of the registered details (e.g. second PASS or USER command)
```

#### Command responses

RPL\_TOPIC
