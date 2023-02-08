#ifndef STATE_STEPPER1_H
#define STATE_STEPPER1_H

#include <iostream>
#include <map>
#include "State.h"

class Root;
class State_Stepper1 : public State {
	public:
		State_Stepper1();
		State_Stepper1(StateName name,std::map<EventName,StateName> &state_transitions,Root* root);
		EventName execute() override;
		EventName unitExecute() override;
};
#endif