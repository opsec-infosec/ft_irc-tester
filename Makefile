# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dfurneau <dfurneau@student.42abudhabi.ae>  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/07 07:25:25 by dfurneau          #+#    #+#              #
#    Updated: 2023/04/15 00:52:29 by dfurneau         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	client-tester

SSRCS		=	./src/main.cpp \
				./src/parse.cpp \
				./src/client-tester.cpp \


SOBJS		=	${SSRCS:.cpp=.o}
CXX			=	g++

CXXFLAGS	=	-Wall -Wextra -Werror -std=c++2a -Ofast

LIBS		=	-pthread

RM			=	rm -f

all:		${NAME}

${NAME}:	${SOBJS}

			${CXX} ${CXXFLAGS} ${LIBS} ${SOBJS} -o ${NAME}

clean:

			${RM} ${NAME}

fclean:		clean

			${RM} ${SOBJS}

re:			fclean all

.PHONY : all clean re fclean
