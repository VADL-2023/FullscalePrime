#include "State_Nacelle_Servo.h"
#include "Root.h"

State_Nacelle_Servo::State_Nacelle_Servo() : State()
{
}

State_Nacelle_Servo::State_Nacelle_Servo(StateName name, std::map<EventName, StateName> &state_transitions, Root *root) : State(name, state_transitions, root)
{
}

EventName State_Nacelle_Servo::execute()
{
	std::cout << "In State_Nacelle_Servo and will return end state." << std::endl;
	return TERMINATE;
}

EventName State_Nacelle_Servo::unitExecute()
{
	std::cout << "Started Nacelle Servo Test" << std::endl;
	double start_time = this->root_->getCurrentTime();
	double current_time = start_time;
	double angle = 60;
	float pulse_width = this->root_->angleToPulseWidth(this->root_->servo_pulse_max_,this->root_->servo_pulse_min_,this->root_->servo_deg_range_,angle);
	gpioServo(this->root_->nacelle_servo_,pulse_width);
	gpioSleep(0,2,0);
	angle = 30;
	pulse_width = this->root_->angleToPulseWidth(this->root_->servo_pulse_max_,this->root_->servo_pulse_min_,this->root_->servo_deg_range_,angle);
	gpioServo(this->root_->nacelle_servo_,pulse_width);
	gpioSleep(0,2,0);
	std::cout << "Ended Nacelle Servo Test" << std::endl;
	return BASIC_SERVO;
}