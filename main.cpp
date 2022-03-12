/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maperrea <maperrea@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/12 16:03:56 by maperrea          #+#    #+#             */
/*   Updated: 2022/03/12 16:14:54 by maperrea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"

int main(int argc, char **argv) {
	Database * db = Database.get_instance();
	if (!db->init(argc, argv)) {
		error("Wrong number of arguments (need two)");
		return 1;
	}
	//main loop starts here
}
