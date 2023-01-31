#ifndef STATE_NACELLE_SERVO_H
#define STATE_NACELLE_SERVO_H

#include <iostream>
#include <map>
#include "State.h"
#include "pigpio.h"

class Root;
class State_Nacelle_Servo : public State {
	public:
		State_Nacelle_Servo();
		State_Nacelle_Servo(StateName name, std::map<EventName, StateName> &state_transitions, Root* root);
		EventName execute() override;
		EventName unitExecute() override;
};
#endif