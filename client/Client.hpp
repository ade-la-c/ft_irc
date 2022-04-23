#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "../client/_IoClient.hpp"
# include "../client/_IRCClient.hpp"
# include "../includes/ft_irc.hpp"

class Client : public _IoClient, public _IRCClient {

	public:

		Client();
		Client(int socket);
		Client(Client const & cpy);
		~Client();

};

#endif
