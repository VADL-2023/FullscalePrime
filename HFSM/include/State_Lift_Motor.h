#ifndef STATE_LIFT_MOTOR_H
#define STATE_LIFT_MOTOR_H

#include <iostream>
#include <map>
#include "State.h"
#include "pigpio.h"

class Root;
class State_Lift_Motor : public State {
	public:
		State_Lift_Motor();
		State_Lift_Motor(StateName name, std::map<EventName, StateName> &state_transitions, Root* root);
		EventName execute() override;
		EventName unitExecute() override;
};
#endif