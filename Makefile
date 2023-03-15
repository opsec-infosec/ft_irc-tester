# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dfurneau <dfurneau@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/07 07:25:25 by dfurneau          #+#    #+#              #
#    Updated: 2023/03/15 14:51:15 by dfurneau         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	client-tester

SSRCS		=	./client-tester.cpp ./client-message.cpp


SOBJS		=	${SSRCS:.cpp=.o}
CXX			=	c++
CXXFLAGS	=	-Wall -Wextra -Werror -std=c++20 ${D} ${OPTIMIZE} #${DEBUG}
LIBS		=	-pthread
DEBUG		=	-g3
OPTIMIZE	=	-O2

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
