#include "State_PDS_Delay.h"
#include "Root.h"

State_PDS_Delay::State_PDS_Delay(): State() {

}

State_PDS_Delay::State_PDS_Delay(StateName name, std::map<EventName, StateName> &state_transitions, Root* root) : State(name, state_transitions, root) {

}

EventName State_PDS_Delay::execute() {
	std::cout << "In State_PDS_Delay and will return end state." << std::endl;
	auto start_time = this->root_->getCurrentTime();
	auto current_time = start_time;
	while(current_time - start_time < 15000) {
		current_time = this->root_->getCurrentTime();
	}
	std::cout << "Exiting delay" << std::endl;
	return DELAY;
}

EventName State_PDS_Delay::unitExecute() {
	return this->execute();
}