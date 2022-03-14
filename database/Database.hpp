#ifndef DATABASE_HPP
# define DATABASE_HPP

# include <map>
# include "Client.hpp"
# include "Channel.hpp"

# include "../includes/ft_irc.hpp"

// Singleton Database class
class Database {
	private:

		// private constructor -> can only get instance through get_instance;
		Database();

		// disallow copy and assignation, unimplemented
		Database(Database const & cpy);
		Database & operator=(Database const & rhs);

		typedef std::map<uint32_t, Client> client_map;	// {ip, client} map
		typedef std::map<std::string, Channel> channel_map; // {name, channel} map

		client_map clients;
		channel_map channels;

	public:

		class already_exists : public std::exception {
			public:
				virtual char const * what() const throw() {
					return "Element already exists";
				}
		};

		~Database();

		// getter/setter make no sense
		std::string password;
		std::string port;

		// returns the client/channel or NULL if it doesn't exist
		Client * get_client(uint32_t ip);
		Channel * get_channel(std::string const & name);

		// creates a new client/channel & returns it. Throws an exception if it already exists.
		Client & add_client(uint32_t ip);
		Channel & add_channel(std::string const & name);

		//returns false if wrong amount of args
		bool init(int argc, char **argv);

		// only way to get the instance
		static Database * get_instance();
};

#endif
