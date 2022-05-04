#include "includes/ft_irc.hpp"

void	main_loop() {

	Database *	db = Database::get_instance();
	Server		serv(atoi(db->port.c_str()));
	fd_set		tmpReadFdSet, tmpWriteFdSet;
	char		buf[512];
	int			newFd;

	db->set_server(&serv);

	serv.addToFdSet(serv.getServSocket(), READFD);
	serv.setMaxFd(serv.getServSocket());

	while (true) {

		tmpReadFdSet = serv.getFdSet(READFD);
		tmpWriteFdSet = serv.getFdSet(WRITEFD);
		serv.doSelect(&tmpReadFdSet, &tmpWriteFdSet);

		for (int i = 0; i <= serv.getMaxFd(); i++) {

			if (fdIsset(i, &tmpReadFdSet)) {

				if (i == serv.getServSocket()) {	// handle new connections
					if ((newFd = serv.doAccept()) < 0)
						continue;
					serv.setMaxFd(newFd);
					serv.addToFdSet(newFd, READFD);
					serv.addToFdSet(newFd, WRITEFD);
					db->add_client(newFd);
				} else {							// handle other readfds
					if (serv.doRecv(i, buf)) {
						db->get_client(i)->setBuf(buf);
						db->get_client(i)->parse_input();
					} else {
						db->remove_client(i);
					}
				}
			}
			if (fdIsset(i, &tmpWriteFdSet) && (db->clients.count(i) > 0)) {
				serv.doSend(db->get_client(i));
			}
		}
		FD_ZERO(&tmpReadFdSet);
		FD_ZERO(&tmpWriteFdSet);
	}
}

int		main(int argc, char **argv) {

	Database * db = Database::get_instance();

	if (!db->init(argc, argv)) {
		error("Wrong number of arguments (need two)");
		return 1;
	}
	main_loop();
	return 0;
}
