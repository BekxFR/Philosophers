
.PHONY : all sanit test test1 test2 clean fclean re

NAME := philosophers.a
SOFT_NAME := philosophers
SOFT_NAMES := philosopher

CC := gcc
FLAGS := -Wall -Wextra -Werror -I includes/ -pthread -g3
FLAGS2 := -fsanitize=address -g3
SRC_DIR := sources/
OBJ_DIR := objects/
OBJ_DIR2 := objects2/
AR := ar rc
RM := rm
LEAK := valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes
LEAK2 := valgrind --tool=helgrind

BLACK = \033[1;30m
REDBG = \033[30;41m
RED = \033[0;31m
GREEN = \033[0;32m
ORANGE = \033[0;33m
BLUE = \033[0;34m
MAGENTA = \033[0;35m
CYAN = \033[0;36m
NC = \033[0m

SRCS =	philosophers.c \

OBJS = $(SRCS:%.c=%.o)

OBJ = $(addprefix $(OBJ_DIR),$(OBJS))

OBJF := .cache_exists
OBJF2 := .cache_exists2

all : ${SOFT_NAME}

${NAME} : ${OBJ}
	@echo "${BLUE}###${NC}Update de l'archive ${NAME}${BLUE}###${MAGENTA}"
	${AR} ${NAME} ${OBJ}
	@echo "${NC}"

${OBJ_DIR}%.o : %.c | $(OBJF)
	@echo "${BLUE}###${NC}Creation du fichier ${@:%.c=%.o}${BLUE}###${ORANGE}"
	${CC} ${FLAGS} -c $< -o $@
	@echo "${NC}"

${SOFT_NAME} : ${NAME}
	@echo "${BLUE}###${NC}Creation du fichier ${SOFT_NAME}${BLUE}###${ORANGE}"
	${CC} ${NAME} ${FLAGS} -o ${SOFT_NAME}
	@echo "${NC}"

$(OBJF) :
	@mkdir -p ${OBJ_DIR}

sanit : ${SOFT_NAMES}

${SOFT_NAMES} : ${NAME}
	@echo "${BLUE}###${NC}Creation du fichier ${SOFT_NAMES}${BLUE}###${ORANGE}"
	${CC} ${NAME} ${FLAGS2} -o ${SOFT_NAMES}
	@echo "${NC}"

$(OBJF2) :
	@mkdir -p ${OBJ_DIR}

test : all
	./${SOFT_NAME} 3 5000 500 500

tests : sanit
	./${SOFT_NAMES} 3 5000 500 500

test1 : all
	$(LEAK) ./${SOFT_NAME} 3 5000 500 500

test2 : all
	$(LEAK2) ./${SOFT_NAME} 3 5000 500 500

clean : 
	@echo "${RED}###${NC}Nettoyage des fichiers .o${RED}###"
	${RM} -rf ${OBJ_DIR}
	@echo "${GREEN}###${NC}Nettoyage OK${GREEN}###${NC}\n"

fclean : clean
	@echo "${RED}###${NC}Nettoyage d'archives et de Softs${RED}###"
	${RM} -f ${NAME} ${SOFT_NAME} ${NAMES} ${SOFT_NAMES}
	@echo "${GREEN}###${NC}Nettoyage OK${GREEN}###${NC}\n"

re : fclean all
