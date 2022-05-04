#include "../includes/ft_irc.hpp"

void error(std::string const & str) {
	std::cerr << str << std::endl;
}

void	exit_error( std::string const & str ) {

	std::cerr << "error: " << str << std::endl;
	std::cerr << "errno = " << errno << std::endl;
	exit(EXIT_FAILURE);
}
