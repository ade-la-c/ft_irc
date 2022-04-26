#ifndef _IOCLIENT_HPP
# define _IOCLIENT_HPP

class	_IoClient {

	public:
	
		_IoClient();
		_IoClient( int socket );
		_IoClient(_IoClient const & cpy);
		~_IoClient();
	
		int		getSockFd( void ) const;
		char *	getBuf( void );

		void	setBuf( char * buf );
		void	setSockFd( int sockFd );
	
	
	protected:
	
		int					_sockFd;
		char				_buf[512];

};

#endif
