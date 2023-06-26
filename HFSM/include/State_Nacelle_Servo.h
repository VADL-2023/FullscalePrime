#ifndef STATE_NACELLE_SERVO_H
#define STATE_NACELLE_SERVO_H

#include <iostream>
#include <map>
#include "State.h"
#include "pigpio.h"

class Root;

/**
 * @file State_Nacelle_Servo.h
 * @class State_Nacelle_Servo
 * @brief State responsible for testing nacelle locking servo movement
 * 
 */
class State_Nacelle_Servo : public State {
	public:

		/**
		 * @brief Construct a new State_Nacelle_Servo object
		 * 
		 */
		State_Nacelle_Servo();

		/**
		 * @brief Construct a new State_Nacelle_Servo object
		 * 
		 * @param name String representing state name
		 * @param state_transitions Map of transitions to determine what the next state will be
		 * @param root Root variable used for state machine
		 */
		State_Nacelle_Servo(StateName name, std::map<EventName, StateName> &state_transitions, Root* root);

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