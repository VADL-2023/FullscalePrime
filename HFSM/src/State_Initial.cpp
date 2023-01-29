#include "State_Initial.h"
#include "Root.h"

State_Initial::State_Initial(): State() {

}

State_Initial::State_Initial(StateName name, std::map<EventName, StateName> &state_transitions, Root* root) : State(name, state_transitions, root) {

}

EventName State_Initial::execute() {
	// Temporary initialization thing
    while(gpioInitialise() <= 0) {
        std::cout << "Not initialized" << std::endl;
    }
    std::cout << "Initialized" << std::endl;
	// Stepper 1 Initialization
    this->root_->stepper_1_.initialize(this->root_->steps_per_revolution_,this->root_->stepper_1_pin_1_,this->root_->stepper_1_pin_2_,this->root_->stepper_1_pin_3_,this->root_->stepper_1_pin_4_,true);
    gpioSetMode(this->root_->stepper_1_standby_pin_,PI_OUTPUT);
    gpioWrite(this->root_->stepper_1_standby_pin_,0);
    this->root_->stepper_1_.setSpeed(this->root_->stepper_speed_);
	return INITIALIZE;
}