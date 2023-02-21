#ifndef STATE_LIFT_SERVO_H
#define STATE_LIFT_SERVO_H

#include <iostream>
#include <map>
#include "State.h"
#include "pigpio.h"

class Root;
class State_Lift_Servo : public State {
	public:
		State_Lift_Servo();
		State_Lift_Servo(StateName name, std::map<EventName, StateName> &state_transitions, Root* root);
		EventName execute() override;
		EventName unitExecute() override;
};
#endif