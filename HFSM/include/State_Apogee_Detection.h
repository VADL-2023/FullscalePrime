#ifndef STATE_APOGEE_DETECTION_H
#define STATE_APOGEE_DETECTION_H

#include <iostream>
#include <map>
#include "State.h"

class Root;
class State_Apogee_Detection : public State {
	public:
		State_Apogee_Detection();
		State_Apogee_Detection(StateName name, std::map<EventName, StateName> &state_transitions, Root* root);
		EventName execute() override;
		EventName unitExecute() override;
};
#endif