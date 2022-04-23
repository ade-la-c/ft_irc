#include "includes/ft_irc.hpp"

int main(int argc, char **argv) {

	Database * db = Database::get_instance();

	if (!db->init(argc, argv)) {
		error("Wrong number of arguments (need two)");
		return 1;
	}


<<<<<<< HEAD
	Server	serv(atoi(argv[1]);
	int		newfd, fdmax;
	fd_set	

	serv.addToFdSet(serv.getServSocket(), READFD);
	serv.setMaxFd(serv.getServSocket());

	while (true) {

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

			if (FD_ISSET(i, serv.getFdSet(READFD))) {

				if (i == serv.getServSocket()) {
					// handle new connections
					newfd = serv.acceptNewConnection();
					if (newfd > fdmax)
						serv.setMaxFd(newfd);
					serv.addToFdSet(newfd, READFD);
				} else {
					//TODO si c'est un autre readfd
					
				}
			}
			if (FD_ISSET(i, serv.getFdSet(WRITEFD))) {
				//TODO si c'est un writefd
			}
		}
	}
	return 0;
=======
	//main loop starts here
>>>>>>> 9694c2e4b893bd1761d0eaff270df41af69b9987
}

// si on peut read dans le fd du serveur (celui qu'on donne a accept),
// c'est qu'il faut connecter un nouveau fd (qu'on obtient avec le call d'accept)
