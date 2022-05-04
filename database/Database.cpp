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

Client * Database::get_client(std::string const & nickname) {

	pclient_map::iterator client = pclients.find(nickname);

	if (client == pclients.end())
		return NULL;
	return client->second;
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

Client * Database::add_pclient(Client * client) {
	std::pair<pclient_map::iterator, bool> ret;
	ret = pclients.insert(std::make_pair(client->nickname, client));
	if (!ret.second)
		throw Database::already_exists();
	return ret.first->second;
}

Channel * Database::add_channel(std::string const & name) {
	std::pair<channel_map::iterator, bool> ret;
	ret = channels.insert(std::make_pair(name, Channel(name)));
	if (!ret.second)
		throw Database::already_exists();
	return &(ret.first->second);
}

void	Database::remove_client(int socket) {
	if (!clients.count(socket))
		return ;
	Client & client = clients.at(socket);
	if (client.registered) {
		pchannel_map::iterator begin = client.subscribed_channels.begin();
		pchannel_map::iterator end = client.subscribed_channels.end();
		while (begin != end) {
			leave_channel(client, begin->second);
			begin++;
		}
		pclients.erase(client.nickname);
	}
	clients.erase(socket);
}

void	Database::remove_channel(Channel * chan) {
	pclient_map::iterator begin = chan->subscribed_clients.begin();
	pclient_map::iterator end = chan->subscribed_clients.end();
	while (begin != end) {
		begin->second->subscribed_channels.erase(chan->name);
		begin ++;
	}
	channels.erase(chan->name);
}

bool Database::init(int argc, char **argv) {

	if (argc != 3 && argc != 4)
		return false;

	port = argv[1];
	password = argv[2];

	debug = false;
	if (argc == 4)
		if (std::string(argv[3]) == "-d")
			debug = true;

	char name[512];
	gethostname(name, 512);
	hostname = name;
	return true;
}

Database * Database::get_instance() {

	static Database db;
	return &db;
}
