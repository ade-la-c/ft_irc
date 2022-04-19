#ifndef FT_IRC_HPP
# define FT_IRC_HPP

# include <iostream>

# include <sys/socket.h>
# include <netdb.h>
# include <sys/types.h>
# include <arpa/inet.h>
# include <signal.h>
# include <fcntl.h>
# include <poll.h>
# include <list>
# include <vector>
# include <map>
# include <cstdio>
# include <cstdarg>

class Database;
class Client;
class Channel;
class Message;

typedef std::map<int, Client> client_map;	// {socket, client} map
typedef std::map<std::string, Channel> channel_map; // {name, channel} map

# include "../database/Database.hpp"
# include "../client/Client.hpp"
# include "../channel/Channel.hpp"
# include "../message/Message.hpp"
# include "../message/replies.hpp"

void error(std::string const & str);

template <typename Iter>
void send_response(Iter begin, Iter end);

std::string response(const char * format, ...);

//#include "response.hpp" TODO

#endif
