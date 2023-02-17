#ifndef STATE_SDR1_H
#define STATE_SDR1_H

#include <iostream>
#include <map>
#include "State.h"

class Root;
class State_SDR1 : public State {
	public:
		State_SDR1();
		State_SDR1(StateName name, std::map<EventName, StateName> &state_transitions, Root* root);
		EventName execute() override;
		EventName unitExecute() override;
};
#endif