#include "includes/ft_irc.hpp"

int main(int argc, char **argv) {

	Database * db = Database::get_instance();

	if (!db->init(argc, argv)) {
		error("Wrong number of arguments (need two)");
		return 1;
	}

	Server server(atoi(argv[1]));

	//main loop starts here
}
