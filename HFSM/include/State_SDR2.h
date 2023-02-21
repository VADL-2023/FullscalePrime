#ifndef STATE_SDR2_H
#define STATE_SDR2_H

#include <iostream>
#include <map>
#include "State.h"

class Root;
class State_SDR2 : public State {
	public:
		State_SDR2();
		State_SDR2(StateName name, std::map<EventName, StateName> &state_transitions, Root* root);
		EventName execute() override;
		EventName unitExecute() override;
};
#endif