#ifndef STATE_FULL_RCB_H
#define STATE_FULL_RCB_H

#include <iostream>
#include <map>
#include "State.h"

class Root;
class State_Full_RCB : public State {
	public:
		State_Full_RCB();
		State_Full_RCB(StateName name, std::map<EventName, StateName> &state_transitions, Root* root);
		EventName execute() override;
		EventName unitExecute() override;
};
#endif