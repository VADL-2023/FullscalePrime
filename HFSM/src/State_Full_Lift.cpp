#include "State_Full_Lift.h"
#include "Root.h"

State_Full_Lift::State_Full_Lift(): State() {

}

State_Full_Lift::State_Full_Lift(StateName name, std::map<EventName, StateName> &state_transitions, Root* root) : State(name, state_transitions, root) {

}

EventName State_Full_Lift::execute() {
	this->root_->m_log_.write("In State full lift");
	this->root_->m_log_.write("Initiate lift servo unlock");
	gpioServo(this->root_->lift_servo_,this->root_->lift_unlock_);
	gpioSleep(0,2,0);
	this->root_->m_log_.write("Initiate lift servo lock");
	bool is_done = false;
	bool is_time_up = false;
	double start_time = this->root_->getCurrentTime();
	double current_time = start_time;
	this->root_->m_log_.write("Starting winch");
	gpioWrite(this->root_->rcb_lift_standby_,1);
	while(!is_done) {
		std::cout << "Base Limit switch: " << gpioRead(this->root_->lift_base_limit_switch_) << std::endl;
		std::cout << "Final Limit switch: " << gpioRead(this->root_->lift_final_limit_switch_) << std::endl;
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
		this->root_->m_log_.write("Time Ran Out for Lift");
		return LIFT_FAILURE;
	} else {
		this->root_->m_log_.write("Lift success");
		return LIFT_SUCCESS; 
	}
	
}

EventName State_Full_Lift::unitExecute() {
	return this->execute();
}