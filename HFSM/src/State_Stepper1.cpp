#include "State_Stepper1.h"
#include "Root.h"

State_Stepper1::State_Stepper1(): State() {

}

State_Stepper1::State_Stepper1(StateName &name,std::map<EventName,StateName> &state_transitions,Root* root): State(name,state_transitions,root) {

}


EventName State_Stepper1::execute() {
	std::cout << "I terminate" << std::endl;
    return TERMINATE;
}

EventName State_Stepper1::unitExecute() {
    double start_time = this->root_->getCurrentTime();
	while(this->root_->getCurrentTime() - start_time < this->root_->stepper_threshold_) {
        gpioWrite(this->root_->stepper_1_standby_pin_,1);
        std::cout << "Moving one way" << std::endl;
        //std::cout << "Standby: " << gpioRead(standby_pin) << std::endl;
        this->root_->stepper_1_.step(this->root_->num_steps_);
        std::cout << "Pausing" << std::endl;
        gpioWrite(this->root_->stepper_1_standby_pin_,0);
        usleep(2000000);
        gpioWrite(this->root_->stepper_1_standby_pin_,1);
        std::cout << "Moving other way" << std::endl;
        this->root_->stepper_1_.step(-this->root_->num_steps_);
        std::cout << "Pausing" << std::endl;
        gpioWrite(this->root_->stepper_1_standby_pin_,0);
        usleep(2000000);
    }
	return BASIC_SWIVEL;
}