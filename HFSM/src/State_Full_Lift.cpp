#include "State_Full_Lift.h"
#include "Root.h"

State_Full_Lift::State_Full_Lift(): State() {

}

State_Full_Lift::State_Full_Lift(StateName name, std::map<EventName, StateName> &state_transitions, Root* root) : State(name, state_transitions, root) {

}

EventName State_Full_Lift::execute() {
	std::cout << "In State_Full_Lift and will return end state." << std::endl;
	float pulse_width = this->root_->angleToPulseWidth(this->root_->servo_pulse_max_,this->root_->servo_pulse_min_,this->root_->servo_deg_range_,this->root_->unlock_lift_angle_);
	gpioServo(this->root_->lift_servo_,pulse_width);
	gpioSleep(0,2,0);
	bool is_done = false;
	bool is_time_up = false;
	double start_time = this->root_->getCurrentTime();
	double current_time = start_time;
	std::cout << "Starting winch" << std::endl;
	gpioWrite(this->root_->rcb_lift_standby_,1);
	while(!is_done) {
		is_done = !gpioRead(this->root_->lift_final_limit_switch_);
		current_time = this->root_->getCurrentTime();
		if(current_time - start_time > this->root_->lift_time_threshold_) {
			is_time_up = true;
			is_done = true;
		}
		gpioWrite(this->root_->lift_p_,1);
		gpioWrite(this->root_->lift_n_,0);
		gpioPWM(this->root_->lift_enable_,this->root_->pwm_motor_max_);
		
	}
	gpioWrite(this->root_->rcb_lift_standby_,0);
	gpioWrite(this->root_->lift_p_,0);
	gpioPWM(this->root_->lift_enable_,0);
	if(is_time_up) {
		std::cout << "Time ran out" << std::endl;
		return LIFT_FAILURE;
	} else {
		std::cout << "Winch success" << std::endl;
		return LIFT_SUCCESS; 
	}
	
}

EventName State_Full_Lift::unitExecute() {
	return this->execute();
}