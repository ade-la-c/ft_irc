#include "../includes/ft_irc.hpp"

std::string response(const char * format, ...) {
	va_list arg;
	char response[512];

	va_start(arg, format);
	vsprintf(response, format, arg);
	return response;
}
