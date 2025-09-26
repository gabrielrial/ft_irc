#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "../lib_irc.hpp"
#include "Bot.hpp"

void sendMessage(int socket_fd, const std::string &msg)
{
	std::string finalMsg = msg + "\r\n";
	send(socket_fd, finalMsg.c_str(), finalMsg.size(), 0);
}

int main()
{
	Bot bot;

	bot.init_connection();
	
	if (bot.run_bot() == -1)
		return (-1);

	char buffer[512];
	while (true)
	{
		memset(buffer, 0, sizeof(buffer));
		int bytes_read = recv(socket_fd, buffer, sizeof(buffer) - 1, 0);
		if (bytes_read <= 0)
			break;
		buffer[bytes_read] = '\0';

		std::string line(buffer);
		std::cout << "<< " << line << std::endl;
		std::cout << "line" << line << std::endl;
		handle_client_data(fd, buffer, bytes_read, lineBuffer[fd]);
		if (line.find("!jock"))
		{
			std::cout << "found" << std::endl;
			sendMessage(socket_fd, "PRIVMSG grial :Which is the scariest plant? Bamboo!!");
		}
	}

	close(socket_fd);
	return 0;
}
