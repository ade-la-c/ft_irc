#ifndef FT_IRC_HPP
# define FT_IRC_HPP

# include <iostream>

# include <sys/socket.h>
# include <netdb.h>
# include <sys/types.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <csignal>
# include <fcntl.h>
# include <sys/select.h>
# include <utility>
# include <poll.h>
# include <list>
# include <vector>
# include <map>
# include <cstdio>
# include <cstdarg>

# define MAXFD		1024
# define READFD		0
# define WRITEFD	1

class Database;
class Client;
class Channel;
class Message;

typedef struct sockaddr_in		SA_IN;
typedef struct sockaddr			SA;

typedef std::map<int, Client> client_map;	// {socket, client} map
typedef std::map<std::string, Channel> channel_map; // {name, channel} map

# include "../database/Database.hpp"
# include "../client/Client.hpp"
# include "../channel/Channel.hpp"
# include "../message/Message.hpp"
# include "../message/replies.hpp"
# include "../server/Server.hpp"

void error(std::string const & str);

template <typename Iter>
void send_response(Iter begin, Iter end);

void execute(Client & client, Message & msg);

std::string response(const char * format, ...);

//#include "response.hpp" TODO

#endif
