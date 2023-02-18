#include <iostream>
#include "State_Prelaunch.h"
#include "State_Enums.h"
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
#include "State_Launch_Detection.h"
#include "State_Apogee_Detection.h"
#include "State_Landing_Detection.h"
#include "State_Full_Lift.h"
#include "State_Full_Level.h"
#include "State_Full_RCB.h"
#include "State_PDS_Delay.h"
#include "State_SDR1.h"
#include "State_SDR2.h"
#include <map>
#include "pigpio.h"
#include <unistd.h>
#include <iostream>

std::string getStateName(StateName stateType)
{
    std::string name;
    switch (stateType)
    {
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
    case STATE_SDR1:
        name = "State SDR 1";
        break;
    case STATE_SDR2:
        name = "State SDR 2";
        break;
    case STATE_DELAY:
        name = "Delay";
        break;
    case STATE_PRELAUNCH:
        name = "Pre launch";
        break;
    case STATE_LAUNCH_DETECTION:
        name = "Launch Detection";
        break;
    case STATE_APOGEE_DETECTION:
        name = "Apogee Detection";
        break;
    case STATE_LANDING_DETECTION:
        name = "Landing Detection";
        break;
    case STATE_FULL_LIFT:
        name = "Full Lift";
        break;
    case STATE_FULL_LEVEL:
        name = "Full Level";
        break;
    case STATE_FULL_RCB:
        name = "Full RCB";
        break;
    case STATE_PDS_DELAY:
        name = "PDS Delay";
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

void runFullStateMachine(Root &root, State *initial_state)
{
    State *current_state = initial_state;

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

int main()
{
    bool is_unit_fsm = false;
    Root root(is_unit_fsm);

    // State Definitions and Declarations

    // State Prelaunch
    StateName prelaunch_name = STATE_PRELAUNCH;
    std::map<EventName, StateName> prelaunch_transitions;
    prelaunch_transitions.insert(std::pair<EventName, StateName>(PRELAUNCH_COMPLETE, STATE_LAUNCH_DETECTION));
    State_Prelaunch prelaunch(prelaunch_name, prelaunch_transitions, &root);

    // State Launch Detection
    StateName launch_detection_name = STATE_LAUNCH_DETECTION;
    std::map<EventName, StateName> launch_detection_transitions;
    launch_detection_transitions.insert(std::pair<EventName, StateName>(LAUNCH_DETECTED, STATE_APOGEE_DETECTION));
    State_Launch_Detection launch_detection(launch_detection_name, launch_detection_transitions, &root);

    // State Apogee Detection
    StateName apogee_detection_name = STATE_APOGEE_DETECTION;
    std::map<EventName, StateName> apogee_detection_transitions;
    apogee_detection_transitions.insert(std::pair<EventName, StateName>(APOGEE_DETECTED, STATE_LANDING_DETECTION));
    State_Apogee_Detection apogee_detection(apogee_detection_name, apogee_detection_transitions, &root);

    // State Landing Detection
    StateName landing_detection_name = STATE_LANDING_DETECTION;
    std::map<EventName, StateName> landing_detection_transitions;
    landing_detection_transitions.insert(std::pair<EventName, StateName>(LANDING_DETECTED, STATE_PDS_DELAY));
    State_Landing_Detection landing_detection(landing_detection_name, landing_detection_transitions, &root);

    // State Landing Detection
    StateName pds_delay_name = STATE_PDS_DELAY;
    std::map<EventName, StateName> pds_delay_transitions;
    pds_delay_transitions.insert(std::pair<EventName, StateName>(DELAY, STATE_FULL_RCB));
    State_PDS_Delay pds_delay(pds_delay_name, pds_delay_transitions, &root);

    // State RCB Detection
    StateName full_rcb_name = STATE_FULL_RCB;
    std::map<EventName, StateName> full_rcb_transitions;
    full_rcb_transitions.insert(std::pair<EventName, StateName>(RCB_SUCCESS, STATE_FULL_LIFT));
    full_rcb_transitions.insert(std::pair<EventName, StateName>(RCB_FAILURE, STATE_SDR1));
    State_Full_RCB full_rcb(full_rcb_name, full_rcb_transitions, &root);

    // State Full Lift
    StateName full_lift_name = STATE_FULL_LIFT;
    std::map<EventName, StateName> full_lift_transitions;
    full_lift_transitions.insert(std::pair<EventName, StateName>(LIFT_SUCCESS, STATE_FULL_LEVEL));
    full_lift_transitions.insert(std::pair<EventName, StateName>(LIFT_FAILURE, END_STATE));
    State_Full_Lift full_lift(full_lift_name, full_lift_transitions, &root);

    // State Full Level
    StateName full_level_name = STATE_FULL_LEVEL;
    std::map<EventName, StateName> full_level_transitions;
    full_level_transitions.insert(std::pair<EventName, StateName>(LEVEL_SUCCESS, STATE_SDR1));
    full_level_transitions.insert(std::pair<EventName, StateName>(LEVEL_FAILURE, STATE_SDR1));
    State_Full_Level full_level(full_level_name, full_level_transitions, &root);

    // State Stepper 2
    /*StateName stepper2_name = STATE_STEPPER2;
    std::map<EventName, StateName> stepper2_transitions;
    stepper2_transitions.insert(std::pair<EventName, StateName>(BASIC_SWIVEL, STATE_SDR1));
    State_Stepper2 stepper2(stepper2_name, stepper2_transitions, &root);*/

    // State Stepper 2
    StateName sdr1_name = STATE_SDR1;
    std::map<EventName, StateName> sdr1_transitions;
    sdr1_transitions.insert(std::pair<EventName, StateName>(RECEIVED_PACKETS, END_STATE));
    State_SDR1 sdr1(sdr1_name, sdr1_transitions, &root);

    // Add States to Machine
    root.addState(&prelaunch);
    root.addState(&launch_detection);
    root.addState(&apogee_detection);
    root.addState(&landing_detection);
    root.addState(&pds_delay);
    root.addState(&full_rcb);
    root.addState(&full_lift);
    root.addState(&full_level);
    root.addState(&stepper2);
    root.addState(&sdr1);

    bool runTests = true;
    std::string userInput;
    int userInt = 0;

    State *current_state = &prelaunch;
    State *next_state = current_state;
    EventName curr_event;

    while (runTests)
    {
        std::cout << "------------OPTIONS------------" << std::endl;
        std::cout << "1. Run whole state machine" << std::endl;
        std::cout << "2. Run a single state" << std::endl;
        std::cout << "3. Quit" << std::endl;

        std::cout << "What would you like to do? ";
        std::cin >> userInput;

        try
        {
            userInt = stoi(userInput);
        }
        catch (...)
        {
            std::cout << "Invalid selection" << std::endl;
            userInt = 0;
        }

        if (userInt == 1)
        {
            runFullStateMachine(root, current_state);
            runTests = false;
            return 0;
        }
        else if (userInt == 2)
        {
            std::cout << "------------STATES-------------" << std::endl;
            int numStates = END_STATE + 1;
            for (int i = 0; i < numStates; i++)
            {
                std::cout << std::to_string(i + 1) << ". " << getStateName((StateName)i) << std::endl;
            }
            std::cout << std::to_string(numStates + 1) << ". "
                      << "Quit" << std::endl;

            std::cout << "What state would you like to run? ";
            std::cin >> userInput;

            try
            {
                userInt = stoi(userInput);
            }
            catch (...)
            {
                std::cout << "Invalid selection" << std::endl;
                userInt = numStates;
            }

            if (0 < userInt && userInt < numStates)
            {
                StateName stateType = (StateName)(userInt - 1);
                if (stateType == STATE_LAUNCH_DETECTION)
                {
                    std::cout << "WARNING: YOU SHOULD NOT RUN THIS STATE ALONE. PLEASE RUN STATE_PRELAUNCH FIRST" << std::endl;
                }
                else if (stateType == STATE_APOGEE_DETECTION)
                {
                    std::cout << "WARNING: YOU SHOULD NOT RUN THIS STATE ALONE. PLEASE RUN STATE_PRELAUNCH and STATE_LAUNCH_DETECTION FIRST" << std::endl;
                }
                else if (stateType == STATE_LANDING_DETECTION)
                {
                    std::cout << "WARNING: YOU SHOULD NOT RUN THIS STATE ALONE. PLEASE RUN STATE_PRELAUNCH and STATE_LAUNCH_DETECTION and STATE_APOGEE_DETECTION FIRST" << std::endl;
                }
                current_state = root.states_[stateType];
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
            }
            else if (numStates - 1 <= userInt && userInt <= numStates)
            {
                std::cout << "Doing nothing" << std::endl;
            }
            else
            {
                runTests = false;
                return 0;
            }
        }
        else if (userInt == 3)
        {
            runTests = false;
            return 0;
        }
    }

    return 0;
}