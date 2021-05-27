#ifndef _SERVER_GRESKA_
#define _SERVER_GRESKA_
#include<iostream>
#include<exception>
class E_Server : public std::exception{
public:
	E_Server() = default;
	friend std::ostream& operator<< (std::ostream & it, const E_Server&) {
		it << "Unable to connect to server!! \n\n";

		return it;
	}
};
#endif // !_SERVER_GRESKA_
