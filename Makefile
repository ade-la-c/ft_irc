# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ade-la-c <ade-la-c@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/03/09 11:48:49 by ade-la-c          #+#    #+#              #
#    Updated: 2022/05/04 16:12:46 by ade-la-c         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			= ircserv

INCDIR			= includes

SRCDIR			= sources

OBJDIR			= objects

SRCS			= $(shell find *.cpp **/*.cpp)

OBJS			= $(addprefix $(OBJDIR)/, $(SRCS:.cpp=.o))

CXX				= c++

CXXFLAGS		= -Wall -Werror -Wextra -std=c++98 -Wno-unused-variable \
#				-g -fsanitize=address

RM				= rm -f

$(OBJDIR)/%.o: %.cpp
				mkdir -p $(@D)
				$(CXX) $(CXXFLAGS) -I $(INCDIR) -c $^ -o $@

all:			$(NAME)

$(NAME):		$(OBJS)
				@echo $(SRCS)
				$(CXX) $(CXXFLAGS) -I $(INCDIR) $(OBJS) -o $(NAME)

clean:
				$(RM) -r $(OBJDIR)

fclean:			clean
				$(RM) $(NAME)

re:				fclean all

.PHONY:			all clean fclean re
