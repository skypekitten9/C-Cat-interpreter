#include "Interpreter.h"
#include <numeric>
Interpreter::Interpreter(std::ostream& out_stream) : os(out_stream){}

void Interpreter::evaluate(const std::vector<std::string>& tokens)
{
	position = 0;
	std::string nextToken = peek(tokens);
	if (nextToken == "config")
	{  
		consume(tokens, nextToken);
		parse_ConfigStmt(tokens);
	}
	else if (nextToken == "print")
	{
		consume(tokens, nextToken);
		parse_PrintStmt(tokens);
	}
	else if (isVariable(nextToken))
	{
		consume(tokens, nextToken);
		parse_AssgStmt(tokens);
	}
	else
	{
		throw std::runtime_error("expected config, variable or print statement");
	}
}

std::vector<std::string> Interpreter::tokenize(std::string input)
{
	std::stringstream checkStream(input);
	std::string output = "";
	std::vector<std::string> tokens;
	while (getline(checkStream, output, ' '))
	{
		tokens.push_back(output);
	}

	return tokens;
}

std::string Interpreter::peek(const std::vector<std::string>& tokens)
{
	return peek(tokens, 0);
}

std::string Interpreter::peek(const std::vector<std::string>& tokens, int steps)
{
	if (position + steps >= tokens.size()) return ETX;
	return tokens[position + steps];
}

void Interpreter::consume(const std::vector<std::string>& tokens, const std::string& token)
{
	std::string nextToken = peek(tokens);
	if (nextToken == ETX) throw std::runtime_error("Past last token consumed.\n");
	if (nextToken != token) throw std::runtime_error("Unable to consume token: " + token + "\n");
	++position;
}

void Interpreter::parse_ConfigStmt(const std::vector<std::string>& tokens)
{
	std::string nextToken = peek(tokens);
	if (nextToken == "dec")
	{
		config = dec;
	}
	else if (nextToken == "hex")
	{
		config = hex;
	}
	else if (nextToken == "bin")
	{
		config = bin;
	}
	else
	{
		throw std::runtime_error("Expected: dec, hex or bin\n");
	}
}

void Interpreter::parse_PrintStmt(const std::vector<std::string>& tokens)
{
	if (config == dec)
	{
		os << parse_MathExp(tokens);
	}
	else if (config == hex)
	{
		os << "0x";
		os << std::hex << parse_MathExp(tokens);
	}
	else if (config == bin)
	{
		os << std::bitset<32>(parse_MathExp(tokens));
	}
	else
	{
		throw std::runtime_error("Invalid value: config\n");
	}
	os << std::endl;
}

void Interpreter::parse_AssgStmt(const std::vector<std::string>& tokens)
{
	std::string nextToken = peek(tokens);
	if (nextToken == "=")
	{
		consume(tokens, nextToken);
		std::string key = peek(tokens, -2);
		variableMap[key] = parse_MathExp(tokens);
	}
	else throw std::runtime_error("Expected: =\n");
}

int Interpreter::parse_MathExp(const std::vector<std::string>& tokens)
{
	return parse_SumExp(tokens);
}

int Interpreter::parse_SumExp(const std::vector<std::string>& tokens)
{
	int value = parse_ProductExp(tokens);
	std::string nextToken = "";
	while (true)
	{
		nextToken = peek(tokens);
		if (nextToken == "+")
		{
			consume(tokens, nextToken);
			value += parse_ProductExp(tokens);
		}
		else if (nextToken == "-")
		{
			consume(tokens, nextToken);
			value -= parse_ProductExp(tokens);
		}
		else
		{
			break;
		}
	}
	return value;
}

int Interpreter::parse_ProductExp(const std::vector<std::string>& tokens)
{
	int value = parse_PrimaryExp(tokens);
	std::string nextToken = "";
	while (true)
	{
		nextToken = peek(tokens);
		if (nextToken == "*")
		{
			consume(tokens, nextToken);
			value *= parse_PrimaryExp(tokens);
		}
		else if (nextToken == "/")
		{
			consume(tokens, nextToken);
			value /= parse_PrimaryExp(tokens);
		}
		else
		{
			break;
		}
	}
	return value;
}

int Interpreter::parse_PrimaryExp(const std::vector<std::string>& tokens)
{
	int value = 0;
	std::string nextToken = peek(tokens);
	if (isInteger(nextToken))
	{
		value = std::stoi(nextToken);
		consume(tokens, nextToken);
	}
	else if (isVariable(nextToken))
	{
		value = variableMap[nextToken];
		consume(tokens, nextToken);
	}
	else if (nextToken == "(")
	{
		consume(tokens, nextToken);
		value = parse_MathExp(tokens);
		if (peek(tokens) == ")") consume(tokens, ")");
		else throw std::runtime_error("Expected: )\n");
	}
	else
	{
		throw std::runtime_error("expected int, variable or ( )");
	}
	return value;
}

bool Interpreter::isInteger(std::string& token)
{
	int x = 0;
	if (token[0] == '-')
	{
		x++;
	}
	for (size_t i = x; i < token.size(); i++)
	{
		if (!isdigit(token[i])) return false;
	}
	return true;
}

bool Interpreter::isVariable(std::string& token)
{
	if (!isalpha(token[0])) return false;
	for (size_t i = 1; i < token.size(); i++)
	{
		if (!isalpha(token[i]) && !isdigit(token[i])) return false;
	}
	return true;
}


