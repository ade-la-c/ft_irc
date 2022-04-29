#include "includes/ft_irc.hpp"
void	do_main( int argc, char **argv, Database * db ) {
(void)argc;

	Server	serv(atoi(argv[1]));
	int		newFd;
	fd_set	tmpReadFdSet, tmpWriteFdSet;

	serv.addToFdSet(serv.getServSocket(), READFD);
	serv.setMaxFd(serv.getServSocket());

	while (true) {
std::cout << "loop" << std::endl;
		// update clients to write on (writefds)
		// |->	loop call asking for a set of clients + a message to send
		// |->	or single client set + message call, (select call will loop faster)


		// update clients to read from (readfds)
			// (add new connexions and close old ones)

		// call select
		serv.doSelect();

		// send in writefds and recv from readfds

		// mettre le message dans le buffer client
		// call client parse_input sur chaque readfd
		for (int i = 0; i < FD_SETSIZE; i++) {
// std::cout << "for loop" << std::endl;
			tmpReadFdSet = serv.getFdSet(READFD);
			tmpWriteFdSet = serv.getFdSet(WRITEFD);

			if (FD_ISSET(i, &tmpReadFdSet)) {
std::cout << "if readfd" << std::endl;
				if (i == serv.getServSocket()) {
							//? handle new connections
					newFd = serv.acceptNewConnection();
					if (newFd > serv.getMaxFd())
						serv.setMaxFd(newFd);
					serv.addToFdSet(newFd, READFD);
					db->add_client(newFd);
				} else {
							//? si c'est un autre readfd
					db->get_client(i)->setBuf(serv.doRecv(i));
					db->get_client(i)->parse_input();
				}
			}
			if (FD_ISSET(i, &tmpWriteFdSet)) {
std::cout << "if writefd" << std::endl;
							//? si c'est un writefd
				serv.doSend(db->responses);
			}
			serv.setFdSet(tmpReadFdSet, READFD);
			serv.setFdSet(tmpWriteFdSet, WRITEFD);
		}
	}
}

int		main(int argc, char **argv) {

	Database * db = Database::get_instance();

	if (!db->init(argc, argv)) {
		error("Wrong number of arguments (need two)");
		return 1;
	}

	do_main(argc, argv, db);
	return 0;
}
