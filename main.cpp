#include "includes/ft_irc.hpp"

void	do_main() {

	Database *	db = Database::get_instance();
	Server		serv(atoi(db->port.c_str()));
	fd_set		tmpReadFdSet, tmpWriteFdSet;
	char		buf[512];
	int			newFd;


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
				} else {							// handle other readfds	//! faire function qui close tous les fds
					if (serv.doRecv(i, tmpReadFdSet, buf)) {
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

//	Client client(4);
//	response_pair response;
//	while (1) {
//		bzero(client.getBuf(), 512);
//		read(1, client.getBuf(), 512);
//		client.parse_input();
//		while ((response = db->next_response()).first)
//			std::cout << response.second;
//	}
//	return 0;
	do_main();
	return 0;
}

