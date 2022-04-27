#ifndef SERVER_HPP
# define SERVER_HPP

# include "../includes/ft_irc.hpp"

class	Server {


public:

	Server();
	Server( int port );
	~Server();

	int			getServSocket() const;
	fd_set		getFdSet( int fdType );
	int			getMaxFd() const;

	void		setMaxFd( int newMaxFd );
	void		setFdSet( fd_set set, int fdType );

	void		addToFdSet( int fd, int fdType );
	int			acceptNewConnection() const;
	void		doSelect();
	char *		doRecv( int fd ) const;
	void		doSend( response_list responses );

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
