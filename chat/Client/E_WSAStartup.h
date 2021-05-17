#ifndef _GRESKA_WSA_
#define _GRESKA_WSA_
#include<exception>
#include<iostream>
class E_WSAStartup :public std::exception{


public:
	  E_WSAStartup() = default;
	  friend std::ostream& operator<< (std::ostream& it, const E_WSAStartup&) {
		  it << "WSA startup faild! \n\n";

		  return it;
	  }
};
#endif
