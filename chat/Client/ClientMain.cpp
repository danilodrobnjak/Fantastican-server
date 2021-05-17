#include <iostream>
#include "Client.h"

int main() {

	std::string errMessage;
	bool error = false;

	Client klijent(error, errMessage);
	
	if (error) {
		std::cout << errMessage << std::endl;
	}
	else {
	
		std::thread klijent1(&Client::run,klijent);
		
		klijent1.join();
		
	
	}


	return 0;
}