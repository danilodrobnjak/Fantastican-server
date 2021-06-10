#ifndef _PARSER_H_
#define _PARSER_H_
#define MAX_PORT 65535
#include<queue>
#include<string>
#include <iostream>
#include<regex>
#include "E_Parser.h"

enum Spasoje
{
	PORT,HOST
};

class Parser {

	std::queue<std::string> m_smartQueue;
	std::string m_defPort;
	std::string m_defHost;



	void resolveProblemQueue();


public:
	Parser(int argc, char** argv);
	bool isInputValid();
	
};

#endif // !_PARSER_H_
