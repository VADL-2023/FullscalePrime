#include "State_B.h"
#include "Root.h"

State_B::State_B(): State() {

}

State_B::State_B(StateName name,std::map<EventName, StateName> &state_transitions, Root* root) : State(name,state_transitions,root) {

}

EventName State_B::execute() {
    std::cout << "Started State B" << std::endl;
    while(true) {
        auto current_time = this->root_->getCurrentTime();
        if(current_time - this->root_->start_time_ > 15000) {
            std::cout << "About to go to State A" << std::endl;
            return TIME_15;
        }
    }
}