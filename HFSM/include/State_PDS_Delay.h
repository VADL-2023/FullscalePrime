#ifndef STATE_PDS_DELAY_H
#define STATE_PDS_DELAY_H

#include <iostream>
#include <map>
#include "State.h"

class Root;

/**
 * @file State_PDS_Delay.h
 * @class State_PDS_Delay
 * @brief State responsible for program to "sleep" while waiting for Teensy parachute to detach
 * 
 */
class State_PDS_Delay : public State {
	public:

		/**
		 * @brief Construct a new State_PDS_Delay object
		 * 
		 */
		State_PDS_Delay();

		/**
		 * @brief Construct a new State_PDS_Delay object
		 * 
		 * @param name String representing state name
		 * @param state_transitions Map of transitions to determine what the next state will be
		 * @param root Root variable used for state machine
		 */
		State_PDS_Delay(StateName name, std::map<EventName, StateName> &state_transitions, Root* root);
		
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