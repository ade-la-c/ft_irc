/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maperrea <maperrea@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 19:22:27 by maperrea          #+#    #+#             */
/*   Updated: 2022/03/10 19:24:14 by maperrea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client() : ip(0) {
}

Client::Client(uint32_t ip) : ip(ip) {
}

Client::Client(Client const & cpy) : ip(cpy.ip) {
}

Client::~Client() {
}
