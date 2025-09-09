#include "parser.hpp"

RawTextLine::RawTextLine() :
	_prefix(""),
	_command(""),
	_params(),
	_trailing("")
{}

RawTextLine::RawTextLine(const std::string& raw) :
	_prefix(""),
	_command(""),
	_params(),
	_trailing("")
{
	parse(raw);
}

RawTextLine::~RawTextLine()
{}

RawTextLine::RawTextLine(const RawTextLine &copy)
{
	*this = copy;
}

RawTextLine &RawTextLine::operator=(const RawTextLine &copy)
{
	if (this != &copy)
	{
		this->_prefix = copy._prefix;
		this->_command = copy._command;
		this->_params = copy._params;
		this->_trailing = copy._trailing;
	}
	return (*this);
}

std::string const	&RawTextLine::getPrefix() const
{
	return (this->_prefix);
}

std::string const	&RawTextLine::getCommand() const
{
	return (this->_command);
}

std::vector<std::string> const	&RawTextLine::getParams() const
{
	return (this->_params);
}

std::string const	&RawTextLine::getTrailing() const
{
	return (this->_trailing);
}

bool	RawTextLine::parse(const std::string& raw)
{
	size_t pos = 0;
	//std::cout << "Parsing raw message: '" << raw << "'" << std::endl;

	if (raw[0] == ':')
	{
		size_t space = raw.find(' ');
		if (space == std::string::npos) 
			return false;
		_prefix = raw.substr(1, space - 1);
		pos = space + 1;
	}

	size_t space = raw.find(' ', pos);
	if (space == std::string::npos)
	{
		_command = raw.substr(pos);
		return true;
	}
	_command = raw.substr(pos, space - pos);
	pos = space + 1;

	while (pos < raw.length())
	{
		while (pos < raw.length() && raw[pos] == ' ')
			pos++;
		if (pos >= raw.length())
			break;
		if (raw[pos] == ':')
		{
			_trailing = raw.substr(pos + 1);
			break;
		}
		space = raw.find(' ', pos);
		if (space == std::string::npos)
		{
			_params.push_back(raw.substr(pos));
			break;
		}
		_params.push_back(raw.substr(pos, space - pos));
		pos = space + 1;
	}
	return true;
}