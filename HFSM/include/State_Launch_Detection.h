#ifndef STATE_LAUNCH_DETECTION_H
#define STATE_LAUNCH_DETECTION_H

#include <iostream>
#include <map>
#include "State.h"

class Root;
class State_Launch_Detection : public State {
	public:
		State_Launch_Detection();
		State_Launch_Detection(StateName name, std::map<EventName, StateName> &state_transitions, Root* root);
		EventName execute() override;
		EventName unitExecute() override;
};
#endif