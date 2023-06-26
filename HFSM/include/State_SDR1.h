#ifndef STATE_SDR1_H
#define STATE_SDR1_H

#include <iostream>
#include <map>
#include "State.h"
#include <string.h>
#include "pigpio.h"

class Root;

/**
 * @file State_SDR1.h
 * @class State_SDR1
 * @brief State responsible for debugging SDR 1
 * 
 */
class State_SDR1 : public State {
	public:

		/**
		 * @brief Construct a new State_SDR1 object
		 * 
		 */
		State_SDR1();

		/**
		 * @brief Construct a new State_SDR1 object
		 * 
		 * @param name String representing state name
		 * @param state_transitions Map of transitions to determine what the next state will be
		 * @param root Root variable used for state machine
		 */
		State_SDR1(StateName name, std::map<EventName, StateName> &state_transitions, Root* root);

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