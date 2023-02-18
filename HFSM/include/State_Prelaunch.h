#ifndef STATE_PRELAUNCH_H
#define STATE_PRELAUNCH_H

#include <iostream>
#include <map>
#include "State.h"

class Root;
class State_Prelaunch : public State {
	public:
		State_Prelaunch();
		State_Prelaunch(StateName name, std::map<EventName, StateName> &state_transitions, Root* root);
		EventName execute() override;
		EventName unitExecute() override;
};
#endif