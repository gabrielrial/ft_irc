#include "DCCManager.hpp"
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <sstream>

DCCManager::DCCManager() {}
DCCManager::~DCCManager()
{
	for (size_t i = 0; i < _transfers.size(); ++i)
	{
		cleanup_transfer(_transfers[i]);
	}
}

void DCCManager::handle_dcc_send(Client &sender, const std::string &target,
								 const std::string &filename)
{
	struct stat file_stat;
	if (stat(filename.c_str(), &file_stat) != 0)
	{
		// Send error to client
		return;
	}

	int dcc_socket = create_dcc_socket();
	if (dcc_socket < 0)
		return;

	DCCTransfer transfer(sender.get_nickname(), target, filename, file_stat.st_size);
	transfer.set_socket(dcc_socket);
	transfer.set_is_sender(true);

	// Format: DCC SEND filename ip port filesize
	std::string ip = "127.0.0.1"; // Replace with actual IP
	int port = get_socket_port(dcc_socket);

	std::stringstream ss;
	ss << port;
	std::string port_str = ss.str();
	
	ss.str("");  // Clear the stringstream
	ss << file_stat.st_size;
	std::string size_str = ss.str();

	std::string dcc_request = ":" + sender.get_prefix() +
							  " PRIVMSG " + target +
							  " :\001DCC SEND " + filename + " " +
							  ip + " " + port_str + " " +
							  size_str + "\001\r\n";

	_transfers.push_back(transfer);
}

void DCCManager::check_transfers()
{
	time_t now = time(NULL);

	for (std::vector<DCCTransfer>::iterator it = _transfers.begin();
		 it != _transfers.end();)
	{
		// Check for timeouts
		if (now - it->get_start_time() > TIMEOUT)
		{
			cleanup_transfer(*it);
			it = _transfers.erase(it);
			continue;
		}

		// Process active transfers
		if (!it->is_complete())
		{
			process_transfer(*it);
		}

		++it;
	}
}

void DCCManager::process_transfer(DCCTransfer &transfer)
{
	char buffer[BUFF_SIZE];

	if (transfer.is_sender())
	{
		// Read from file and send
		ssize_t bytes_read = read(transfer.get_socket(), buffer, BUFF_SIZE);
		if (bytes_read > 0)
		{
			ssize_t bytes_sent = send(transfer.get_socket(), buffer, bytes_read, 0);
			if (bytes_sent > 0)
			{
				transfer.add_bytes_transferred(bytes_sent);
			}
		}
	}
	else
	{
		// Receive and write to file
		ssize_t bytes_received = recv(transfer.get_socket(), buffer, BUFF_SIZE, 0);
		if (bytes_received > 0)
		{
			// Write to file
			transfer.add_bytes_transferred(bytes_received);
		}
	}
}

int DCCManager::create_dcc_socket()
{
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
		return -1;

	// Make socket non-blocking
	int flags = fcntl(sock, F_GETFL, 0);
	if (flags >= 0)
	{
		fcntl(sock, F_SETFL, flags | O_NONBLOCK);
	}

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = 0; // Let system assign port

	if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	{
		close(sock);
		return -1;
	}

	if (listen(sock, 1) < 0)
	{
		close(sock);
		return -1;
	}

	return sock;
}

int DCCManager::get_socket_port(int sock)
{
	struct sockaddr_in addr;
	socklen_t len = sizeof(addr);
	if (getsockname(sock, (struct sockaddr*)&addr, &len) < 0)
		return -1;
	return ntohs(addr.sin_port);
}

void DCCManager::handle_dcc_accept(Client &receiver, const std::string &sender,
						const std::string &filename)
{
	// Find the pending transfer
	for (std::vector<DCCTransfer>::iterator it = _transfers.begin();
		 it != _transfers.end(); ++it)
	{
		if (it->get_sender() == sender && 
			it->get_receiver() == receiver.get_nickname() &&
			it->get_filename() == filename)
		{
			// Accept the transfer
			struct sockaddr_in addr;
			socklen_t addr_len = sizeof(addr);
			
			int new_socket = accept(it->get_socket(), (struct sockaddr*)&addr, &addr_len);
			if (new_socket < 0)
			{
				cleanup_transfer(*it);
				_transfers.erase(it);
				return;
			}
			
			it->set_socket(new_socket);
			return;
		}
	}
}

void DCCManager::cleanup_transfer(DCCTransfer &transfer)
{
	if (transfer.get_socket() != -1)
	{
		close(transfer.get_socket());
	}
}