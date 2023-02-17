#ifndef ROOT_H
#define ROOT_H

#include "State_Enums.h"

#include <iostream>
#include <map>
#include <ctime>
#include <string>
#include <unistd.h>
#include <thread>
#include <math.h>
#include "sensors.h"
#include "config_IMU.hpp"
#include "Log.h"
#include "PacketReceiver.h"
#include "Stepper.h"

class State;

class Root
{
public:
    bool gpio_initialized_ = false;
    double start_time_;
    bool is_unit_fsm_;
    int unit_test_delay_ms_ = 1000000;

    int pds_delay_ = 60000;

    int steps_per_revolution_ = 200;
    int stepper_speed_ = 30;
    int num_steps_ = 40;
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
    int rcb_angle_threshold_ = 1;
    bool is_aligned_ = false;
    int full_rcb_time_threshold_ = 40000;

    int nacelle_servo_ = 14;
    uint16_t servo_pulse_min_ = 500;
    uint16_t servo_pulse_max_ = 2250;
    uint8_t servo_deg_range_ = 90;
    int nacelle_lock_ = 1375;
    int nacelle_unlock_ = 2250;

    int lift_servo_ = 2;
    double unlock_lift_angle_ = 0;
    int lift_base_limit_switch_ = 18;
    int lift_final_limit_switch_ = 23;
    int lift_unit_time_threshold_ = 2000;
    int lift_lock_ = 1500;
    int lift_unlock_ = 900;

    int lift_p_ = 20;
    int lift_n_ = 16;
    int lift_enable_ = 12;
    int lift_time_threshold_ = 45000;

    int level_servo_ = 3;
    int num_level_samples_ = 20;
    double ideal_level_angle_ = 180;
    int servo_up_pos_ = 0;
    int servo_down_pos_ = 45;
    double max_down_angle_ = 15;
    double max_up_angle_ = -15;

    std::map<StateName, State *> states_;

    // conversion factors
    float ft_2_m_ = 0.3048;      // [m/ft]
    float m_2_ft_ = 1 / ft_2_m_; // [ft/m]
    float c_2_k_ = 273.15;       // Celsius to Kelvin
    float km_2_m_ = 0.001;       // [km/m]

    // constants
    float r_ = 287;           // [kg/JK] universal gas constant
    float b_ = 6.5 * km_2_m_; // [K/m] variation of temperature within the troposphere

    // fixed flight parameters
    float t_burn_ = 15;            // [s] motor burn time
    float sampling_frequency_ = 20; // [Hz] how fast does the IMU sample data
    bool restart_ = false;          // tells the program whether or not we NO-GOed
    bool time_delay_enabled_ = false;

    // TODO: double check these flight parameters
    // possibly variable flight parameters (stuff we might change)
    float accel_roof_ = 3;                                                                      // how many g's does the program need to see in order for launch to be detected
    int num_data_points_checked_4_launch_ = 8;                                                  // how many acceleration points are averaged to see if data set is over accel_roof_
    int num_data_points_checked_4_apogee_ = 10;                                                 // how many altitude points must a new max not be found for apogee to be declared
    int num_seconds_no_new_minimum_ = 10;                                                       // [s] number of seconds to wait for no new minimum to determine landing
    int num_data_points_checked_4_landing_ = num_seconds_no_new_minimum_ * sampling_frequency_; // how many altitude points must a new min not be found for landing to be declared
    int z_threshold_for_landing_ = 175 * ft_2_m_;                                               // [m] threshold that the altitude must be within for landing
    int max_flight_time_ = 150;                                                                 // [s] max allowable flight time, if exceeded program ends
    int max_parachute_detach_wait_time_ = 2;                                                    // [s] maximum time to wait for the parachute detach signal to be returned from the Teensy before continuing
    int length_collect_rafco_ = 20;                                                             // TODO CHA4 * 60; // [s] amount of time to collect RAFCO signals and perform image processing

    // calibration parameters
    uint16_t num_sample_readings_ = 60; // amount of samples taken and averaged to find ground P and T
    int imu_wait_ = 60;                 // number of samples to get from IMU before actually starting to use + save data

    float p0_;
    float t0_;
    float g0_;

    double launch_time_;
    float z_current_ = 0;

    // IMU Connection and Configuration
    VnSensor *m_vn_;
    ImuMeasurementsRegister response_;
    std::string input_file_name_ = "packets_" + std::to_string(start_time_).substr(0, std::to_string(start_time_).find(".")) + ".txt";
    std::string flight_log_name_ = "flightDataLog";
    std::string program_log_name_ = "programDataLog";
    Log m_log_;
    bool is_imu_connected_ = false;
    
    PacketReceiver radio1 = PacketReceiver(1);
    PacketReceiver radio2 = PacketReceiver(2);
    int maxRadioWaitTime = 60 * 4; // [s] max wait time to receive packets

    Root();
    Root(bool is_unit_fsm);
    ~Root();

    void addState(State *the_state);
    void execute();
    double getCurrentTime();
    float angleToPulseWidth(double pulse_max, double pulse_min, double range, float angle);

    // given T0 [K], P0 [kPa], g0 [m/s^2], P [kPa], returns altitude above ground level
    // 0 indicates baseline measurement, R and B are constants
    float pressure2Altitude(float t0, float p0, float g0, float p);

    // given a float array, calculates the average of all the arrays values
    float calcArrayAverage(float array[], int size);

    // disconnects IMUf
    // returns true if IMU was active but was successfuly disconnected; returns false if IMU was not active
    bool terminateConnections(VnSensor *imu);

    // functional the same as sleep() command but still records and logs IMU data
    // sleep_time given in seconds
    void activeSleep(float sleep_time, VnSensor *imu, ImuMeasurementsRegister &response, Log &log, double &start_time);

    // checks if flight has gone on for too long, if so returns true to end program
    // launchTime given in milliseconds; triggerTime given in seconds
    bool isTimeExceeded(double launch_time, double trigger_time);

private:

};
#endif