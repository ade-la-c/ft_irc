#ifndef DATABASE_HPP
# define DATABASE_HPP

# include "../includes/ft_irc.hpp"

// Singleton Database class
class Database {
	public:

	private:

		// private constructor -> can only get instance through get_instance;
		Database();

		// disallow copy and assignation, unimplemented
		Database(Database const & cpy);
		Database & operator=(Database const & rhs);


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
		Client * get_client(int socket);
		Channel * get_channel(std::string const & name);

		// creates a new client/channel & returns it. Throws an exception if it already exists.
		Client * add_client(int socket);
		Channel * add_channel(std::string const & name);

		//returns false if wrong amount of args
		bool init(int argc, char **argv);

		// only way to get the instance
		static Database * get_instance();
};

#endif
