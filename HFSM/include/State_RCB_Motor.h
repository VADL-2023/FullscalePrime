#ifndef STATE_RCB_MOTOR_H
#define STATE_RCB_MOTOR_H

#include <iostream>
#include <map>
#include "State.h"
#include "pigpio.h"

class Root;

/**
 * @file State_RCB_Motor.h
 * @class State_RCB_Motor
 * @brief State responsible for testing RCB motor without IMU
 * 
 */
class State_RCB_Motor : public State {
	public:

		/**
		 * @brief Construct a new State_RCB_Motor object
		 * 
		 */
		State_RCB_Motor();

		/**
		 * @brief Construct a new State_RCB_Motor object
		 * 
		 * @param name String representing state name
		 * @param state_transitions Map of transitions to determine what the next state will be
		 * @param root Root variable used for state machine
		 */
		State_RCB_Motor(StateName name, std::map<EventName, StateName> &state_transitions, Root* root);
		
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