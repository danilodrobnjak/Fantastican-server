#include "Client.h"
#include<vector>
#define SIZE 4

int main() {
	/*

	std::vector<std::thread> niz = {};
	Client* klijenti = new Client[SIZE];
	
	try{

		for (int i = 0;i < SIZE;i++) {
			niz.push_back(std::thread(&Client::run, klijenti[i]));
		}


		for (int i = 0; i < SIZE; i++)
		{
			niz[i].join();
		}

	}
	catch (E_ADDR_INFO& g) { std::cout << g << std::endl; }
	catch (E_Server& g) { std::cout << g << std::endl; }
	catch (E_Socket& g) { std::cout << g << std::endl; }
	catch (E_WSAStartup& g) { std::cout << g << std::endl; }
	

	*/
	try {

		Client klijent;
		std::thread klijent1(&Client::run, klijent);
		klijent1.join();
		
	}
	catch (E_ADDR_INFO& g) { std::cout << g << std::endl; }
	catch (E_Server& g) { std::cout << g << std::endl; }
	catch (E_Socket& g) { std::cout << g << std::endl; }
	catch (E_WSAStartup& g) { std::cout << g << std::endl; }

	return 0;
}