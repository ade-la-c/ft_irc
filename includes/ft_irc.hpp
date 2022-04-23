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

# include "../database/Database.hpp"
# include "../client/Client.hpp"
# include "../client/_IoClient.hpp"
# include "../channel/Channel.hpp"
# include "../server/Server.hpp"

# define READFD		0
# define WRITEFD	1

typedef struct sockaddr_in		SA_IN;
typedef struct sockaddr			SA;

void error(std::string const & str);

template <typename Iter>
void send_response(Iter begin, Iter end);
//#include "response.hpp" TODO

#endif
