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
        int stepper_1_standby_pin_ = 0;
        int steps_per_revolution_ = 200;
        int stepper_1_pin_1_ = 11;
        int stepper_1_pin_2_ = 9;
        int stepper_1_pin_3_ = 5;
        int stepper_1_pin_4_ = 6;
        int stepper_speed_ = 30;
        bool gpio_initialized_ = false;
        int num_steps_ = 34;
        Stepper stepper_1_;
        std::map<StateName,State*> states_;
        Root();
        void addState(State* the_state);
        void execute();
        double getCurrentTime();

    private:

};
#endif