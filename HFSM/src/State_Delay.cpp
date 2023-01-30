#include "State_Delay.h"
#include "Root.h"

State_Delay::State_Delay(): State() {

}

State_Delay::State_Delay(StateName name, std::map<EventName, StateName> &state_transitions, Root* root,double delay_in_ms) : State(name, state_transitions, root),delay_in_ms_(delay_in_ms) {

}

EventName State_Delay::execute() {
	double start_time = this->root_->getCurrentTime();
	double current_time = start_time;
	std::cout << "Start delay" << std::endl;
	while(current_time - start_time < this->delay_in_ms_) {
		current_time = this->root_->getCurrentTime();
	}
	std::cout << "End delay" << std::endl;
	if(this -> delay_in_ms_ == 1000) {
		return DELAY_1_SECOND;
	} else {
		return DELAY;
	}
}

EventName State_Delay::unitExecute() {
	return this->execute();
}