#include "includes/ft_irc.hpp"

int main(int argc, char **argv) {

	Database * db = Database::get_instance();

	if (!db->init(argc, argv)) {
		error("Wrong number of arguments (need two)");
		return 1;
	}

	//main loop starts here
	try {
		Message msg(std::string(":nickname COMMAND these are :some parameters\r\n"));
		std::cout << "[" << msg.get_prefix() << "]" << std::endl;
		std::cout << "[" << msg.get_command() << "]" << std::endl;
		for (std::vector<std::string>::size_type i = 0; i < msg.get_params_count(); i++) {
		std::cout << "[" << msg.get_params()[i] << "]" << std::endl;
		}
	} catch (std::exception & e) {
		std::cerr << e.what() << std::endl;
	}
}
