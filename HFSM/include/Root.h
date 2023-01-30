#ifndef ROOT_H
#define ROOT_H

#include "State_Enums.h"

#include <iostream>
#include <map>
#include <ctime>
#include <string>
#include <unistd.h>
#include <thread>
#include "Stepper.h"

class State;

class Root {
    public:
        double start_time_;
        bool is_unit_fsm_;
        int steps_per_revolution_ = 200;
        int stepper_1_standby_pin_ = 0;
        int stepper_1_pin_1_ = 11;
        int stepper_1_pin_2_ = 9;
        int stepper_1_pin_3_ = 5;
        int stepper_1_pin_4_ = 6;
        int stepper_2_standby_pin_ = 27;
        int stepper_2_pin_1_ = 17;
        int stepper_2_pin_2_ = 4;
        int stepper_2_pin_3_ = 22;
        int stepper_2_pin_4_ = 10;
        int stepper_speed_ = 30;
        bool gpio_initialized_ = false;
        int num_steps_ = 34;
        int stepper_threshold_ = 10000;
        Stepper stepper_1_;
        Stepper stepper_2_;
        std::map<StateName,State*> states_;
        Root();
        Root(bool is_unit_fsm);
        void addState(State* the_state);
        void execute();
        double getCurrentTime();

    private:

};
#endif