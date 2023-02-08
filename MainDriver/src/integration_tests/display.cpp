#include <iostream>
#include "Root.h"
#include "State.h"
#include "State_Stepper1.h"
#include "State_Stepper2.h"
#include "State_Stepper3.h"
#include "State_RCB_Motor.h"
#include "State_Nacelle_Servo.h"
#include "State_Lift_Servo.h"
#include "State_Lift_Motor.h"
#include "State_Level_Servo.h"
#include <map>
#include "pigpio.h"

std::string getStateName(StateName stateType) {
    std::string name;

    switch (stateType) {
        case STATE_RCB_MOTOR:
            name = "RCB Motor";
            break;
        case STATE_NACELLE_SERVO:
            name = "Nacelle Servo";
            break;
        case STATE_LIFT_SERVO:
            name = "Lift Servo";
            break;
        case STATE_LIFT_MOTOR:
            name = "Lift Motor";
            break;
        case STATE_LEVEL_SERVO:
            name = "Level Servo";
            break;
        case STATE_STEPPER1:
            name = "Stepper 1";
            break;
        case STATE_STEPPER2:
            name = "Stepper 2";
            break;
        case STATE_STEPPER3:
            name = "Stepper 3";
            break;
        case STATE_DELAY:
            name = "Delay";
            break;
        case END_STATE:
            name = "End State";
            break;
        default:
            name = "UNKNOWN";
            break;
    }

    return name;
}

void runFullStateMachine(Root &root, State &initial_state) {
    State *current_state = &initial_state;

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
}

int main() {
    bool is_unit_fsm = true;
    Root root(is_unit_fsm);

    // State Definitions and Declarations

    // State Nacelle Servo
    std::map<EventName, StateName> nacelle_servo_transitions;
    nacelle_servo_transitions.insert(std::pair<EventName, StateName>(BASIC_SERVO, STATE_RCB_MOTOR));
    State_Nacelle_Servo nacelle_servo(STATE_NACELLE_SERVO, nacelle_servo_transitions, &root);

    // State RCB Motor
    std::map<EventName, StateName> rcb_transitions;
    rcb_transitions.insert(std::pair<EventName, StateName>(BASIC_ROTATE, STATE_LIFT_SERVO));
    State_RCB_Motor rcb(STATE_RCB_MOTOR, rcb_transitions, &root);

    // State Lift Retention Servo
    std::map<EventName, StateName> lift_servo_transitions;
    lift_servo_transitions.insert(std::pair<EventName, StateName>(BASIC_SERVO, STATE_LIFT_MOTOR));
    State_Lift_Servo lift_servo(STATE_LIFT_SERVO, lift_servo_transitions, &root);

    // State Lift Motor Test
    std::map<EventName, StateName> lift_motor_transitions;
    lift_motor_transitions.insert(std::pair<EventName, StateName>(BASIC_ROTATE, STATE_LEVEL_SERVO));
    State_Lift_Motor lift_motor(STATE_LIFT_MOTOR, lift_motor_transitions, &root);

    // State Level Servo Test
    std::map<EventName, StateName> level_servo_transitions;
    level_servo_transitions.insert(std::pair<EventName, StateName>(BASIC_SERVO, STATE_STEPPER1));
    State_Level_Servo level_servo(STATE_LEVEL_SERVO, level_servo_transitions, &root);

    // State Stepper 1 Test
    std::map<EventName, StateName> stepper1_transitions;
    stepper1_transitions.insert(std::pair<EventName, StateName>(BASIC_SWIVEL, STATE_STEPPER2));
    State_Stepper1 stepper1(STATE_STEPPER1, stepper1_transitions, &root);

    // State Stepper 2 Test
    std::map<EventName, StateName> stepper2_transitions;
    stepper2_transitions.insert(std::pair<EventName, StateName>(BASIC_SWIVEL, STATE_STEPPER3));
    State_Stepper2 stepper2(STATE_STEPPER2, stepper2_transitions, &root);

    // State Stepper 3 Test
    std::map<EventName, StateName> stepper3_transitions;
    stepper3_transitions.insert(std::pair<EventName, StateName>(BASIC_SWIVEL, END_STATE));
    State_Stepper3 stepper3(STATE_STEPPER3, stepper3_transitions, &root);

    // Add States to Machine
    root.addState(&nacelle_servo);
    root.addState(&rcb);
    root.addState(&lift_servo);
    root.addState(&lift_motor);
    root.addState(&level_servo);
    root.addState(&stepper1);
    root.addState(&stepper2);
    root.addState(&stepper3);    


    bool runTests = true;
    int userInput;

    State *current_state = &nacelle_servo;
    State *next_state = current_state;
    EventName curr_event;

    while (runTests) {
        std::cout << "------------OPTIONS------------" << std::endl;
        std::cout << "1. Run whole state machine" << std::endl;
        std::cout << "2. Run a single state" << std::endl;
        std::cout << "3. Quit" << std::endl;

        std::cout << "What would you like to do? ";
        std::cin >> userInput;

        if (userInput == 1) {
            runFullStateMachine(root, nacelle_servo);
        } else if (userInput == 2) {
            std::cout << "------------STATES-------------" << std::endl;
            int numStates = END_STATE + 1;
            for (int i = 0; i < numStates; i++) {
                std::cout << std::to_string(i + 1) << ". " << getStateName((StateName) i) << std::endl;
            }
            std::cout << std::to_string(numStates + 1) << ". " << "Quit" << std::endl;
            
            std::cout << "What state would you like to run? ";
            std::cin >> userInput;

            if (0 < userInput && userInput < numStates - 1) {
                StateName stateType = (StateName) (userInput-1);

                current_state = root.states_[stateType];
                if (root.is_unit_fsm_) {
                    curr_event = current_state->unitExecute();
                    // Automatically sleep for 1 second after
                    usleep(root.unit_test_delay_ms_);
                }
                else
                {
                    curr_event = current_state->execute();
                }
            } else if (userInput == numStates + 1) {
                runTests = false;
            } else {
                std::cout << "Doing nothing" << std::endl;
            }
        } else if (userInput == 3) {
            runTests = false;
        }
    }

    return 0;
}