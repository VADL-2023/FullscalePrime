#include "State_Lift_Motor.h"
#include "Root.h"

State_Lift_Motor::State_Lift_Motor(): State() {

}

State_Lift_Motor::State_Lift_Motor(StateName name, std::map<EventName, StateName> &state_transitions, Root* root) : State(name, state_transitions, root) {

}

EventName State_Lift_Motor::execute() {
	std::cout << "In State_Lift_Motor and will return end state." << std::endl;
	return TERMINATE;
}

EventName State_Lift_Motor::unitExecute() {
	std::cout << "Starting Lift Unit Test" << std::endl;
	double start_time = this->root_->getCurrentTime();
	double current_time = start_time;
	gpioWrite(this->root_->rcb_lift_standby_,1);
	std::cout << "Spin one way" << std::endl;
	while(current_time - start_time < this->root_->lift_time_threshold_) {
		gpioWrite(this->root_->lift_p_,1);
		gpioWrite(this->root_->lift_n_,0);
		gpioPWM(this->root_->lift_enable_,this->root_->pwm_motor_max_);
		current_time = this->root_->getCurrentTime();
	}
	gpioWrite(this->root_->rcb_lift_standby_,0);
	usleep(2000000);
	start_time = this->root_->getCurrentTime();
	current_time = start_time;
	gpioWrite(this->root_->rcb_lift_standby_,1);
	std::cout << "Spin other way" << std::endl;
	while(current_time - start_time < this->root_->lift_time_threshold_) {
		gpioWrite(this->root_->lift_p_,0);
		gpioWrite(this->root_->lift_n_,1);
		gpioPWM(this->root_->lift_enable_,this->root_->pwm_motor_max_);
		current_time = this->root_->getCurrentTime();
	}
	gpioWrite(this->root_->rcb_lift_standby_,0);
	gpioWrite(this->root_->lift_n_,0);
	gpioPWM(this->root_->lift_enable_,0);
	std::cout << "Ending Lift Unit Test" << std::endl;
	return BASIC_ROTATE;
}