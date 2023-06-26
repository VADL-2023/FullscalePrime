#ifndef STATE_LIFT_SERVO_H
#define STATE_LIFT_SERVO_H

#include <iostream>
#include <map>
#include "State.h"
#include "pigpio.h"

class Root;

/**
 * @file State_Lift_Servo.h
 * @class State_Lift_Servo
 * @brief State responsible for testing lift servo to see if it can move
 * 
 */
class State_Lift_Servo : public State {
	public:

		/**
		 * @brief Construct a new State_Lift_Servo object
		 * 
		 */
		State_Lift_Servo();

		/**
		 * @brief Construct a new State_Lift_Servo object
		 * 
		 * @param name String representing state name
		 * @param state_transitions Map of transitions to determine what the next state will be
		 * @param root Root variable used for state machine
		 */
		State_Lift_Servo(StateName name, std::map<EventName, StateName> &state_transitions, Root* root);

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