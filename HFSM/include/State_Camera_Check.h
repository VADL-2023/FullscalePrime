#ifndef STATE_CAMERA_CHECK_H
#define STATE_CAMERA_CHECK_H

#include <iostream>
#include <map>
#include "State.h"

class Root;
class State_Camera_Check : public State {
	public:
		State_Camera_Check();
		State_Camera_Check(StateName name, std::map<EventName, StateName> &state_transitions, Root* root);
		EventName execute() override;
		EventName unitExecute() override;
		int cameraCheck(std::string camera_check);
};
#endif