# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/30 11:17:47 by ulmagner          #+#    #+#              #
#    Updated: 2025/07/18 16:05:18 by ulmagner         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= ircserv
SRCS		= main.cpp Serv.cpp ACmd.cpp PassCmd.cpp
INCS		= Serv.hpp ACmd.hpp PassCmd.hpp

SDIRS		= srcs
IDIRS		= includes
ODIRS		= objs

SFILES		= $(addprefix $(SDIRS)/,$(SRCS))
IFILES		= $(addprefix $(IDIRS)/,$(INCS))
OFILES		= $(addprefix $(ODIRS)/,$(patsubst %.cpp,%.o,$(SRCS)))

CXX			= c++
CPPFLAGS	= -Wall -Wextra -Werror -std=c++98
HEADERS		= -I$(IDIRS)
MAKEFLAGS	+= -s
MAKE		= make

all:		$(NAME)

$(ODIRS):
	mkdir -p $(ODIRS)

$(ODIRS)/%.o:	$(SDIRS)/%.cpp $(IFILES) | $(ODIRS)
	$(CXX) $(CPPFLAGS) $(HEADERS) -c $< -o $@

$(NAME):		$(OFILES)
	$(CXX) -o $(NAME) $(CPPFLAGS) $(HEADERS) $(OFILES)

clean:
	rm -f $(OFILES)
	@rm -rf $(ODIRS)

fclean:	clean
	rm -f $(NAME)

re:	fclean all

.PHONY: all bonus fclean clean re test
