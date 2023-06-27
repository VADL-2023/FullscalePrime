#ifndef STATE_PRELAUNCH_H
#define STATE_PRELAUNCH_H

#include <iostream>
#include <map>
#include "State.h"

class Root;

/**
 * @file State_Prelaunch.h
 * @class State_Prelaunch
 * @brief State responsible for verifying parameters before launch detection
 * 
 */
class State_Prelaunch : public State {
	public:

		/**
		 * @brief Construct a new State_Prelaunch object
		 * 
		 */
		State_Prelaunch();

		/**
		 * @brief Construct a new State_Prelaunch object
		 * 
		 * @param name String representing state name
		 * @param state_transitions Map of transitions to determine what the next state will be
		 * @param root Root variable used for state machine
		 */
		State_Prelaunch(StateName name, std::map<EventName, StateName> &state_transitions, Root* root);
		
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