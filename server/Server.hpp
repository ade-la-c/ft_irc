#ifndef SERVER_HPP
# define SERVER_HPP

# include "../includes/ft_irc.hpp"

class	Server {


public:

	Server();
	Server( int port );
	~Server();

	int			getServSocket() const;
	int			getFdSet() const;

	void		addToFdSet( int fd, int fdType );
	int			acceptNewConnection() const;
	void		doSelect();

	class	init_error : public std::exception {
		public:
			virtual char const * what() const throw() {
				return "error: server cannot initialize";
			}
	};


private:

	int			_servSocket;
	fd_set		_readFds;
	fd_set		_writeFds;

};

#endif
