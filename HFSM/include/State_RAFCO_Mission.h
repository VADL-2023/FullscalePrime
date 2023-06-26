#ifndef STATE_RAFCO_MISSION_H
#define STATE_RAFCO_MISSION_H

#include <iostream>
#include <map>
#include "State.h"

class Root;

/**
 * @file State_RAFCO_Mission.h
 * @class State_RAFCO_Mission
 * @brief State responsible for receiving APRS packets and perform image processing with camera nacelles
 * 
 */
class State_RAFCO_Mission : public State {
	public:
		
		/**
		 * @brief Construct a new State_RAFCO_Mission object
		 * 
		 */
		State_RAFCO_Mission();

		/**
		 * @brief Construct a new State_RAFCO_Mission object
		 * 
		 * @param name String representing state name
		 * @param state_transitions Map of transitions to determine what the next state will be
		 * @param root Root variable used for state machine
		 */
		State_RAFCO_Mission(StateName name, std::map<EventName, StateName> &state_transitions, Root* root);

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