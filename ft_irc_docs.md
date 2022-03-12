# ft\_irc documentation

### A (very) small irc server made in c++98 with sockets during the 42 cursus

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
SQUIT?, PRIVMSG masks, KILL, ERROR?, REHASH, RESTART
```
* We must **NOT** develop a client
* We must **NOT** handle server to server connections




## Command Details

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
Maximun 9 characters\
If it is already in use, a nickname collision occurs.\
As a result, all instances of the nickname are removed & if it comes from a nickname change, the old one is also removed\
If the collusion comes from a directly connected client (always the case for us), the server may simply drop the command and respond with ERR_NICKCOLLISION

* Replies:
```
ERR_NONICKNAMEGIVEN             ERR_ERRONEUSNICKNAME
ERR_NICKNAMEINUSE               ERR_NICKCOLLISION
```

#### USER
```
Parameters: <username> <hostname> <servername> <realname>
```
Used at the beginning of connection to specify info about the user\
hostname and serverame are normally ignored when coming from a client for security reasons\
Realname must be the last parameter and must be prefixed by a colon (:) because it can contain spaces



## REPLIES

#### Error replies

* 431 - ERR\_NONICKNAMEGIVEN
* 461 - ERR\_NEEDMOREPARAMS
> "\<command\> :Not enough parameters"

returned when there aren't enough parameters
* 462 - ERR\_ALREADYREGISTERED
> ":You may not reregister"

returned when there is an attempt to change part of the registered details (e.g. second PASS or USER command)

#### Command responses
