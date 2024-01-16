
NAME	= webserv

SRCS	= $(wildcard srcs/*.cpp)
			
OBJDIR	= obj
CC		= g++
CFLAGS	= -Wall -Wextra -Werror -std=c++98

OBJS	= $(addprefix ${OBJDIR}/, $(addsuffix .o, $(basename ${SRCS})))

TOTAL_FILES = $(words $(SRCS))
COUNT = 0

GREEN=\033[0;32m
NC=\033[0m # No Color

${OBJDIR}/%.o: %.cpp
		@mkdir -p $(dir $@)
		$(eval COUNT=$(shell echo $$(($(COUNT)+1))))
		@echo "$(GREEN)$$(echo $(COUNT)*100/$(TOTAL_FILES) | bc)%$(NC)"
		@${CC} ${CFLAGS} -c -o $@ $<

${NAME}:	${OBJS}
		${CC} ${OBJS} -o ${NAME}

all:		${NAME}

clean:
			rm -rf ${OBJDIR}

fclean:		clean
			rm -f ${NAME}

re:			fclean all

.PHONY:		all clean fclean re NAME