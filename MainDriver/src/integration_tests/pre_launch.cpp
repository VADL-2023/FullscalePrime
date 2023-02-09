#include <iostream>
#include "Root.h"
#include "State.h"
#include "State_Prelaunch.h"
#include <map>
#include "pigpio.h"
int main()
{
    bool is_unit_fsm = false;
    Root root(is_unit_fsm);

    // State Definitions and Declarations

    // State Prelaunch
    StateName prelaunch_name = STATE_PRELAUNCH;
    std::map<EventName, StateName> prelaunch_transitions;
    prelaunch_transitions.insert(std::pair<EventName, StateName>(BASIC_SERVO, END_STATE));
    State_Prelaunch prelaunch(prelaunch_name, prelaunch_transitions, &root);

    // Add States to Machine
    root.addState(&prelaunch);
    State *current_state = &prelaunch;

    root.start_time_ = root.getCurrentTime();
    std::cout << "Start machine" << std::endl;
    EventName curr_event;

    if (root.is_unit_fsm_)
    {
        curr_event = current_state->unitExecute();
    }
    else
    {
        curr_event = current_state->execute();
    }
    StateName next_state = current_state->getNextState(curr_event);
    while (next_state != END_STATE)
    {
        current_state = root.states_[next_state];
        if (root.is_unit_fsm_)
        {
            curr_event = current_state->unitExecute();
        }
        else
        {
            curr_event = current_state->execute();
        }
        next_state = current_state->getNextState(curr_event);
    }

    // Temporary termination thing
    std::cout << "State Machine Completed" << std::endl;
    return 0;
}