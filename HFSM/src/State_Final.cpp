#include "State_Final.h"
#include "Root.h"
#include "pigpio.h"

State_Final::State_Final(): State() {

}

State_Final::State_Final(StateName name, std::map<EventName, StateName> &state_transitions, Root* root) : State(name, state_transitions, root) {

}

EventName State_Final::execute() {
	gpioTerminate();
	std::cout << "Terminated" << std::endl;
	return TERMINATE;
}