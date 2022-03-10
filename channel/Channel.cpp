/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maperrea <maperrea@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 19:25:46 by maperrea          #+#    #+#             */
/*   Updated: 2022/03/10 19:27:01 by maperrea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel() : name("") {
}

Channel::Channel(std::string const & name) : name(name) {
}

Channel::Channel(Channel const & cpy) : name(cpy.name) {
}

Channel::~Channel() {
}
