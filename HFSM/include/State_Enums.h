#pragma once

enum StateName {
    STATE_A, 
    STATE_B,
    STATE_STEPPER,
    END_STATE,
    STATE_INITIAL,
    STATE_FINAL
};

enum EventName {
    TIME_10,
    TIME_15,
    TIME_20,
    BASIC_SWIVEL,
    TERMINATE,
    INITIALIZE,
};