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
	int			getMaxFd() const;

	void		setMaxFd( int newMaxFd );

	void		addToFdSet( int fd, int fdType );
	int			acceptNewConnection() const;
	void		doSelect();

	class	init_error : public std::exception {
		public:
			virtual char const * what() const throw() {
				return "error: server cannot initialize";
			}
	};

	class	server_error : public std::exception {
		public:
			virtual char const * what() const throw() {
				return "error: fatal";
			}
	};


private:

	int			_servSocket;
	int			_maxFd;
	fd_set		_readFds;
	fd_set		_writeFds;

};

#endif
