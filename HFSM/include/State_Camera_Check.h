#ifndef STATE_CAMERA_CHECK_H
#define STATE_CAMERA_CHECK_H

#include <iostream>
#include <map>
#include "State.h"

class Root;

/**
 * @file State_Camera_Check.h
 * @class State_Camera_Check
 * @brief State responsible for determining primary camera for RCB
 * 
 */
class State_Camera_Check : public State {
	public:

		/**
		 * @brief Construct a new State_Camera_Check object
		 * 
		 */
		State_Camera_Check();

		/**
		 * @brief Construct a new State_Camera_Check object
		 * 
		 * @param name String representing state name
		 * @param state_transitions Map of transitions to determine what the next state will be
		 * @param root Root variable used for state machine
		 */
		State_Camera_Check(StateName name, std::map<EventName, StateName> &state_transitions, Root* root);
		
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