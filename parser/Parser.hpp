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

	std::string const				&get_prefix() const;
	std::string const				&get_command() const;
	std::vector<std::string> const	&get_params() const;
	std::string const				&get_trailing() const;
	std::vector<std::string> const	&get_sep_params() const;

	bool parse(const std::string& raw);
};
