#ifndef _IRCCLIENT_HPP
# define _IRCCLIENT_HPP

# include "../includes/ft_irc.hpp"

class _IRCClient {

	public:

		_IRCClient();
		_IRCClient(_IRCClient const & cpy);
		~_IRCClient();

		void parse_input();
		void reg(); //register is a reserved keyword
		response_pair response(int r, ...);
		response_pair command(int r, ...);

		std::string password;
		std::string nickname;
		std::string username;
		std::string hostname;
		std::string servername;
		std::string realname;
		bool registered;
		bool pass_set;
		bool nick_set;
		bool user_set;
		channel_map subscribed_channels;
		//****** TODO make map with pointers or they do not update
		bool oper;
};

#endif
