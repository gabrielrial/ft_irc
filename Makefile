NAME = ircserv

CPP = c++

CPPFLAGS = -Wall -Wextra -Werror -std=c++98

SRC = main.cpp \
		src/server/Server.cpp \
		src/parser/RawTextLine.cpp \
 		src/client/Client.cpp \
		src/channel/Channel.cpp \
		src/err_rpl/err.cpp \
		src/err_rpl/rpl.cpp \
		src/cmds/run_cmds.cpp \
		src/cmds/channel/cmd_list.cpp \
		src/cmds/channel/cmd_part.cpp \
		src/cmds/channel/cmd_join.cpp \
		src/cmds/channel/cmd_invite.cpp \
		src/cmds/channel/cmd_kick.cpp \
		src/cmds/channel/cmd_topic.cpp \
		src/cmds/channel/cmd_mode.cpp \
		src/cmds/connection/cmd_nick.cpp \
		src/cmds/connection/cmd_pass.cpp \
		src/cmds/connection/cmd_user.cpp \
		src/cmds/connection/cmd_pong.cpp \
		src/cmds/connection/cmd_quit.cpp \
		src/cmds/messaging/cmd_privmsg.cpp \
		src/cmds/messaging/cmd_notice.cpp \
		src/cmds/messaging/messaging_utils.cpp \
		src/cmds/user_info/cmd_who.cpp \
		src/cmds/user_info/cmd_names.cpp \
		src/cmds/user_info/cmd_names_join.cpp \

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