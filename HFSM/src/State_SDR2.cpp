#include "State_SDR2.h"
#include "Root.h"

State_SDR2::State_SDR2(): State() {

}

State_SDR2::State_SDR2(StateName name, std::map<EventName, StateName> &state_transitions, Root* root) : State(name, state_transitions, root) {

}

EventName State_SDR2::execute() {
	std::cout << "In State_SDR2 and will return end state." << std::endl;
	return TERMINATE;
}

EventName State_SDR2::unitExecute() {
	std::cout << "Starting SDR" << std::endl;
	this->root_->radio2_.startSDR();
	std::cout << "Waiting for packets" << std::endl;
	AX25Packet p1;
	while (p1.msg.find("shutdown") == std::string::npos && p1.msg.find("ERROR") == std::string::npos) {
        p1 = this->root_->radio2_.getPacket();
        std::cout << p1.msg << std::endl;
    }
	std::cout << "Halting SDR" << std::endl;
	sleep(5);
	this->root_->radio2_.stopSDR();
	return RECEIVED_PACKETS;
}