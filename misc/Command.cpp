#include "Command.hpp"
#include <sstream>
#include <algorithm>

Command::Command(const std::string& raw_message) : _raw_message(raw_message) {}

Command::~Command() {}

bool Command::parse()
{
	size_t pos = 0;

	// Skip leading spaces
	while (pos < _raw_message.size() && _raw_message[pos] == ' ')
		pos++;

	// Parse prefix if exists
	if (pos < _raw_message.size() && _raw_message[pos] == ':')
		parsePrefix(pos);

	// Parse command
	parseCommand(pos);
	if (_command.empty())
		return false;

	// Parse parameters and trailing
	parseParams(pos);

	return true;
}

void Command::parsePrefix(size_t& pos)
{
	pos++; // Skip ':'
	size_t space = _raw_message.find(' ', pos);
	if (space != std::string::npos)
	{
		_prefix = _raw_message.substr(pos, space - pos);
		pos = space + 1;
	}
}

void Command::parseCommand(size_t& pos)
{
	// Skip spaces
	while (pos < _raw_message.size() && _raw_message[pos] == ' ')
		pos++;

	size_t space = _raw_message.find(' ', pos);
	if (space != std::string::npos)
	{
		_command = _raw_message.substr(pos, space - pos);
		pos = space + 1;
	}
	else
	{
		_command = _raw_message.substr(pos);
		pos = _raw_message.size();
	}

	// Convert command to uppercase
	std::transform(_command.begin(), _command.end(), _command.begin(), ::toupper);
}

void Command::parseParams(size_t& pos)
{
	while (pos < _raw_message.size())
	{
		// Skip spaces
		while (pos < _raw_message.size() && _raw_message[pos] == ' ')
			pos++;

		if (pos >= _raw_message.size())
			break;

		// Check for trailing
		if (_raw_message[pos] == ':')
		{
			_trailing = _raw_message.substr(pos + 1);
			break;
		}

		// Parse regular parameter
		size_t space = _raw_message.find(' ', pos);
		if (space != std::string::npos)
		{
			_params.push_back(_raw_message.substr(pos, space - pos));
			pos = space + 1;
		}
		else
		{
			_params.push_back(_raw_message.substr(pos));
			break;
		}
	}
}

// Getter functions implementation
std::string Command::getPrefix() const 
{
	return _prefix;
}

std::string Command::getCommand() const 
{
	return _command;
}

const std::vector<std::string>& Command::getParams() const 
{
	return _params;
}

std::string Command::getTrailing() const 
{
	return _trailing;
}

std::string Command::getRawMessage() const 
{
	return _raw_message;
}
