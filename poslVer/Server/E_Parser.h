#ifndef _E_PARSER_H_
#define _E_PARSER_H_
#include <exception>
#include <iostream>

class E_Parser : public std::exception {


public: 
	E_Parser(std::string i) {
		std::cout <<i<<std::endl;
	}

	friend std::ostream& operator<< (std::ostream & it, const E_Parser&) {
		it << "Greska pri unosu komande u parser \n\n";

		return it;
	}
};
#endif // !_E_PARSER_H_
