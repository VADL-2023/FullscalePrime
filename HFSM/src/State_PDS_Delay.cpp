#include "State_PDS_Delay.h"
#include "Root.h"

State_PDS_Delay::State_PDS_Delay(): State() {

}

State_PDS_Delay::State_PDS_Delay(StateName name, std::map<EventName, StateName> &state_transitions, Root* root) : State(name, state_transitions, root) {

}

EventName State_PDS_Delay::execute() {
	this->root_->m_log_.write("In State PDS Delay and starting " + std::to_string(this->root_->pds_delay_) + " ms delay.");
	auto start_time = this->root_->getCurrentTime();
	auto current_time = start_time;
	while(current_time - start_time < this->root_->pds_delay_) {
		current_time = this->root_->getCurrentTime();
	}
	this->root_->m_log_.write("Exiting PDS delay");
	this->root_->m_log_.tempSaveProgLog();
	return DELAY;
}

EventName State_PDS_Delay::unitExecute() {
	return this->execute();
}