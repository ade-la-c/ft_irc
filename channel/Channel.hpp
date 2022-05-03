#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "../includes/ft_irc.hpp"

class Channel {

	public:

		Channel();
		Channel(std::string const & name);
		Channel(Channel const & cpy);
		~Channel();

		void add_client(Client & client);
		void remove_client(Client & client);
		bool empty();

		std::string name;

		pclient_map subscribed_clients;

};

#endif
