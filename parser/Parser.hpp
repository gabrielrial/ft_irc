#pragma once

# include <string>
# include <vector>
# include <iostream>

class RawTextLine
{
private:
	std::string					_prefix;
	std::string					_command;
	std::vector<std::string>	_params;
	std::string					_trailing;
	std::vector<std::string>	_separ_params;

public:
	RawTextLine();
	RawTextLine(const std::string& raw); //parse immediately
	~RawTextLine();
	RawTextLine(const RawTextLine &copy);
	RawTextLine	&operator=(const RawTextLine &copy);

	std::string const				&getPrefix() const;
	std::string const				&getCommand() const;
	std::vector<std::string> const	&getParams() const;
	std::string const				&getTrailing() const;
	std::vector<std::string> const	&getSepParams() const;

	bool parse(const std::string& raw);
};
