#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "../includes/ft_irc.hpp"

class Channel {

	public:

		Channel();
		Channel(std::string const & name);
		Channel(Channel const & cpy);
		~Channel();

		std::string name;
		client_map subscribed_clients;

};

#endif
