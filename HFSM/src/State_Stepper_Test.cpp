#include "State_Stepper_Test.h"
#include "Root.h"

State_Stepper_Test::State_Stepper_Test(): State() {

}

State_Stepper_Test::State_Stepper_Test(std::string &name,std::map<std::string,std::string> &state_transitions,Root* root): State(name,state_transitions,root) {

}


std::string State_Stepper_Test::execute() {
	double start_time = this->root_->getCurrentTime();
	while(this->root_->getCurrentTime() - start_time < 10000) {
        gpioWrite(this->root_->stepper_1_standby_pin_,1);
        std::cout << "Moving one way" << std::endl;
        //std::cout << "Standby: " << gpioRead(standby_pin) << std::endl;
        this->root_->stepper_1_.step(34);
        std::cout << "Pausing" << std::endl;
        gpioWrite(this->root_->stepper_1_standby_pin_,0);
        usleep(2000000);
        gpioWrite(this->root_->stepper_1_standby_pin_,1);
        std::cout << "Moving other way" << std::endl;
        this->root_->stepper_1_.step(-34);
        std::cout << "Pausing" << std::endl;
        gpioWrite(this->root_->stepper_1_standby_pin_,0);
        usleep(2000000);
    }
	return "end_state";
}