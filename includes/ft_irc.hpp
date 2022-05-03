#ifndef FT_IRC_HPP
# define FT_IRC_HPP

# include <iostream>

# include <sys/socket.h>
# include <netdb.h>
# include <sys/types.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <csignal>
# include <cstdlib>
# include <fcntl.h>
# include <sys/select.h>
# include <utility>
# include <poll.h>
# include <list>
# include <vector>
# include <map>
# include <cstdio>
# include <cstdarg>
# include <cstring>
# include <errno.h>

# define READFD				0
# define WRITEFD			1

class Database;
class Client;
class Channel;
class Message;

typedef struct sockaddr_in		SA_IN;
typedef struct sockaddr			SA;

typedef std::pair<Client *, std::string> response_pair;
typedef std::list<response_pair> response_list;

typedef std::map<int, Client> client_map;	// {socket, client} map
typedef std::map<std::string, Channel> channel_map; // {name, channel} map

typedef std::map<std::string, Client *> pclient_map;	// {nickname, client} map
typedef std::map<std::string, Channel *> pchannel_map; // {name, channel} map

# include "../database/Database.hpp"
# include "../client/Client.hpp"
# include "../channel/Channel.hpp"
# include "../message/Message.hpp"
# include "../message/replies.hpp"
# include "../server/Server.hpp"


void	error(std::string const & str);
void	exit_error( std::string const & str );

bool	fdIsset( int fd, fd_set * set );
void	fdSet( int fd, fd_set * set );
void	fdClr( int fd, fd_set * set );

void execute(Client & client, Message & msg);

//#include "response.hpp" TODO

#endif
