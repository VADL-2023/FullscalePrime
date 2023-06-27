#ifndef STATE_LIFT_MOTOR_H
#define STATE_LIFT_MOTOR_H

#include <iostream>
#include <map>
#include "State.h"
#include "pigpio.h"

class Root;

/**
 * @file State_Lift_Motor.h
 * @class State_Lift_Motor
 * @brief State responsible for testing lift motor without limit switch sensors
 * 
 */
class State_Lift_Motor : public State {
	public:
		/**
		 * @brief Construct a new State_Lift_Motor object
		 * 
		 */
		State_Lift_Motor();

		/**
		 * @brief Construct a new State_Lift_Motor object
		 * 
		 * @param name String representing state name
		 * @param state_transitions Map of transitions to determine what the next state will be
		 * @param root Root variable used for state machine
		 */
		State_Lift_Motor(StateName name, std::map<EventName, StateName> &state_transitions, Root* root);
		
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