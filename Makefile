# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ade-la-c <ade-la-c@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/03/09 11:48:49 by ade-la-c          #+#    #+#              #
#    Updated: 2022/03/25 07:00:44 by maperrea         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			= ircserv

SRCS			= ${shell find */*.cpp} main.cpp

OBJS			= ${SRCS:.cpp=.o}

CXX				= c++ #-g -fsanitize=address

CXXFLAGS		= -Wall -Werror -Wextra -std=c++98 -Wno-unused-variable \
				# -g -fsanitize=address

RM				= rm -f

all:			${NAME}

${NAME}:		${OBJS}
				${CXX} ${CXXFLAGS} ${OBJS} -o ${NAME}

clean:
				${RM} ${OBJS}

fclean:			clean
				${RM} ${NAME}

re:				fclean all

.PHONY:			all clean fclean re
