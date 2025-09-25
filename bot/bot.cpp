#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>

void sendMessage(int sock, const std::string &msg)
{
	std::string finalMsg = msg + "\r\n";
	send(sock, finalMsg.c_str(), finalMsg.size(), 0);
}

()
{
	std::string server = "irc.libera.chat";
	int port = 6667;
	std::string nick = "MiBot";
	std::string user = "MiBot";
	std::string channel = "#prueba42";

	int sock = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in servAddr{};
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(port);

	hostent *host = gethostbyname(server.c_str());
	memcpy(&servAddr.sin_addr, host->h_addr, host->h_length);

	// 2. Conectar
	if (connect(sock, (sockaddr *)&servAddr, sizeof(servAddr)) < 0)
	{
		perror("Error conectando");
		return 1;
	}

	sendMessage(sock, "NICK " + nick);
	sendMessage(sock, "USER " + user + " 0 * :" + user);
	char buffer[512];
	while (true)
	{
		memset(buffer, 0, sizeof(buffer));
		int n = recv(sock, buffer, sizeof(buffer) - 1, 0);
		if (n <= 0)
			break;

		std::string line(buffer);
		std::cout << "<< " << line;

		if (line.find("PING") == 0)
		{
			sendMessage(sock, "PONG :" + line.substr(5));
		}

		if (line.find("376") != std::string::npos)
		{ // End of MOTD
			sendMessage(sock, "JOIN " + channel);
		}

		if (line.find("PRIVMSG " + channel + " :!hola") != std::string::npos)
		{
			sendMessage(sock, "PRIVMSG " + channel + " :Hola, soy un bot :)");
		}
	}

	close(sock);
	return 0;
}
