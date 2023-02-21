#ifndef STATE_RAFCO_MISSION_H
#define STATE_RAFCO_MISSION_H

#include <iostream>
#include <map>
#include "State.h"

class Root;
class State_RAFCO_Mission : public State {
	public:
		State_RAFCO_Mission();
		State_RAFCO_Mission(StateName name, std::map<EventName, StateName> &state_transitions, Root* root);
		EventName execute() override;
		EventName unitExecute() override;
};
#endif