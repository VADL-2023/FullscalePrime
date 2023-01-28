#include "Root.h"
#include "State.h"

Root::Root():start_time_(0) {
    // Stepper 1 Initialization
    this->stepper_1_.initialize(this->steps_per_revolution_,this->stepper_1_pin_1_,stepper_1_pin_2_,stepper_1_pin_3_,stepper_1_pin_4_,true);
    gpioSetMode(this->stepper_1_standby_pin_,PI_OUTPUT);
    gpioWrite(this->stepper_1_standby_pin_,0);
    this->stepper_1_.setSpeed(this->stepper_speed_);
}

void Root::addStates(State* the_state) {
    this->states_.insert(std::pair<std::string,State*>(the_state->getName(),the_state));
}

double Root::getCurrentTime() {
    return double(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
}