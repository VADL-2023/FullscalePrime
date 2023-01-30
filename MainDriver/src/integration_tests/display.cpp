#include <iostream>
#include "Root.h"
#include "State.h"
#include "State_Stepper1.h"
#include "State_Stepper2.h"
#include "State_Initial.h"
#include "State_Delay.h"
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
    initial_transitions.insert(std::pair<EventName, StateName>(INITIALIZE, STATE_STEPPER1));
    State_Initial initial(initial_name, initial_transitions, &root);

    // State Stepper 1 Test
    StateName stepper1_name = STATE_STEPPER1;
    std::map<EventName, StateName> stepper1_transitions;
    stepper1_transitions.insert(std::pair<EventName, StateName>(BASIC_SWIVEL, STATE_DELAY));
    State_Stepper1 stepper1(stepper1_name, stepper1_transitions, &root);

    //State Delay 5 Seconds
    StateName delay_name = STATE_DELAY;
    std::map<EventName, StateName> delay_transitions;
    delay_transitions.insert(std::pair<EventName, StateName>(DELAY_1_SECOND, STATE_STEPPER2));
    State_Delay delay1(delay_name, delay_transitions, &root);

    // State Stepper 2 Test
    StateName stepper2_name = STATE_STEPPER2;
    std::map<EventName, StateName> stepper2_transitions;
    stepper2_transitions.insert(std::pair<EventName, StateName>(BASIC_SWIVEL, STATE_FINAL));
    State_Stepper2 stepper2(stepper2_name, stepper2_transitions, &root);

    // State Final
    StateName final_name = STATE_FINAL;
    std::map<EventName, StateName> final_transitions;
    final_transitions.insert(std::pair<EventName, StateName>(TERMINATE, END_STATE));
    State_Final final(final_name, final_transitions, &root);

    // Add States to Machine
    root.addState(&initial);
    root.addState(&stepper1);
    root.addState(&delay1);
    root.addState(&stepper2);
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