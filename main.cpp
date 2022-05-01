#include "includes/ft_irc.hpp"

/**
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

		tmpReadFdSet = serv.getFdSet(READFD);
		tmpWriteFdSet = serv.getFdSet(WRITEFD);

		serv.doSelect(tmpReadFdSet, tmpWriteFdSet);


		// mettre le message dans le buffer client
		// call client parse_input sur chaque readfd
		for (int i = 0; i < FD_SETSIZE; i++) {
// std::cout << "for loop" << std::endl;

			if (FD_ISSET(i, &tmpReadFdSet)) {
std::cout << "if readfd" << std::endl;
				if (i == serv.getServSocket()) {
							//? handle new connections
					newFd = serv.doAccept();
					if (newFd > serv.getMaxFd())
						serv.setMaxFd(newFd);
					serv.addToFdSet(newFd, READFD);
					db->add_client(newFd);
				} else {
							//? si c'est un autre readfd
					db->get_client(i)->setBuf(serv.doRecv(i, tmpReadFdSet));
					db->get_client(i)->parse_input();
				}
			}
			if (FD_ISSET(i, &tmpWriteFdSet)) {
std::cout << "if writefd" << std::endl;
							//? si c'est un writefd
				serv.doSend(db->responses);
			}
		}
		serv.setFdSet(tmpReadFdSet, READFD);
		serv.setFdSet(tmpWriteFdSet, WRITEFD);
		FD_ZERO(&tmpReadFdSet);
		FD_ZERO(&tmpWriteFdSet);
	}
}

// */

void	do_main( int ac, char ** av, Database * db ) {

	(void)ac;

	Server		serv(atoi(av[1]));
	fd_set		tmpReadFdSet, tmpWriteFdSet;
	char		buf[512];
	int			newFd;

	serv.addToFdSet(serv.getServSocket(), READFD);
	serv.setMaxFd(serv.getServSocket());

	while (true) {

		tmpReadFdSet = serv.getFdSet(READFD);
		tmpWriteFdSet = serv.getFdSet(WRITEFD);
std::cout<<"preselect"<<std::endl;
		serv.doSelect(tmpReadFdSet, tmpWriteFdSet);
std::cout<<"postselect"<<std::endl;

		for (int i = 0; i < FD_SETSIZE; i++) {
// std::cout<<"print random"<<std::endl;
			if (FD_ISSET(i, &tmpReadFdSet)) {

				if (i == serv.getServSocket()) {	// handle new connections
					if ((newFd = serv.doAccept()) < 0)
						continue;
					if (newFd > serv.getMaxFd())
						serv.setMaxFd(newFd);
					serv.addToFdSet(newFd, READFD);
					db->add_client(newFd);
				} else {							// handle other readfds
					if (serv.doRecv(i, tmpReadFdSet, buf)) {
						db->get_client(i)->setBuf(buf);
						db->get_client(i)->parse_input();
					std::cout << "-->" << db->responses.front().second << std::endl;
					} else {
						db->remove_client(i);
					}
				}
			}
			if (FD_ISSET(i, &tmpWriteFdSet)) {		//! writefdset ain't gonna fill itself

std::cout << "writefd" << std::endl;
exit(1);
				serv.doSend(db->responses);
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
	do_main(argc, argv, db);
	return 0;
}
