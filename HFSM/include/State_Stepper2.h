#ifndef STATE_STEPPER2_H
#define STATE_STEPPER2_H

#include <iostream>
#include <map>
#include "State.h"

class Root;
class State_Stepper2 : public State {
	public:
		State_Stepper2();
		State_Stepper2(StateName name, std::map<EventName, StateName> &state_transitions, Root* root);
		EventName execute() override;
		EventName unitExecute() override;
};
#endif