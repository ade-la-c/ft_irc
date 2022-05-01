#ifndef DATABASE_HPP
# define DATABASE_HPP

# include <map>
# include "../includes/ft_irc.hpp"

// Singleton Database class
class Database {
	private:

		// private constructor -> can only get instance through get_instance;
		Database();

		// disallow copy and assignation, unimplemented
		Database(Database const & cpy);
		Database & operator=(Database const & rhs);


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
		std::string hostname;

		// returns the client/channel or NULL if it doesn't exist
		Client * get_client(int socket);
		Channel * get_channel(std::string const & name);

		// creates a new client/channel & returns it. Throws an exception if it already exists.
		Client * add_client(int socket);
		Channel * add_channel(std::string const & name);

		void remove_client(int socket);

		void add_response(response_pair response);
		response_pair next_response(); // sends (NULL, "") if no response.

		//returns false if wrong amount of args
		bool init(int argc, char **argv);

		client_map clients;
		channel_map channels;

		response_list responses;

		// only way to get the instance
		static Database * get_instance();
};

#endif
