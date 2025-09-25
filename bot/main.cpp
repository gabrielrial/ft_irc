#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>

void sendMessage(int socket_fd, const std::string &msg)
{
	std::string finalMsg = msg + "\r\n";
	send(socket_fd, finalMsg.c_str(), finalMsg.size(), 0);
}

int main()
{
	std::string server = "localhost"; // ejemplo
	int port = 6667;
	std::string nick = "Samuel_L_Jackson";
	std::string user = "MiBot";

	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in servAddr{};
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(port);

	hostent *host = gethostbyname(server.c_str());
	memcpy(&servAddr.sin_addr, host->h_addr, host->h_length);

	if (connect(socket_fd, (sockaddr *)&servAddr, sizeof(servAddr)) < 0)
	{
		perror("Error conectando");
		return 1;
	}

	sendMessage(socket_fd, "PASS 42");
	sendMessage(socket_fd, "NICK " + nick);
	sendMessage(socket_fd, "USER " + user + " 0 * :" + user);

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
		if (line.find("!jock"))
		{
			std::cout << "found" << std::endl;
			sendMessage(socket_fd, "PRIVMSG grial :Which is the scariest plant? Bamboo!!");
		}
	}

	close(socket_fd);
	return 0;
}
