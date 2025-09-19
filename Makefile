NAME = ircserv

CPP = c++

CPPFLAGS = -Wall -Wextra -Werror -std=c++98

SRC = main.cpp \
		server/Server.cpp \
		parser/Parser.cpp \
 		client/Client.cpp \
		channel/Channel.cpp \
		cmds/run_cmds.cpp \
		cmds/channel/cmd_list.cpp \
		cmds/channel/cmd_part.cpp \
		cmds/channel/cmd_join.cpp \
		cmds/channel/cmd_invite.cpp \
		cmds/connection/cmd_quit.cpp \
		cmds/channel/cmd_topic.cpp \
		cmds/channel/cmd_mode.cpp \
		cmds/connection/cmd_nick.cpp \
		cmds/connection/cmd_pass.cpp \
		cmds/connection/cmd_user.cpp \
		cmds/messaging/cmd_privmsg.cpp \
		cmds/user_info/cmd_who.cpp \

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