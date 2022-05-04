#ifndef _IRCCLIENT_HPP
# define _IRCCLIENT_HPP

# include "../includes/ft_irc.hpp"

class _IRCClient {

	public:
		
		class NoResponseException : public std::exception {
			virtual char const * what() const throw() {
				return "No response to send";
			}
		};

		_IRCClient();
		_IRCClient(_IRCClient const & cpy);
		~_IRCClient();

		void parse_input();
		void reg(); //register is a reserved keyword
		void response(int r, ...);
		void command(int r, ...);

		bool should_send() const;
		std::string response() const;
		void sent_bytes(ssize_t bytes);

		std::string password;
		std::string nickname;
		std::string username;
		std::string hostname;
		std::string servername;
		std::string realname;
		std::string buffer;
		bool registered;
		bool pass_set;
		bool nick_set;
		bool user_set;
		pchannel_map subscribed_channels;
		bool oper;

		std::list<std::string> responses;
};

#endif
