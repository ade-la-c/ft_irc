#include "ft_irc.h"

std::string response(std::string const & format, ...) {
	va_list arg;
	char * response;

	va_start(arg, format.c_str()); //unsure of this c_str(), maybe should be &format
	vsprintf(response, format.c_str(), arg);
	return response;
}
