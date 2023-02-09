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
    STATE_DELAY,
    END_STATE,
    STATE_PRELAUNCH
};

enum EventName {
    INITIALIZE,
    BASIC_SWIVEL,
    BASIC_ROTATE,
    BASIC_SERVO,
    DELAY_1_SECOND,
    DELAY,
    TERMINATE,
    PRELAUNCH
};