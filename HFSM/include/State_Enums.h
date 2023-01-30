#pragma once

enum StateName {
    STATE_A, 
    STATE_B,
    STATE_STEPPER1,
    END_STATE,
    STATE_INITIAL,
    STATE_FINAL,
    STATE_STEPPER2,
    STATE_DELAY,
    STATE_RCB_MOTOR
};

enum EventName {
    TIME_10,
    TIME_15,
    TIME_20,
    BASIC_SWIVEL,
    BASIC_ROTATE,
    TERMINATE,
    INITIALIZE,
    DELAY_1_SECOND,
    DELAY
};