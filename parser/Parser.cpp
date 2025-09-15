#include "Parser.hpp"

RawTextLine::RawTextLine() :
	_prefix(""),
	_command(""),
	_params(),
	_trailing(""),
	_separ_params()
{}

RawTextLine::RawTextLine(const std::string& raw) :
	_prefix(""),
	_command(""),
	_params(),
	_trailing(""),
	_separ_params()
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
		this->_separ_params = copy._separ_params;
	}
	return (*this);
}

std::string const	&RawTextLine::get_prefix() const
{
	return (this->_prefix);
}

std::string const	&RawTextLine::get_command() const
{
	return (this->_command);
}

std::vector<std::string> const	&RawTextLine::get_params() const
{
	return (this->_params);
}

std::string const	&RawTextLine::get_trailing() const
{
	return (this->_trailing);
}

std::vector<std::string> const	&RawTextLine::get_sep_params() const
{
	return (this->_separ_params);
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

	_separ_params.clear();
	for (std::vector<std::string>::const_iterator it = _params.begin(); it != _params.end(); ++it)
	{
		size_t start = 0;
		size_t end = 0;
		while ((end = it->find(',', start)) != std::string::npos)
		{
			_separ_params.push_back(it->substr(start, end - start));
			start = end + 1;
		}
		_separ_params.push_back(it->substr(start));
	}
	return true;
}
