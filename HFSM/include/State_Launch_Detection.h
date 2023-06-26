#ifndef STATE_LAUNCH_DETECTION_H
#define STATE_LAUNCH_DETECTION_H

#include <iostream>
#include <map>
#include "State.h"

class Root;

/**
 * @file State_Launch_Detection.h
 * @class State_Launch_Detection
 * @brief State responsible for detecting launch with IMU
 * 
 */
class State_Launch_Detection : public State {
	public:

		/**
		 * @brief Construct a new State_Launch_Detection object
		 * 
		 */
		State_Launch_Detection();

		/**
		 * @brief Construct a new State_Launch_Detection object
		 * 
		 * @param name String representing state name
		 * @param state_transitions Map of transitions to determine what the next state will be
		 * @param root Root variable used for state machine
		 */
		State_Launch_Detection(StateName name, std::map<EventName, StateName> &state_transitions, Root* root);
		
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