#ifndef STATE_APOGEE_DETECTION_H
#define STATE_APOGEE_DETECTION_H

#include <iostream>
#include <map>
#include "State.h"

class Root;

/**
 * @file State_Apogee_Detection.h
 * @class State_Apogee_Detection
 * @brief State responsible for detecting apogee with IMU
 * 
 */
class State_Apogee_Detection : public State {
	public:

		/**
		 * @brief Construct a new State_Apogee_Detection object
		 * 
		 */
		State_Apogee_Detection();

		/**
		 * @brief Construct a new State_Apogee_Detection object
		 * 
		 * @param name String representing state name
		 * @param state_transitions Map of transitions to determine what the next state will be
		 * @param root Root variable used for state machine
		 */
		State_Apogee_Detection(StateName name, std::map<EventName, StateName> &state_transitions, Root* root);
		
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