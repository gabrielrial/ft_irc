#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <string>
#include <vector>

class Command
{
private:
	std::string _prefix;
	std::string _command;
	std::vector<std::string> _params;
	std::string _trailing;
	std::string _raw_message;
	void parsePrefix(size_t& pos);
	void parseCommand(size_t& pos);
	void parseParams(size_t& pos);

public:
	Command(const std::string& raw_message);
	~Command();

	bool parse();
	
	// Getters
	std::string getPrefix() const;
	std::string getCommand() const;
	const std::vector<std::string>& getParams() const;
	std::string getTrailing() const;
	std::string getRawMessage() const;
};

#endif
