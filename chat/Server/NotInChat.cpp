 #include "NotInChat.h"

NotInChat::NotInChat()
{

}


State* NotInChat::nextState() {
	return this;
}


void NotInChat::messageCome(std::string& message) {

	std::cout << "NotInChat " << message << std::endl;

}