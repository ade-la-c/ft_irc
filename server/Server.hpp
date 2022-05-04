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
	int			doAccept() const;
	void		doSelect( fd_set *readfds, fd_set *writefds ) const;
	bool		doRecv( int fd, char buf[512] );
	void		doSend( Client * client );

	void		closeClient( Client * client );

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

	int					_servSocket;
	int					_maxFd;
	fd_set				_readFds;
	fd_set				_writeFds;

};

#endif
