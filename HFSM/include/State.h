#ifndef STATE_H
#define STATE_H

#include "State_Enums.h"

#include <iostream>
#include <map>

class Root;

/**
 * @file State.h
 * @class State
 * @brief Base state
 * 
 */
class State {
    public:

        /**
         * @brief Construct a new State object
         * 
         */
        State();

        /**
         * @brief Construct a new State object
         * 
         * @param name String representing state name
		 * @param state_transitions Map of transitions to determine what the next state will be
		 * @param root Root variable used for state machine
         */
        State(StateName &name, std::map<EventName, StateName> &state_transitions, Root* root);

        /**
		 * @brief Main execution method for state
		 * 
		 * @return Result of state execution 
		 */
        virtual EventName execute();

        /**
		 * @brief Execution method for state in display mode (DOESN'T WORK)
		 * 
		 * @return Result of state execution 
		 */
        virtual EventName unitExecute();

        /**
         * @brief Get the name of the state
         * 
         * @return StateName (Name of the state)
         */
        StateName getName();

        /**
         * @brief Get the next state based on the event that occured during state machine execution
         * 
         * @param curr_event The event that just occured in execution
         * @return StateName The next state to be transitioned to in the state machine
         */
        StateName getNextState(EventName curr_event);

    // Subclasses cannot access these variables unless they protected, not private 
    protected:

        /**
         * @brief Root object containing variables and methods accessible to all states
         * 
         */
        Root* root_;

        /**
         * @brief Name of the state
         * 
         */
        StateName name_;

        /**
         * @brief Map of transitions to map events occuring in the state machine to the next state
         * 
         */
        std::map<EventName,StateName> state_transitions_;
};

#endif