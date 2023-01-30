#ifndef STATE_INITIAL_H
#define STATE_INITIAL_H

#include <iostream>
#include <map>
#include "State.h"
#include "pigpio.h"

class Root;
class State_Initial : public State {
	public:
		State_Initial();
		State_Initial(StateName name, std::map<EventName, StateName> &state_transitions, Root* root);
		EventName execute() override;
		EventName unitExecute() override;
};
#endif