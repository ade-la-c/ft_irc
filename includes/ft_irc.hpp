#ifndef FT_IRC_H
# define FT_IRC_H

# include <iostream>

# include "Database.hpp"
# include "Client.hpp"
# include "Channel.hpp"

void error(std::string const & str);

template <typename Iter>
void send_response(Iter begin, Iter end);
//#include "response.hpp" TODO

#endif
