#ifndef _GRESKA_SOCKET_
#define _GRESKA_SOCKET_
#include<iostream>
#include<exception>


class E_Socket : public std::exception{

public:
	E_Socket() = default;
	friend std::ostream& operator<< (std::ostream & it, const E_Socket&) {
		it << "Creating socket failed! \n\n";

		return it;
	}
};


#endif