#include "State_RCB_Motor.h"
#include "Root.h"

State_RCB_Motor::State_RCB_Motor(): State() {

}

State_RCB_Motor::State_RCB_Motor(StateName name, std::map<EventName, StateName> &state_transitions, Root* root) : State(name, state_transitions, root) {

}

EventName State_RCB_Motor::execute() {
	std::cout << "In State_RCB_Motor and will return end state." << std::endl;
	return TERMINATE;
}

EventName State_RCB_Motor::unitExecute() {
	std::cout << "Starting RCB Unit Test" << std::endl;
	double start_time = this->root_->getCurrentTime();
	double current_time = start_time;
	gpioWrite(this->root_->rcb_standby_,1);
	std::cout << "Spin one way" << std::endl;
	while(current_time - start_time < this->root_->rcb_time_threshold_) {
		gpioWrite(this->root_->rcb_p_,1);
		gpioWrite(this->root_->rcb_n_,0);
		gpioPWM(this->root_->rcb_enable_,this->root_->pwm_motor_max_);
		current_time = this->root_->getCurrentTime();
	}
	gpioWrite(this->root_->rcb_standby_,0);
	usleep(2000000);
	start_time = this->root_->getCurrentTime();
	current_time = start_time;
	gpioWrite(this->root_->rcb_standby_,1);
	std::cout << "Spin other way" << std::endl;
	while(current_time - start_time < this->root_->rcb_time_threshold_) {
		gpioWrite(this->root_->rcb_p_,0);
		gpioWrite(this->root_->rcb_n_,1);
		gpioPWM(this->root_->rcb_enable_,this->root_->pwm_motor_max_);
		current_time = this->root_->getCurrentTime();
	}
	gpioWrite(this->root_->rcb_standby_,0);
	gpioWrite(this->root_->rcb_n_,0);
	gpioPWM(this->root_->rcb_enable_,0);
	std::cout << "Ending RCB Unit Test" << std::endl;
	return BASIC_ROTATE;
}