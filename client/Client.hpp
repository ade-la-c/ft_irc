#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "../includes/ft_irc.hpp"

class Client : public _io_client, _irc_client {

	public:

		Client() : _IoClient(), _IrcClient() ;
		Client(int socket);
		Client(Client const & cpy);
		~Client();

		void parse_input();
		void reg(); //register is a reserved keyword

		// either keep public or make a read member function and put in private
		char buf[512];
		ssize_t length;

		int socket;
		std::string password;
		std::string nickname;
		std::string username;
		bool registered;
		bool pass_set;
		bool nick_set;
		bool user_set;
		int mode;
		std::string realname;
		channel_map subscribed_channels;
		bool oper;
};

#endif
