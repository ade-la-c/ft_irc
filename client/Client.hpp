#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "../includes/ft_irc.hpp"

class Client {

	public:

		Client();
		Client(int socket);
		Client(Client const & cpy);
		~Client();

		void parse_input();

		// either keep public or make a read member function and put in private
		char buf[512];
		ssize_t length;

		int socket;
		std::string password;
		std::string nickname;
		std::string username;
		bool registered;
		int mode;
		std::string realname;
		channel_map subscribed_channels;
		bool oper;
};

#endif
