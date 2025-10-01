NAME = bot

CPP = c++

CPPFLAGS = -Wall -Wextra -Werror -std=c++98

SRC = main.cpp \
		Bot.cpp \
		bot_cmds/bot_cmds.cpp \
		bot_cmds/cmds/bot_filter.cpp \
		bot_cmds/cmds/cmd_invite.cpp \
		bot_cmds/cmds/cmd_spam.cpp \
		bot_cmds/cmds/cmd_joke.cpp \
		bot_cmds/cmds/cmd_privmsg.cpp \
		parser/Parser.cpp

OBJ = $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CPP) $(CPPFLAGS) $(OBJ) -o $(NAME)

%.o: %.cpp
	$(CPP) $(CPPFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all