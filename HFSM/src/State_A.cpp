#include "State_A.h"
#include "Root.h"

State_A::State_A(): State() {

}

State_A::State_A(StateName name, std::map<EventName,StateName> &state_transitions, Root* root) : State(name,state_transitions,root) {

}

EventName State_A::execute() {
    std::cout << "Started State A" << std::endl;
    while(true) {
        auto current_time = this->root_->getCurrentTime();
        if(current_time - this->root_->start_time_ > 20000) {
            std::cout << "About to End" << std::endl;
            return TIME_20;
        } else if(current_time - this->root_->start_time_ > 10000 && current_time - this->root_->start_time_ < 10010) {
            std::cout << "About to go to State B" << std::endl;
            return TIME_10;
        }
    }
}