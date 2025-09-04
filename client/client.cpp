#include <iostream>
#include <string>
#include <cstring> // memset
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> // sockaddr_in
#include <arpa/inet.h>  // inet_pton
#include <unistd.h>     // close()

#define PORT 54000
#define SERVER_IP "127.0.0.2"

int main()
{
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1)
    {
        std::cerr << "No se pudo crear el socket\n";
        return 1;
    }

    sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);

    if (connect(clientSocket, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)    {
        std::cerr << "Error al conectar con el servidor\n";
        close(clientSocket);
        return 1;
    }

    std::cout << "Conectado al servidor!\n";

    std::string msg;
    char buffer[1024];

    while (true)
    {
        std::cout << "Tú: ";
        std::getline(std::cin, msg);

        if (msg == "/quit")
            break;

        send(clientSocket, msg.c_str(), msg.size(), 0);

        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived <= 0)
        {
            std::cout << "Servidor desconectado o error.\n";
            break;
        }

        buffer[bytesReceived] = '\0';
        std::cout << "Servidor: " << buffer << std::endl;
    }

    close(clientSocket);
    return 0;
}
