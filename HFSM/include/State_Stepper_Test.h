#ifndef STATE_STEPPER_TEST_H
#define STATE_STEPPER_TEST_H

#include <iostream>
#include <map>
#include "State.h"

class Root;
class State_Stepper_Test : public State {
	public:
		State_Stepper_Test();
		State_Stepper_Test(StateName &name,std::map<EventName,StateName> &state_transitions,Root* root);
		EventName execute() override;
};
#endif