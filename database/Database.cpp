#include "Database.hpp"

Database::Database() {
}

Database::~Database() {
}

Client * Database::get_client(int socket) {

	client_map::iterator client = clients.find(socket);

	if (client == clients.end())
		return NULL;
	return &(client->second);
}

Channel * Database::get_channel(std::string const & name) {

	channel_map::iterator channel = channels.find(name);

	if (channel == channels.end())
		return NULL;
	return &(channel->second);
}

Client * Database::add_client(int socket) {
	std::pair<client_map::iterator, bool> ret;
	ret = clients.insert(std::make_pair(socket, Client(socket)));
	if (!ret.second)
		throw Database::already_exists();
	return &(ret.first->second);
}

Channel * Database::add_channel(std::string const & name) {
	std::pair<channel_map::iterator, bool> ret;
	ret = channels.insert(std::make_pair(name, Channel(name)));
	if (!ret.second)
		throw Database::already_exists();
	return &(ret.first->second);
}

void	remove_client(int socket) {
	client.erase(socket);
}

void	Database::add_response(response_pair response) {
	responses.push_back(response);
}

response_pair	Database::next_response() {
	response_pair tmp = responses.front();
	responses.pop_front();
	return tmp;
}

bool Database::init(int argc, char **argv) {

	if (argc != 3)
		return false;

	password = argv[1];
	port = argv[2];

	char name[512];
	gethostname(name, 512);
	hostname = name;
	return true;
}

Database * Database::get_instance() {

	static Database db;
	return &db;
}
