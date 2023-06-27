#pragma once
/**
 * @file State_Enums.h 
 * 
 */

/**
 * @enum StateName
 * @brief Different possible states that can be run in this state machine
 * 
 */
enum StateName {

    /**
     * @brief State to test RCB Motor
     * 
     */
    STATE_RCB_MOTOR,

    /**
     * @brief State to test nacelle servo
     * 
     */
    STATE_NACELLE_SERVO,

    /**
     * @brief State to test lift locking servo
     * 
     */
    STATE_LIFT_SERVO,

    /**
     * @brief State to test lift motor
     * 
     */
    STATE_LIFT_MOTOR,

    /**
     * @brief State to test level servo
     * 
     */
    STATE_LEVEL_SERVO,

    /**
     * @brief State to test stepper for nacelle 1
     * 
     */
    STATE_STEPPER1,

    /**
     * @brief State to test stepper for nacelle 2
     * 
     */
    STATE_STEPPER2,

    /**
     * @brief State to test stepper for nacelle 3
     * 
     */
    STATE_STEPPER3,

    /**
     * @brief State to test SDR 1
     * 
     */
    STATE_SDR1,

    /**
     * @brief State to test SDR 2
     * 
     */
    STATE_SDR2,

    /**
     * @brief State run before launch detection to double check parameters
     * 
     */
    STATE_PRELAUNCH,

    /**
     * @brief State responsible for detecting launch
     * 
     */
    STATE_LAUNCH_DETECTION,

    /**
     * @brief State responsible for detecting apogee
     * 
     */
    STATE_APOGEE_DETECTION,

    /**
     * @brief State responsible for detecting landing
     * 
     */
    STATE_LANDING_DETECTION,

    /**
     * @brief State responsible for elevating the lift during the payload mission
     * 
     */
    STATE_FULL_LIFT,

    /**
     * @brief State responsible for leveling the key nacelle parallel with the horizon
     * 
     */
    STATE_FULL_LEVEL,

    /**
     * @brief State responsible for rotating RCB to put key nacelle on top
     * 
     */
    STATE_FULL_RCB,

    /**
     * @brief State responsible for "sleeping" the program until PDS is detached from Teensy
     * 
     */
    STATE_PDS_DELAY,

    /**
     * @brief State responsible for receiving RAFCO packets and performing the proper image filtering
     * 
     */
    STATE_RAFCO_MISSION,

    /**
     * @brief State responsible for determining the primary nacelle
     * 
     */
    STATE_CAMERA_CHECK,

    /**
     * @brief State responsible for terminating the program
     * 
     */
    END_STATE
 };

/**
 * @enum EventName
 * @brief Different possible events that can occur in this state machine
 * 
 */
enum EventName {

    /**
     * @brief Occurs when state machine has been initialized
     * 
     */
    INITIALIZE,

    /**
     * @brief Occurs when stepper motor has swiveled nacelle
     * 
     */
    BASIC_SWIVEL,

    /**
     * @brief Occurs when DC motor has rotated
     * 
     */
    BASIC_ROTATE,

    /**
     * @brief Occurs when servo moves
     * 
     */
    BASIC_SERVO,

    /**
     * @brief Occurs when program receives packets
     * 
     */
    RECEIVED_PACKETS,

    /**
     * @brief Occurs when the program has been delayed by a second
     * 
     */
    DELAY_1_SECOND,

    /**
     * @brief Occurs when the program has been delayed by some time threshold
     * 
     */
    DELAY,

    /**
     * @brief Occurs when the program needs to be terminated
     * 
     */
    TERMINATE,

    /**
     * @brief Occurs when the prelaunch state has been completed
     * 
     */
    PRELAUNCH_COMPLETE,

    /**
     * @brief Occurs when launch has been detected
     * 
     */
    LAUNCH_DETECTED,

    /**
     * @brief Occurs when apogee has been detected
     * 
     */
    APOGEE_DETECTED,

    /**
     * @brief Occurs when landing has been detected
     * 
     */
    LANDING_DETECTED,

    /**
     * @brief Occurs when the lift is successful
     * 
     */
    LIFT_SUCCESS,

    /**
     * @brief Occurs when there has been an error in the lift state
     * 
     */
    LIFT_FAILURE,

    /**
     * @brief Occurs when the level is successful
     * 
     */
    LEVEL_SUCCESS,

    /**
     * @brief Occurs when there has been an error in the level
     * 
     */
    LEVEL_FAILURE,

    /**
     * @brief Occurs when the RCB state is successful
     * 
     */
    RCB_SUCCESS,

    /**
     * @brief Occurs when there has been an error in the RCB state
     * 
     */
    RCB_FAILURE,

    /**
     * @brief Occurs when RAFCO state is successfully completed
     * 
     */
    RAFCO_COMPLETE,

    /**
     * @brief Occurs when backup RAFCO is completed, and want to try it with primary
     * 
     */
    RAFCO_REDO,

    /**
     * @brief Occurs when camera has been selected
     * 
     */
    CAMERA_PICKED
};