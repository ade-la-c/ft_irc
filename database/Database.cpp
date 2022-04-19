#include "Database.hpp"

Database::Database() {
}

Database::~Database() {
}

Client * Database::get_client(int socket) {

	client_map::iterator client = clients.find(socket);

	if (client == clients.end())
		return NULL;
	return &(*client);
}

Channel * Database::get_channel(std::string const & name) {

	channel_map::iterator channel = channels.find(name);

	if (channel == channels.end())
		return NULL;
	return &(*channel);
}

Client & Database::add_client(int socket) {

	std::pair<clients_map::iterator, bool> ret;

	ret = clients.insert(std::make_pair(socket, Client(socket)));
	if (!ret.second)
		throw Database::already_exists();
	return *ret.first;
}

Channel & Database::add_channel(std::string const & name) {

	std::pair<channels_map::iterator, bool> ret;

	ret = channels.insert(std::make_pair(name, channel(name)));
	if (!ret.second)
		throw Database::already_exists();
	return *ret.first;
}

bool Database::init(int argc, char **argv) {

	if (argc != 3)
		return false;

	password = argv[1];
	port = argv[2];
}

Database * Database::get_instance() {

	static Database db;
	return &db;
}
