#include "DCCTransfer.hpp"
#include <unistd.h>

DCCTransfer::DCCTransfer(const std::string &sender, const std::string &receiver,
						 const std::string &filename, size_t filesize)
	: _sender(sender), _receiver(receiver), _filename(filename),
	_filesize(filesize), _socket(-1), _is_sender(false),
	_start_time(time(NULL)), _bytes_transferred(0), _file_fd(-1)
{
}

DCCTransfer::~DCCTransfer()
{
	if (_socket != -1)
		close(_socket);
	if (_file_fd != -1)
		close(_file_fd);
}

const std::string &DCCTransfer::get_sender() const
{
	return _sender;
}

const std::string &DCCTransfer::get_receiver() const
{
	return _receiver;
}

const std::string &DCCTransfer::get_filename() const
{
	return _filename;
}

size_t DCCTransfer::get_filesize() const
{
	return _filesize;
}

int DCCTransfer::get_socket() const
{
	return _socket;
}

bool DCCTransfer::is_sender() const
{
	return _is_sender;
}

time_t DCCTransfer::get_start_time() const
{
	return _start_time;
}

size_t DCCTransfer::get_bytes_transferred() const
{
	return _bytes_transferred;
}

void DCCTransfer::set_socket(int socket)
{
	_socket = socket;
}

void DCCTransfer::set_is_sender(bool is_sender)
{
	_is_sender = is_sender;
}

void DCCTransfer::add_bytes_transferred(size_t bytes)
{
	_bytes_transferred += bytes;
}

bool DCCTransfer::is_complete() const
{
	return _bytes_transferred >= _filesize;
}
