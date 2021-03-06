#include "../includes/ft_irc.hpp"

bool	fdIsset( int fd, fd_set * set ) {

	if (fd < 0) {
		std::cerr << fd << std::endl;
		exit_error("fd_isset: fd is negative");
	}
	return FD_ISSET(fd, set);
}

void	fdSet( int fd, fd_set * set ) {

	if (fd < 0)
		exit_error("fd_set: fd is negative");
	FD_SET(fd, set);
}

void	fdClr( int fd, fd_set * set ) {

	if (fd < 0)
		exit_error("fd_clr: fd is negative");
	FD_CLR(fd, set);
}

