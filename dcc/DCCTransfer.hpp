#pragma once
#include "../lib_irc.hpp"
#include <ctime>

class DCCTransfer
{
private:
	std::string	_sender;
	std::string	_receiver;
	std::string	_filename;
	size_t		_filesize;
	std::string	_ip;
	int			_socket;
	bool		_is_sender;
	std::time_t	_start_time;
	size_t		_bytes_transferred;
	int			_file_fd;

public:
	DCCTransfer(const std::string &sender, const std::string &receiver,
				const std::string &filename, size_t filesize);
	~DCCTransfer();

	// Getter methods
	const std::string	&get_sender() const;
	const std::string	&get_receiver() const;
	const std::string	&get_filename() const;
	size_t				get_filesize() const;
	int					get_socket() const;
	bool				is_sender() const;
	time_t				get_start_time() const;
	size_t				get_bytes_transferred() const;

	// Setter methods
	void				set_socket(int socket);
	void				set_is_sender(bool is_sender);
	//void				set_port(int port);
	void				add_bytes_transferred(size_t bytes);
	bool				is_complete() const;
	
	// Additional getter
	//int get_port() const;
};