#include <iostream>
#include "Root.h"
#include "State.h"
#include "State_Lift_Motor.h"
#include "State_Initial.h"
#include "State_Final.h"
#include <map>
#include "pigpio.h"
int main()
{
    bool is_unit_fsm = true;
    Root root(is_unit_fsm);

    // State Definitions and Declarations

    // State Initial
    StateName initial_name = STATE_INITIAL;
    std::map<EventName, StateName> initial_transitions;
    initial_transitions.insert(std::pair<EventName, StateName>(INITIALIZE, STATE_LIFT_MOTOR));
    State_Initial initial(initial_name, initial_transitions, &root);

    // lift Motor Test
    StateName lift_motor_name = STATE_LIFT_MOTOR;
    std::map<EventName, StateName> lift_motor_transitions;
    lift_motor_transitions.insert(std::pair<EventName, StateName>(BASIC_ROTATE, STATE_FINAL));
    State_Lift_Motor lift_motor(lift_motor_name, lift_motor_transitions, &root);

    // State Final
    StateName final_name = STATE_FINAL;
    std::map<EventName, StateName> final_transitions;
    final_transitions.insert(std::pair<EventName, StateName>(TERMINATE, END_STATE));
    State_Final final(final_name, final_transitions, &root);

    // Add States to Machine
    root.addState(&initial);
    root.addState(&lift_motor);
    root.addState(&final);
    State *current_state = &initial;

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