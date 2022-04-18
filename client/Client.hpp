#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "../includes/ft_irc.hpp"
# include "_IoClient.hpp"

class Client : public _IoClient {

	public:

		Client();
		Client(int socket);
		Client(Client const & cpy);
		~Client();

		void parse_input();

		int				getSocket( void ) const;
		std::string		getBuf( void ) const;

		// either keep public or make a read member function and put in private

	private:

		int			_socket;
		char		_buf[512];
		ssize_t		_length = 512;

};

#endif
