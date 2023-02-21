#ifndef STATE_STEPPER3_H
#define STATE_STEPPER3_H

#include <iostream>
#include <map>
#include "State.h"

class Root;
class State_Stepper3 : public State {
	public:
		State_Stepper3();
		State_Stepper3(StateName name, std::map<EventName, StateName> &state_transitions, Root* root);
		EventName execute() override;
		EventName unitExecute() override;
};
#endif