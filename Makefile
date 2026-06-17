NAME = ircserv
CXX = c++
FLAGS = -Wall -Wextra -Werror -std=c++98
SRC = main.cpp Server/Server.cpp Client/Client.cpp Channel/Channel.cpp Commands/invite.cpp Commands/join.cpp \
		Commands/kick.cpp Commands/manual.cpp Commands/mode.cpp Commands/nick.cpp Commands/part.cpp Commands/pass.cpp \
		Commands/privmsg.cpp Commands/topic.cpp Commands/user.cpp

OBJ = $(SRC:.cpp =.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(FLAGS) $(OBJ) -o $(NAME)

%.o: %.cpp
	$(CXX) $(FLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)
	
re: fclean all
