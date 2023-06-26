#include "State_Lift_Servo.h"
#include "Root.h"

State_Lift_Servo::State_Lift_Servo(): State() {

}

State_Lift_Servo::State_Lift_Servo(StateName name, std::map<EventName, StateName> &state_transitions, Root* root) : State(name, state_transitions, root) {

}

EventName State_Lift_Servo::execute() {
	std::cout << "In State_Lift_Servo and will return end state." << std::endl;
	return TERMINATE;
}

EventName State_Lift_Servo::unitExecute() {
	// Move lift servo one way and then the other way
	std::cout << "Started Lift Servo Test" << std::endl;
	double start_time = this->root_->getCurrentTime();
	double current_time = start_time;
	double angle = 0;
	float pulse_width = this->root_->angleToPulseWidth(this->root_->servo_pulse_max_,this->root_->servo_pulse_min_,this->root_->servo_deg_range_,angle);
	gpioServo(this->root_->lift_servo_,pulse_width);
	gpioSleep(0,2,0);
	angle = 45;
	pulse_width = this->root_->angleToPulseWidth(this->root_->servo_pulse_max_,this->root_->servo_pulse_min_,this->root_->servo_deg_range_,angle);
	gpioServo(this->root_->lift_servo_,pulse_width);
	gpioSleep(0,2,0);
	std::cout << "Ended Lift Servo Test" << std::endl;
	return BASIC_SERVO;
}