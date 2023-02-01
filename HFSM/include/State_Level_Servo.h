#ifndef STATE_LEVEL_SERVO_H
#define STATE_LEVEL_SERVO_H

#include <iostream>
#include <map>
#include "State.h"
#include "pigpio.h"

class Root;
class State_Level_Servo : public State {
	public:
		State_Level_Servo();
		State_Level_Servo(StateName name, std::map<EventName, StateName> &state_transitions, Root* root);
		EventName execute() override;
		EventName unitExecute() override;
};
#endif