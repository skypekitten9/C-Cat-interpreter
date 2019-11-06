#include <iostream>
#include <fstream>
#include "Interpreter.h"
int main()
{
	Interpreter i = Interpreter(std::cout);
	std::string line = "";
	std::ifstream myfile("cat.txt");
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			while (line.length() == 0 && getline(myfile, line));
			i.evaluate(i.tokenize(line));
		}
		myfile.close();
	}
	else
	{
		std::cout << "Unable to open file, read unsuccessful.\n";
		return 1;
	}
}