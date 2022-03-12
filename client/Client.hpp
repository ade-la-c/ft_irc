/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maperrea <maperrea@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 19:21:14 by maperrea          #+#    #+#             */
/*   Updated: 2022/03/12 16:26:40 by maperrea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

class Client {

	public:

		Client();
		Client(uint32_t ip);
		Client(Client const & cpy);
		~Client();

		char buf[512];
		ssize_t length;

	private:

		uint32_t ip;
}

#endif
