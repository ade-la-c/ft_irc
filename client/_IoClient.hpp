#ifndef _IOCLIENT_HPP
# define _IOCLIENT_HPP

// # include "../includes/ft_irc.hpp"
# include <cstring>

class	_IoClient {

	public:
	
		_IoClient();
		_IoClient( int socket );
		_IoClient(_IoClient const & cpy);
		~_IoClient();
	
		int		getSockFd( void ) const;
		char *	getBuf( void );

		void	setBuf( char buf[512] );
		void	setSockFd( int sockFd );
	
	
	protected:
	
		int					_sockFd;
		char				_buf[512];

};

#endif
