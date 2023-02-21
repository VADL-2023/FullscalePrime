#include "State_Stepper2.h"
#include "Root.h"

State_Stepper2::State_Stepper2(): State() {

}

State_Stepper2::State_Stepper2(StateName name, std::map<EventName, StateName> &state_transitions, Root* root) : State(name, state_transitions, root) {

}

EventName State_Stepper2::execute() {
	std::cout << "In State_Stepper2 and will return end state." << std::endl;
	return TERMINATE;
}

EventName State_Stepper2::unitExecute() {
	double start_time = this->root_->getCurrentTime();
	while(this->root_->getCurrentTime() - start_time < this->root_->stepper_threshold_) {
        gpioWrite(this->root_->stepper_2_standby_pin_,1);
        std::cout << "Moving one way" << std::endl;
        //std::cout << "Standby: " << gpioRead(standby_pin) << std::endl;
        this->root_->stepper_2_.step(this->root_->num_steps_);
        std::cout << "Pausing" << std::endl;
        gpioWrite(this->root_->stepper_2_standby_pin_,0);
        usleep(2000000);
        gpioWrite(this->root_->stepper_2_standby_pin_,1);
        std::cout << "Moving other way" << std::endl;
        this->root_->stepper_2_.step(-this->root_->num_steps_);
        std::cout << "Pausing" << std::endl;
        gpioWrite(this->root_->stepper_2_standby_pin_,0);
        usleep(2000000);
    }
	return BASIC_SWIVEL;
}