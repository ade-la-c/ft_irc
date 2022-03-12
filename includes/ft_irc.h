/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maperrea <maperrea@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/12 16:04:12 by maperrea          #+#    #+#             */
/*   Updated: 2022/03/12 16:17:12 by maperrea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_IRC_H
# define FT_IRC_H

# include "Database.hpp"
# include "Client.hpp"
# include "Channel.hpp"

void error(std::string const & str);

void parse_input(Client & client);

template <typename Iter>
void send_response(Iter begin, Iter end);
//#include "response.hpp" TODO

#endif
