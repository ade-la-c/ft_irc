#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "_IoClient.hpp"
# include "_IRCClient.hpp"

class Client : public _IoClient, public _IRCClient {

	public:

		Client();
		Client(int socket);
		Client(Client const & cpy);
		~Client();

};

#endif
