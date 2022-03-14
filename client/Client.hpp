#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "../includes/ft_irc.hpp"

class Client {

	public:

		Client();
		Client(uint32_t ip);
		Client(Client const & cpy);
		~Client();

		void parse_input();

		// either keep public or make a read member function and put in private
		char buf[512];
		ssize_t length;

	private:

		uint32_t ip;
};

#endif
