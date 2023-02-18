#include "State_RAFCO_Mission.h"
#include "Root.h"

State_RAFCO_Mission::State_RAFCO_Mission(): State() {

}

State_RAFCO_Mission::State_RAFCO_Mission(StateName name, std::map<EventName, StateName> &state_transitions, Root* root) : State(name, state_transitions, root) {

}

EventName State_RAFCO_Mission::execute() {
	std::cout << "In State_RAFCO_Mission and will return end state." << std::endl;
	return TERMINATE;
}

EventName State_RAFCO_Mission::unitExecute() {
	return this->execute();
}