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

		bool debug;

		// returns the client/channel or NULL if it doesn't exist
		Client * get_client(int socket);
		Client * get_client(std::string const & nickname);
		Channel * get_channel(std::string const & name);

		// creates a new client/channel & returns it. Throws an exception if it already exists.
		Client * add_client(int socket);
		Client * add_pclient(Client * client);
		Channel * add_channel(std::string const & name);

		void remove_client(int socket, std::string msg = "");
		void remove_channel(Channel * chan);

		//returns false if wrong amount of args
		bool init(int argc, char **argv);

		client_map clients;
		pclient_map pclients;
		channel_map channels;

		Server * serv;

		void	set_server( Server * serv );

		// only way to get the instance
		static Database * get_instance();
};

#endif
