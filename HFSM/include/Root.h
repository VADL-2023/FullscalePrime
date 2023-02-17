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
#include "PacketReceiver.h"

class State;

class Root {
    public:
        bool gpio_initialized_ = false;
        double start_time_;
        bool is_unit_fsm_;
        int unit_test_delay_ms_ = 1000000;

        int steps_per_revolution_ = 200;
        int stepper_speed_ = 30;
        int num_steps_ = 34;
        int stepper_threshold_ = 5000;
        int pwm_motor_max_ = 255;

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

        int stepper_3_standby_pin_ = 8;
        int stepper_3_pin_1_ = 7;
        int stepper_3_pin_2_ = 1;
        int stepper_3_pin_3_ = 25;
        int stepper_3_pin_4_ = 24;

        Stepper stepper_1_;
        Stepper stepper_2_;
        Stepper stepper_3_;

        int rcb_p_ = 26;
        int rcb_n_ = 19;
        int rcb_enable_ = 13;
        int rcb_lift_standby_ = 21;
        int rcb_time_threshold_ = 5000;

        int nacelle_servo_ = 14;
        uint16_t servo_pulse_min_ = 750;
        uint16_t servo_pulse_max_ = 2250;
        uint8_t servo_deg_range_ = 180;

        int lift_servo_ = 2;

        int lift_p_ = 20;
        int lift_n_ = 16;
        int lift_enable_ = 12;
        int lift_time_threshold_ = 25000;

        int level_servo_ = 3;
        
        std::map<StateName,State*> states_;

        PacketReceiver radio1 = PacketReceiver(1);
        PacketReceiver radio2 = PacketReceiver(2);

        Root();
        Root(bool is_unit_fsm);
        ~Root();
        
        void addState(State* the_state);
        void execute();
        double getCurrentTime();
        float angleToPulseWidth(double pulse_max,double pulse_min,double range,float angle);

    private:

};
#endif