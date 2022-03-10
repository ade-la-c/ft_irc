/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Database.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maperrea <maperrea@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 16:27:00 by maperrea          #+#    #+#             */
/*   Updated: 2022/03/10 17:03:10 by maperrea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Databse.hpp"

Database::Database() {
}

Databade::~Database() {
}

Client * Database::get_client(/* Client identifier */) {
	client_map::iterator client = clients.find(/* Client identifier */);
	if (client == clients.end())
		return NULL;
	return &(*client);
}

Channel * Database::get_channel(std::string const & name) {
	channel_map::iterator channel = channels.find(/* channel identifier */);
	if (channel == channels.end())
		return NULL;
	return &(*channel);
}

Client & Database::add_client(/* Client identifier */) {
	std::pair<clients_map::iterator, bool> ret;
	ret = clients.insert(std::make_pair(/* Client identifier */, Client(/* Client identifier */)));
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

Database * Database::get_instance() {
	static Database db;
	return db;
}
