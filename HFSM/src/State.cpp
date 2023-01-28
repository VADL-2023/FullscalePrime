#include "State.h"
#include "Root.h"

State::State() {

}

State::State(StateName name, std::map<EventName,StateName> &state_transitions, Root* root) : name_(name), state_transitions_(state_transitions), root_(root) {

}

StateName State::getName() {
    return this->name_;
}

StateName State::getNextState(EventName curr_event) {
    return this->state_transitions_[curr_event];
}

EventName State::execute() {
    return TERMINATE;
}
