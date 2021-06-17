#include "Parser.h" 


Parser::Parser(int argc, char** argv) {

	m_inputValid = true;
	for (int i = 1;i < argc;i++) {

		m_smartQueue.push(argv[i]);

	}
	resolveProblemQueue();
	std::cout << this->m_defPort << std::endl;
	std::cout << this->m_defHost << std::endl;
}

bool Parser::isInputValid() {

	return m_inputValid;

}

Spasoje strToEnum(std::string &input) {


	if (!input.compare("-p") || !input.compare("--port")) return PORT;
	if (!input.compare("-h") || !input.compare("--host")) return HOST;


}
bool Validate_It(std::string& IP)
{
	// IPv4
	std::regex ipv4("(([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])\\.){3}([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])");
	// IPv6
	std::regex ipv6("((([0-9a-fA-F]){1,4})\\:){7}([0-9a-fA-F]){1,4}");
	// valid IPv4 addresses
	if (regex_match(IP, ipv4))
		return true;
	// valid IPv6 addresses
	else if (regex_match(IP, ipv6))
		return true;
	// Return Invalid
	return false;
}

void Parser::resolveProblemQueue() {
	//-p -> port
	//-h ->host
	//--port 
	//--host
	while (!this->m_smartQueue.empty()) {

		std::string temp = m_smartQueue.front();
		m_smartQueue.pop();
		

		Spasoje realTemp = strToEnum(temp);
		temp = m_smartQueue.front();
		m_smartQueue.pop();

		
		switch (realTemp)
		{
		case PORT: {
			if (atoi(temp.c_str()) < 0 && atoi(temp.c_str()) >= MAX_PORT)
				throw new E_Parser("Broj porta koji ste uneli je van dozvoljenog opsega");

			this->m_defPort = temp;
			
			break;
		}
		case HOST: 
		{
			//getHostByName()
			//napraviti fju validacije za host
			if (!Validate_It(temp)) {
				//std::cout << "Ne valja ip";
				throw new E_Parser("Zadata ip adresa nije u dobrom formatu!");
			}
			this->m_defHost = temp;               
			break;
		}
		default:
		{
			m_inputValid = false;
		}
			

		}

		



	}

}