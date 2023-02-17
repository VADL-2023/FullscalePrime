#ifndef STATE_FULL_LIFT_H
#define STATE_FULL_LIFT_H

#include <iostream>
#include <map>
#include "State.h"

class Root;
class State_Full_Lift : public State {
	public:
		State_Full_Lift();
		State_Full_Lift(StateName name, std::map<EventName, StateName> &state_transitions, Root* root);
		EventName execute() override;
		EventName unitExecute() override;
};
#endif