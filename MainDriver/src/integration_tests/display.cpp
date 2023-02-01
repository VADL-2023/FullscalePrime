#include <iostream>
#include "Root.h"
#include "State.h"
#include "State_Stepper1.h"
#include "State_Stepper2.h"
#include "State_Stepper3.h"
#include "State_Initial.h"
#include "State_RCB_Motor.h"
#include "State_Nacelle_Servo.h"
#include "State_Lift_Servo.h"
#include "State_Final.h"
#include "State_Lift_Motor.h"
#include "State_Level_Servo.h"
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
    initial_transitions.insert(std::pair<EventName, StateName>(INITIALIZE, STATE_NACELLE_SERVO));
    State_Initial initial(initial_name, initial_transitions, &root);

    // State Nacelle Servo
    StateName nacelle_servo_name = STATE_NACELLE_SERVO;
    std::map<EventName, StateName> nacelle_servo_transitions;
    nacelle_servo_transitions.insert(std::pair<EventName, StateName>(BASIC_SERVO, STATE_RCB_MOTOR));
    State_Nacelle_Servo nacelle_servo(nacelle_servo_name, nacelle_servo_transitions, &root);

    // State RCB Motor
    StateName rcb_name = STATE_RCB_MOTOR;
    std::map<EventName, StateName> rcb_transitions;
    rcb_transitions.insert(std::pair<EventName, StateName>(BASIC_ROTATE, STATE_LIFT_SERVO));
    State_RCB_Motor rcb(rcb_name, rcb_transitions, &root);

    // State Lift Retention Servo
    StateName lift_servo_name = STATE_LIFT_SERVO;
    std::map<EventName, StateName> lift_servo_transitions;
    lift_servo_transitions.insert(std::pair<EventName, StateName>(BASIC_SERVO, STATE_LIFT_MOTOR));
    State_Lift_Servo lift_servo(lift_servo_name, lift_servo_transitions, &root);

    // State Lift Motor Test
    StateName lift_motor_name = STATE_LIFT_MOTOR;
    std::map<EventName, StateName> lift_motor_transitions;
    lift_motor_transitions.insert(std::pair<EventName, StateName>(BASIC_ROTATE, STATE_LEVEL_SERVO));
    State_Lift_Motor lift_motor(lift_motor_name, lift_motor_transitions, &root);

    // State Level Servo Test
    StateName level_servo_name = STATE_LEVEL_SERVO;
    std::map<EventName, StateName> level_servo_transitions;
    level_servo_transitions.insert(std::pair<EventName, StateName>(BASIC_SERVO, STATE_STEPPER1));
    State_Level_Servo level_servo(level_servo_name, level_servo_transitions, &root);

    // State Stepper 1 Test
    StateName stepper1_name = STATE_STEPPER1;
    std::map<EventName, StateName> stepper1_transitions;
    stepper1_transitions.insert(std::pair<EventName, StateName>(BASIC_SWIVEL, STATE_STEPPER2));
    State_Stepper1 stepper1(stepper1_name, stepper1_transitions, &root);

    // State Stepper 2 Test
    StateName stepper2_name = STATE_STEPPER2;
    std::map<EventName, StateName> stepper2_transitions;
    stepper2_transitions.insert(std::pair<EventName, StateName>(BASIC_SWIVEL, STATE_STEPPER3));
    State_Stepper2 stepper2(stepper2_name, stepper2_transitions, &root);

    // State Stepper 3 Test
    StateName stepper3_name = STATE_STEPPER3;
    std::map<EventName, StateName> stepper3_transitions;
    stepper3_transitions.insert(std::pair<EventName, StateName>(BASIC_SWIVEL, STATE_FINAL));
    State_Stepper3 stepper3(stepper3_name, stepper3_transitions, &root);

    // State Final
    StateName final_name = STATE_FINAL;
    std::map<EventName, StateName> final_transitions;
    final_transitions.insert(std::pair<EventName, StateName>(TERMINATE, END_STATE));
    State_Final final(final_name, final_transitions, &root);

    // Add States to Machine
    root.addState(&initial);
    root.addState(&nacelle_servo);
    root.addState(&rcb);
    root.addState(&lift_servo);
    root.addState(&lift_motor);
    root.addState(&level_servo);
    root.addState(&stepper1);
    root.addState(&stepper2);
    root.addState(&stepper3);
    root.addState(&final);
    State *current_state = &initial;

    root.start_time_ = root.getCurrentTime();
    std::cout << "Start machine" << std::endl;
    EventName curr_event;

    if (root.is_unit_fsm_)
    {
        curr_event = current_state->unitExecute();
        usleep(root.unit_test_delay_ms_);
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
            // Automatically sleep for 1 second after
            usleep(root.unit_test_delay_ms_);
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