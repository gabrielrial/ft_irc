// dcc/DCCManager.hpp

#pragma once
#pragma once
#include <vector>
#include <string>
#include <ctime>
#include "DCCTransfer.hpp"

// Forward declarations
class Client;
class DCCTransfer;

class DCCManager
{
private:
	std::vector<DCCTransfer>	_transfers;
	static const size_t			BUFF_SIZE = 8192;
	static const time_t			TIMEOUT = 30; // 30 seconds timeout
	void						cleanup_transfer(DCCTransfer &transfer);

public:
	DCCManager();
	~DCCManager();

	//void	initiate_transfer(Client &sender, Client &receiver,
	//						const std::string &filename);
	//void	accept_transfer(const std::string &sender,
	//						const std::string &filename);
	void	check_transfers();
	//void	handle_timeout();
	void	handle_dcc_send(Client &sender, const std::string &target,
							const std::string &filename);
	void	handle_dcc_accept(Client &receiver, const std::string &sender,
							const std::string &filename);
	int		create_dcc_socket();
	int		get_socket_port(int sock);
	void	process_transfer(DCCTransfer &transfer);
};