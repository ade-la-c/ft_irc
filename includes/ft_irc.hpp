#ifndef FT_IRC_HPP
# define FT_IRC_HPP

# include <iostream>

# include <sys/socket.h>
# include <netdb.h>
# include <sys/types.h>
# include <arpa/inet.h>
# include <signal.h>
# include <fcntl.h>
# include <sys/select.h>
# include <utility>

# include "../database/Database.hpp"
# include "../client/Client.hpp"
# include "../channel/Channel.hpp"

void error(std::string const & str);

template <typename Iter>
void send_response(Iter begin, Iter end);
//#include "response.hpp" TODO

#endif
