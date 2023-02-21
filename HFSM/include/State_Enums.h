#pragma once

enum StateName {
    STATE_RCB_MOTOR,
    STATE_NACELLE_SERVO,
    STATE_LIFT_SERVO,
    STATE_LIFT_MOTOR,
    STATE_LEVEL_SERVO,
    STATE_STEPPER1,
    STATE_STEPPER2,
    STATE_STEPPER3,
    STATE_SDR1,
    STATE_SDR2,
    STATE_DELAY,
    STATE_PRELAUNCH,
    STATE_LAUNCH_DETECTION,
    STATE_APOGEE_DETECTION,
    STATE_LANDING_DETECTION,
    STATE_FULL_LIFT,
    STATE_FULL_LEVEL,
    STATE_FULL_RCB,
    STATE_PDS_DELAY,
    STATE_RAFCO_MISSION,
    END_STATE
 };

enum EventName {
    INITIALIZE,
    BASIC_SWIVEL,
    BASIC_ROTATE,
    BASIC_SERVO,
    RECEIVED_PACKETS,
    DELAY_1_SECOND,
    DELAY,
    TERMINATE,
    PRELAUNCH_COMPLETE,
    LAUNCH_DETECTED,
    APOGEE_DETECTED,
    LANDING_DETECTED,
    LIFT_SUCCESS,
    LIFT_FAILURE,
    LEVEL_SUCCESS,
    LEVEL_FAILURE,
    RCB_SUCCESS,
    RCB_FAILURE,
    RAFCO_COMPLETE
};