#ifndef _GRESKA_WSA_
#define _GRESKA_WSA_
#include<exception>
#include<iostream>
class E_WsaFaildServer :public std::exception{


public:
	E_WsaFaildServer() {

		std::cout << "WSA nije uspeo da se inicijalizuje!";
	}


};
#endif // !_GRESKA_WSA_
