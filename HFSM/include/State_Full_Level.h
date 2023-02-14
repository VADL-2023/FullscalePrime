#ifndef STATE_FULL_LEVEL_H
#define STATE_FULL_LEVEL_H

#include <iostream>
#include <map>
#include "State.h"

class Root;
class State_Full_Level : public State {
	public:
		State_Full_Level();
		State_Full_Level(StateName name, std::map<EventName, StateName> &state_transitions, Root* root);
		EventName execute() override;
		EventName unitExecute() override;
};
#endif