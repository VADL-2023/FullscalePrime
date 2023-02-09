#ifndef STATE_LANDING_DETECTION_H
#define STATE_LANDING_DETECTION_H

#include <iostream>
#include <map>
#include "State.h"

class Root;
class State_Landing_Detection : public State {
	public:
		State_Landing_Detection();
		State_Landing_Detection(StateName name, std::map<EventName, StateName> &state_transitions, Root* root);
		EventName execute() override;
		EventName unitExecute() override;
};
#endif