#ifndef STATE_STEPPER2_H
#define STATE_STEPPER2_H

#include <iostream>
#include <map>
#include "State.h"

class Root;

/**
 * @file State_Stepper2.h
 * @class State_Stepper2
 * @brief State responsible for testing movement for nacelle 2
 * 
 */
class State_Stepper2 : public State {
	public:

		/**
		 * @brief Construct a new State_Stepper2 object
		 * 
		 */
		State_Stepper2();

		/**
		 * @brief Construct a new State_Stepper2 object
		 * 
		 * @param name String representing state name
		 * @param state_transitions Map of transitions to determine what the next state will be
		 * @param root Root variable used for state machine
		 */
		State_Stepper2(StateName name, std::map<EventName, StateName> &state_transitions, Root* root);
		
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