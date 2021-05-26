#ifndef _GRESKA_ADDR_INFO_
#define _GRESKA_ADDR_INFO_
#include<exception>
#include<iostream>

class E_ADDR_INFO : public std::exception {
public:
	E_ADDR_INFO() = default;
	friend std::ostream& operator<< (std::ostream & it, const E_ADDR_INFO&) {
		it << "Getaddrinfo failed! \n\n";

		return it;
	}
};
#endif // !_GRESKA_ADDR_INFO_
