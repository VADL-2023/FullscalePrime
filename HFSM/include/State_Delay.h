#ifndef STATE_DELAY_H
#define STATE_DELAY_H

#include <iostream>
#include <map>
#include "State.h"

class Root;
class State_Delay : public State {
	public:
		State_Delay();
		State_Delay(StateName name, std::map<EventName, StateName> &state_transitions, Root* root,double delay_in_ms=1000);
		EventName execute() override;
		EventName unitExecute() override;
	private:
		double delay_in_ms_;
};
#endif