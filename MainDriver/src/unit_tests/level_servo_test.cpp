#include <iostream>
#include "Root.h"
#include "State.h"
#include "State_Level_Servo.h"
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
    initial_transitions.insert(std::pair<EventName, StateName>(INITIALIZE, STATE_LEVEL_SERVO));
    State_Initial initial(initial_name, initial_transitions, &root);

    // State Level Retention Servo Test
    StateName level_servo_name = STATE_LEVEL_SERVO;
    std::map<EventName, StateName> level_servo_transitions;
    level_servo_transitions.insert(std::pair<EventName, StateName>(BASIC_SERVO, STATE_FINAL));
    State_Level_Servo level_servo(level_servo_name, level_servo_transitions, &root);

    // State Final
    StateName final_name = STATE_FINAL;
    std::map<EventName, StateName> final_transitions;
    final_transitions.insert(std::pair<EventName, StateName>(TERMINATE, END_STATE));
    State_Final final(final_name, final_transitions, &root);

    // Add States to Machine
    root.addState(&initial);
    root.addState(&level_servo);
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