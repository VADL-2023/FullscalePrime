#ifndef STATE_FULL_LIFT_H
#define STATE_FULL_LIFT_H

#include <iostream>
#include <map>
#include "State.h"

class Root;

/**
 * @file State_Full_Lift.h
 * @class State_Full_Lift
 * @brief State responsible for elevating the key nacelle with the lift
 * 
 */
class State_Full_Lift : public State {
	public:

		/**
		 * @brief Construct a new State_Full_Lift object
		 * 
		 */
		State_Full_Lift();

		/**
		 * @brief Construct a new State_Full_Lift object
		 * 
		 * @param name String representing state name
		 * @param state_transitions Map of transitions to determine what the next state will be
		 * @param root Root variable used for state machine
		 */
		State_Full_Lift(StateName name, std::map<EventName, StateName> &state_transitions, Root* root);
		
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