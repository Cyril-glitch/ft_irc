NAME = ircserv

CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98

OBJ_DIR = obj

SRC = src/main.cpp \
	  src/mainUtils.cpp \
	  src/Server.cpp \
	  src/Client.cpp

OBJ = $(addprefix $(OBJ_DIR)/, $(notdir $(SRC:.cpp=.o)))

VPATH = src

all : $(NAME)

$(NAME) : $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

$(OBJ_DIR)/%.o : %.cpp
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all clean fclean re it val

it : all
	./ircserv 6667 test 

clean :
	rm -rf $(OBJ_DIR)

fclean : clean
	rm -rf $(BIN_DIR)
	rm -rf $(NAME)

re: fclean all