#ifndef STATE_FINAL_H
#define STATE_FINAL_H

#include <iostream>
#include <map>
#include "State.h"

class Root;
class State_Final : public State {
	public:
		State_Final();
		State_Final(StateName name, std::map<EventName, StateName> &state_transitions, Root* root);
		EventName execute() override;
};
#endif