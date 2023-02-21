#ifndef STATE_RCB_MOTOR_H
#define STATE_RCB_MOTOR_H

#include <iostream>
#include <map>
#include "State.h"
#include "pigpio.h"

class Root;
class State_RCB_Motor : public State {
	public:
		State_RCB_Motor();
		State_RCB_Motor(StateName name, std::map<EventName, StateName> &state_transitions, Root* root);
		EventName execute() override;
		EventName unitExecute() override;
};
#endif