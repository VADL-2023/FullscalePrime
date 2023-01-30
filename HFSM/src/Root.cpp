#include "Root.h"
#include "State.h"

Root::Root():start_time_(0),is_unit_fsm_(false) {

}

Root::Root(bool is_unit_fsm):start_time_(0),is_unit_fsm_(is_unit_fsm) {
}

void Root::addState(State* the_state) {
    this->states_.insert(std::pair<StateName,State*>(the_state->getName(),the_state));
}

double Root::getCurrentTime() {
    return double(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
}