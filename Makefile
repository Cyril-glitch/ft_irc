NAME = ircserv

CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98
CPPFLAGS = -Iinc
OBJ_DIR = obj

CHANNEL_SRC = src/Channel.cpp
CLIENT_SRC = src/Client.cpp
COMMAND_SRC = src/Command.cpp
SERVER_SRC = src/Server.cpp

SRC = $(CHANNEL_SRC) $(CLIENT_SRC) $(COMMAND_SRC) $(SERVER_SRC)
OBJ = $(addprefix $(OBJ_DIR)/, $(notdir $(SRC:.cpp=.o)))

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

.PHONY: all clean fclean re

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all
