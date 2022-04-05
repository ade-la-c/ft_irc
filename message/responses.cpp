#include "ft_irc.h"

std::string response(char const * format, ...) {
	va_list arg;
	char * response;

	va_start(arg, format);
	vsprintf(response, format, arg);
	return response;
}
