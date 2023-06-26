#ifndef STATE_LANDING_DETECTION_H
#define STATE_LANDING_DETECTION_H

#include <iostream>
#include <map>
#include "State.h"

class Root;

/**
 * @file State_Landing_Detection.h
 * @class State_Landing_Detection
 * @brief State responsible for detecting landing with the IMU
 * 
 */
class State_Landing_Detection : public State {
	public:

		/**
		 * @brief Construct a new State_Landing_Detection object
		 * 
		 */
		State_Landing_Detection();

		/**
		 * @brief Construct a new State_Landing_Detection object
		 * 
		 * @param name String representing state name
		 * @param state_transitions Map of transitions to determine what the next state will be
		 * @param root Root variable used for state machine
		 */
		State_Landing_Detection(StateName name, std::map<EventName, StateName> &state_transitions, Root* root);
		
		/**
		 * @brief Main execution method for state
		 * 
		 * @return Result of state execution 
		 */
		EventName execute() override;

		/**
		 * @brief Execution method for state in display mode (DOESN'T WORK)
		 * 
		 * @return Result of state execution 
		 */
		EventName unitExecute() override;
};
#endif