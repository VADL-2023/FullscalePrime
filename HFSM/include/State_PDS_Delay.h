#ifndef STATE_PDS_DELAY_H
#define STATE_PDS_DELAY_H

#include <iostream>
#include <map>
#include "State.h"

class Root;
class State_PDS_Delay : public State {
	public:
		State_PDS_Delay();
		State_PDS_Delay(StateName name, std::map<EventName, StateName> &state_transitions, Root* root);
		EventName execute() override;
		EventName unitExecute() override;
};
#endif