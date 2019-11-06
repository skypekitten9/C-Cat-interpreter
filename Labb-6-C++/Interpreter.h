#pragma once
#include <ostream>
#include <vector>
#include <regex>
#include <sstream>
#include <map>
#include <bitset>

class Interpreter
{
	std::ostream& os;
	size_t position = 0;
	const std::string ETX = "\u0003";
	std::map<std::string, int> variableMap;
	enum Config { dec, hex, bin };
	Config config = dec;
	
public:
	Interpreter(std::ostream& out_stream);
	void evaluate(const std::vector<std::string>& tokens);
	std::vector<std::string> tokenize(std::string input);

private:
	std::string peek(const std::vector<std::string>& tokens);
	std::string peek(const std::vector<std::string>& tokens, int steps);
	void consume(const std::vector<std::string>& tokens, const std::string& token);
	void parse_ConfigStmt(const std::vector<std::string>& tokens);
	void parse_PrintStmt(const std::vector<std::string>& tokens);
	void parse_AssgStmt(const std::vector<std::string>& tokens);
	int parse_MathExp(const std::vector<std::string>& tokens);
	int parse_SumExp(const std::vector<std::string>& tokens);
	int parse_ProductExp(const std::vector<std::string>& tokens);
	int parse_PrimaryExp(const std::vector<std::string>& tokens);
	bool isInteger(std::string& token);
	bool isVariable(std::string& token);
};


