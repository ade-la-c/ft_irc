/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   database.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maperrea <maperrea@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 16:09:27 by maperrea          #+#    #+#             */
/*   Updated: 2022/03/10 19:10:37 by maperrea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DATABASE_HPP
# define DATABASE_HPP

# include <map>
# include "Client.hpp"
# include "Channel.hpp"

// Singleton Database class
class Database {
	private:

		// private constructor -> can only get instance through get_instance;
		Database();

		// disallow copy and assignation, unimplemented
		Database(Database const & cpy);
		Database & operator=(Database const & rhs);

		typedef std::map<uint32_t, Client> client_map;
		typedef std::map<std::string, Channel> channel_map;

		client_map clients;
		channel_map channels;

	public:

		class already_exists : public std::exception {
			public:
				virtual char const * what() const throw() {
					return "Element already exists";
				}
		}

		~Database();

		// returns the client/channel or NULL if it doesn't exist
		Client * get_client(uint32_t ip);
		Channel * get_channel(std::string const & name);

		// creates a new client/channel & returns it
		Client & add_client(uint32_t ip); //throws exception if element exists
		Channel & add_channel(std::string const & name); //throws exception if element exists

		// only way to get the instance
		static Database & get_instance();
}

#endif
