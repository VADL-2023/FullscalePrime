#include "State_Level_Servo.h"
#include "Root.h"

State_Level_Servo::State_Level_Servo(): State() {

}

State_Level_Servo::State_Level_Servo(StateName name, std::map<EventName, StateName> &state_transitions, Root* root) : State(name, state_transitions, root) {

}

EventName State_Level_Servo::execute() {
	std::cout << "In State_Level_Servo and will return end state." << std::endl;
	return TERMINATE;
}

EventName State_Level_Servo::unitExecute() {
	// Move servo one way and then the other way
	std::cout << "Started Level Servo Test" << std::endl;
	double start_time = this->root_->getCurrentTime();
	double current_time = start_time;
	double angle = 0;
	float pulse_width = this->root_->angleToPulseWidth(this->root_->servo_pulse_max_,this->root_->servo_pulse_min_,this->root_->servo_deg_range_,angle);
	gpioServo(this->root_->level_servo_,pulse_width);
	gpioSleep(0,2,0);
	angle = 20;
	pulse_width = this->root_->angleToPulseWidth(this->root_->servo_pulse_max_,this->root_->servo_pulse_min_,this->root_->servo_deg_range_,angle);
	gpioServo(this->root_->level_servo_,pulse_width);
	gpioSleep(0,2,0);
	std::cout << "Ended Level Servo Test" << std::endl;
	return BASIC_SERVO;
}