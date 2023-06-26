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
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

class State;

 /**
 * @file Root.h
 * @class Root
 * @brief Contains all common variables and functions accessible to every state in the state machine
 * 
 */
class Root
{
public:

    /**
     * @brief Boolean indicating if GPIO pins have been initialized
     * 
     */
    bool gpio_initialized_ = false;

    /**
     * @brief Start time of program in milliseconds (arbitrary number)
     * 
     */
    double start_time_;

    /**
     * @brief Boolean indicating if it is in display mode (normally false, and should probably delete this)
     * 
     */
    bool is_unit_fsm_;

    /**
     * @brief Delay between each motor/servo movement in display state machine
     * 
     */
    int unit_test_delay_ms_ = 1000000;

    /**
     * @brief Time and date of when program starts
     * 
     */
    std::string date_timestamp_ = "";

    /**
     * @brief Number of steps required to fully rotate stepper motor in nacelle
     * 
     */
    int steps_per_revolution_ = 200;

    /**
     * @brief Speed of stepper motor in nacelle
     * 
     */
    int stepper_speed_ = 10;

    /**
     * @brief Number of steps to rotate stepper motor 60 degrees
     * 
     */
    int num_steps_ = 40;

    /**
     * @brief Time threshold for how long stepper should run in display mode
     * 
     */
    int stepper_threshold_ = 5000;

    /**
     * @brief PWM Signal to move motor at max speed
     * 
     */
    int pwm_motor_max_ = 255;

    /**
     * @brief RPi GPIO Pin for stepper 1 standby
     * 
     */
    int stepper_1_standby_pin_ = 0;

    /**
     * @brief RPi GPIO Pin for stepper 1 pin 1
     * 
     */
    int stepper_1_pin_1_ = 11;

    /**
     * @brief RPi GPIO Pin for stepper 1 pin 2
     * 
     */
    int stepper_1_pin_2_ = 9;

    /**
     * @brief RPi GPIO Pin for stepper 1 pin 3
     * 
     */
    int stepper_1_pin_3_ = 5;

    /**
     * @brief RPi GPIO Pin for stepper 1 pin 4
     * 
     */
    int stepper_1_pin_4_ = 6;

    /**
     * @brief RPi GPIO Pin for stepper 2 standby
     * 
     */
    int stepper_2_standby_pin_ = 27;

    /**
     * @brief RPi GPIO Pin for stepper 2 pin 1
     * 
     */
    int stepper_2_pin_1_ = 17;

    /**
     * @brief RPi GPIO Pin for stepper 2 pin 2
     * 
     */
    int stepper_2_pin_2_ = 4;

    /**
     * @brief RPi GPIO Pin for stepper 2 pin 3
     * 
     */
    int stepper_2_pin_3_ = 22;

    /**
     * @brief RPi GPIO Pin for stepper 2 pin 4
     * 
     */
    int stepper_2_pin_4_ = 10;

    /**
     * @brief List of strings where each string represents functional camera stream
     * 
     */
    std::vector<std::string> aac_camera_streams_;

    /**
     * @brief List of VideoCapture objects representing camera streams used during mission
     * 
     */
    std::vector<cv::VideoCapture> aac_camera_captures_;

    /**
     * @brief List of strings where each string represents a camera stream during mission (we are not using all functional camera streams from aac_camera_streams_, only a subset)
     * 
     */
    std::vector<std::string> aac_camera_captures_strings_;
    //--- INITIALIZE VIDEOCAPTURE

    /**
     * @brief VideoCapture object for cam 1
     * 
     */
    cv::VideoCapture cap1;

    /**
     * @brief VideoCapture object for cam 2
     * 
     */
    cv::VideoCapture cap2;

    /**
     * @brief VideoCapture object for cam 3
     * 
     */
    cv::VideoCapture cap3;

    /**
     * @brief Boolean indicating if launch has been detected yet
     * 
     */
    bool launch_detected_ = false;

    /**
     * @brief Boolean indicating if apogee has been detected yet
     * 
     */
    bool apogee_detected_ = false;

    /**
     * @brief Boolean indicating if landing has been detected yet
     * 
     */
    bool landing_detected_ = false;

    /**
     * @brief Number of pictures taken by camera 1 during flight
     * 
     */
    int aac_pic_num_cam_1_ = 1;

    /**
     * @brief Number of pictures taken by camera 2 during flight
     * 
     */
    int aac_pic_num_cam_2_ = 1;

    /**
     * @brief Number of pictures taken by camera 3 during flight
     * 
     */
    int aac_pic_num_cam_3_ = 1;

    /**
     * @brief RPi GPIO pin for nacelle stepper 3 standby
     * 
     */
    int stepper_3_standby_pin_ = 8;

    /**
     * @brief RPi GPIO pin for nacelle stepper 3 pin 1
     * 
     */
    int stepper_3_pin_1_ = 7;

    /**
     * @brief RPi GPIO pin for nacelle stepper 3 pin 2
     * 
     */
    int stepper_3_pin_2_ = 1;

    /**
     * @brief RPi GPIO pin for nacelle stepper 3 pin 3
     * 
     */
    int stepper_3_pin_3_ = 25;

    /**
     * @brief RPi GPIO pin for nacelle stepper 3 pin 4
     * 
     */
    int stepper_3_pin_4_ = 24;

    /**
     * @brief Stepper controller object for nacelle stepper 1
     * 
     */
    Stepper stepper_1_;

    /**
     * @brief Stepper controller object for nacelle stepper 2
     * 
     */
    Stepper stepper_2_;

    /**
     * @brief Stepper controller object for nacelle stepper 3
     * 
     */
    Stepper stepper_3_;

    /**
     * @brief RPi GPIO Pin for positive RCB motor terminal
     * 
     */
    int rcb_p_ = 26;

    /**
     * @brief RPi GPIO Pin for negative RCB motor terminal
     * 
     */
    int rcb_n_ = 19;

    /**
     * @brief RPi GPIO Pin for enable RCB motor
     * 
     */
    int rcb_enable_ = 13;

    /**
     * @brief RPi GPIO Pin for standby pin for RCB and Lift Motor Controller
     * 
     */
    int rcb_lift_standby_ = 21;

    /**
     * @brief Time threshold for RCB to rotate in display mode (Note: Display mode will just tick each motor in order and has no perception)
     * 
     */
    int rcb_time_threshold_ = 5000;

    /**
     * @brief Error threshold for RCB motor error (+/- rcb_angle_threshold_ meaning total error is twice as big)
     * 
     */
    double rcb_angle_threshold_ = 1;

    /**
     * @brief Boolean indicating if RCB is finished (aligned)
     * 
     */
    bool is_aligned_ = false;

    /**
     * @brief Maximum time to run RCB during payload before transitioning to next state
     * 
     */
    int full_rcb_time_threshold_ = 75000;

    /**
     * @brief Time in milliseconds of landing
     * 
     */
    double landing_time_;

    /**
     * @brief Number of leading zeroes in AAC saved photos filename
     * 
     */
    size_t n_photo_bit_size_ = 5;

    /**
     * @brief RPi GPIO pin for nacelle locking servo
     * 
     */
    int nacelle_servo_ = 14;

    /**
     * @brief Minimum servo pulse width (only for display mode, which doesn't work at the moment)
     * 
     */
    uint16_t servo_pulse_min_ = 500;

    /**
     * @brief Maximum servo pulse width (only display mode)
     * 
     */
    uint16_t servo_pulse_max_ = 2250;

    /**
     * @brief Servo angle range
     * 
     */
    uint8_t servo_deg_range_ = 90;

    /**
     * @brief Servo pulse width to lock nacelles
     * 
     */
    int nacelle_lock_ = 1375;

    /**
     * @brief Servo pulse width to unlock nacelles
     * 
     */
    int nacelle_unlock_ = 755;

    /**
     * @brief Boolean indicating if the lift state is done
     * 
     */
    bool lift_done_ = false;

    /**
     * @brief RPi GPIO pin for lift servo
     * 
     */
    int lift_servo_ = 2;

    /**
     * @brief RPi GPIO pin for top limit switch for lift
     * 
     */
    int lift_final_limit_switch_ = 23;

    /**
     * @brief Time the lift motor will run in display mode
     * 
     */
    int lift_unit_time_threshold_ = 2000;

    /**
     * @brief Pulse width to lock the lift
     * 
     */
    int lift_lock_ = 1500;

    /**
     * @brief Pulse width to unlock the lift
     * 
     */
    int lift_unlock_ = 1150;

    /**
     * @brief RPi GPIO Pin for positive lift motor terminal
     * 
     */
    int lift_p_ = 20;

    /**
     * @brief RPi GPIO Pin for negative lift motor terminal
     * 
     */
    int lift_n_ = 16;

    /**
     * @brief RPi GPIO Pin for the Lift motor enable
     * 
     */
    int lift_enable_ = 12;

    /**
     * @brief Maximum amount of time (ms) to run lift before transitioning to next state
     * 
     */
    int lift_time_threshold_ = 30000;

    /**
     * @brief Amount of time to move the lift back down to prevent wire tension
     * 
     */
    int lift_backwards_time_threshold_ = 1068;

    /**
     * @brief Minimum amount of time (ms) to run lift
     * 
     */
    int lift_min_threshold_ = 15000;

    /**
     * @brief RPi GPIO Pin for level servo
     * 
     */
    int level_servo_ = 3;

    /**
     * @brief Number of IMU measurements taken to determine pitch error for level
     * 
     */
    int num_level_samples_ = 20;

    /**
     * @brief Ground truth angle for pitch level
     * 
     */
    double ideal_level_angle_ = 180;

    /**
     * @brief Maximum angle the level can tilt up
     * 
     */
    int min_angle_ = -10;

    /**
     * @brief Corresponding pulse width to tilt level up at the maximum angle
     * 
     */
    int min_pulse_width_ = 1200;

    /**
     * @brief Maximum angle the level can tilt down
     * 
     */
    int max_angle_ = 20;

    /**
     * @brief Corresponding pulse width to tilt level down at the maximum angle
     * 
     */
    int max_pulse_width_ = 1950;

    /**
     * @brief Number of RAFCO pictures taken (used for photo saving)
     * 
     */
    int rafco_pic_num_ = 1;

    /**
     * @brief Map every state name to a state
     * 
     */
    std::map<StateName, State *> states_;

    /**
     * @brief Threads for flight photos
     * 
     */
    std::vector<std::thread> threads_;

    /**
     * @brief Feet to meters conversion factor
     * 
     */
    float ft_2_m_ = 0.3048; 

    /**
     * @brief Meters to feet conversion factor
     * 
     */
    float m_2_ft_ = 1 / ft_2_m_;

    /**
     * @brief Celsius to Kelvin conversion factor
     * 
     */
    float c_2_k_ = 273.15;

    /**
     * @brief Kilometers to meters conversion factor
     * 
     */
    float km_2_m_ = 0.001;

    /**
     * @brief Universal gas constant (kg/JK)
     * 
     */
    float r_ = 287;

    /**
     * @brief Variation of temperature within the troposphere (K/m)
     * 
     */
    float b_ = 6.5 * km_2_m_;

    /**
     * @brief Motor burn time (s)
     * 
     */
    float t_burn_ = 1.9;            

    /**
     * @brief Speed the IMU samples data (Hz)
     * 
     */
    float sampling_frequency_ = 20;

    /**
     * @brief Tells program if we NO-GOed
     * 
     */
    bool restart_ = false;          

    /**
     * @brief Flight relies on time thresholds if true rather than IMU data
     * 
     */
    bool time_delay_enabled_ = false;

    /**
     * @brief Flight video frames per second
     * 
     */
    int fps_ = 30;

    /**
     * @brief Flight video frame width
     * 
     */
    int frame_width_ = 640;

    /**
     * @brief Flight video frame height
     * 
     */
    int frame_height_ = 480;

    /**
     * @brief Maximum flight recording time
     * 
     */
    int max_proper_flight_time_ = 3 * 60;

    /**
     * @brief Time of individual video clip recording (individual 3 second videos are written out and saved)
     * 
     */
    int vid_clip_time_ = 3;

    /**
     * @brief Numnber of frames per individual 3 second video
     * 
     */
    int frames_per_vid_ = fps_ * vid_clip_time_;

    /**
     * @brief List of all individual videos from camera 1
     * 
     */
    std::vector<cv::VideoWriter> videos1_;

    /**
     * @brief List of all individual videos from camera 2
     * 
     */
    std::vector<cv::VideoWriter> videos2_;

    /**
     * @brief List of all individual videos from camera 3
     * 
     */
    std::vector<cv::VideoWriter> videos3_;

    /**
     * @brief Videocapture object for lift and level video
     * 
     */
    cv::VideoCapture lift_and_level_cap_;

    /**
     * @brief Videowriter object for lift and level video
     * 
     */
	cv::VideoWriter lift_and_level_video_;

    /**
     * @brief Thread to record lift video
     * 
     */
    std::thread lift_thread_;

    // TODO: double check these flight parameters
    // possibly variable flight parameters (stuff we might change)
    /**
     * @brief Acceleration to detect launch (g's)
     * 
     */
    float accel_roof_ = 3.5;
    
    /**
     * @brief how many acceleration points are averaged to see if data set is over accel_roof_
     * 
     */
    int num_data_points_checked_4_launch_ = 8;

    /**
     * @brief how many altitude points must a new max not be found for apogee to be declared
     * 
     */
    int num_data_points_checked_4_apogee_ = 10;

    /**
     * @brief [s] number of seconds to wait for no new minimum to determine landing
     * 
     */
    int num_seconds_no_new_minimum_ = 10;

    /**
     * @brief how many altitude points must a new min not be found for landing to be declared
     * 
     */
    int num_data_points_checked_4_landing_ = num_seconds_no_new_minimum_ * sampling_frequency_;

    /**
     * @brief [m] threshold that the altitude must be within for landing
     * 
     */
    int z_threshold_for_landing_ = 175 * ft_2_m_;   

    /**
     * @brief [s] max allowable flight time, if exceeded program ends (accounts for if we switched drogue amd main)
     * 
     */
    int max_flight_time_ = 10 * 60;

    /**
     * @brief [s] amount of time to collect RAFCO signals and perform image processing
     * 
     */
    int length_collect_rafco_ = 20 * 60;                      

    /**
     * @brief Frequency for RAFCO transmissions
     * 
     */
    std::string rafco_freq_ = "144.900M";   

    /**
     * @brief Callsign to look for
     * 
     */
    std::string callsign_ = "KQ4CTL";      

    /**
     * @brief Service Set Identifier, probably used for TCP Packet Receiver, Ask Meredith Hunter
     * 
     */
    int source_ssid_ = 6;

    /**
     * @brief [ms] aditional time to wait for PDS
     * 
     */
    int pds_delay_ = 60 * 1000;

    /**
     * @brief Boolean that is true if we had to use backup cameras, but we want to try to redo RAFCO for the primary camera
     * 
     */
    bool rafco_redo_ = false;

    /**
     * @brief amount of samples taken and averaged to find ground P and T
     * 
     */
    uint16_t num_sample_readings_ = 60; 

    /**
     * @brief number of samples to get from IMU before actually starting to use + save data
     * 
     */
    int imu_wait_ = 60;                 

    /**
     * @brief Baseline pressure reading
     * 
     */
    float p0_;

    /**
     * @brief Baseline temperature reading
     * 
     */
    float t0_;

    /**
     * @brief Baseline gravity reading
     * 
     */
    float g0_;

    /**
     * @brief Time of launch in ms
     * 
     */
    double launch_time_;

    /**
     * @brief Current altitude
     * 
     */
    float z_current_ = 0;

    /**
     * @brief VN Sensor Object
     * 
     */
    VnSensor *m_vn_;

    /**
     * @brief IMU Register storing the data
     * 
     */
    ImuMeasurementsRegister response_;

    /**
     * @brief String with the name for APRS Packet filename
     * 
     */
    std::string input_file_name_ = "packets_" + std::to_string(start_time_).substr(0, std::to_string(start_time_).find(".")) + ".txt";

    /**
     * @brief String with name for flight data log
     * 
     */
    std::string flight_log_name_ = "flightDataLog";

    /**
     * @brief String with name for program data log
     * 
     */
    std::string program_log_name_ = "programDataLog";

    /**
     * @brief Log object for flight data
     * 
     */
    Log m_log_;

    /**
     * @brief Boolean indicating if the IMU is still connected
     * 
     */
    bool is_imu_connected_ = false;

    /**
     * @brief String for first SDR configuration file
     * 
     */
    std::string config1_ = "/home/vadl/FullscalePrime/sdr1.conf";

    /**
     * @brief String for second SDR configuration file
     * 
     */
    std::string config2_ = "/home/vadl/FullscalePrime/sdr2.conf";

    /**
     * @brief Packet Receiver object for first SDR
     * 
     */
    PacketReceiver radio1_ = PacketReceiver(100, rafco_freq_, 8001, config1_);

    /**
     * @brief Packet Receiver object for second SDR
     * 
     */
    PacketReceiver radio2_ = PacketReceiver(200, rafco_freq_, 9001, config2_);

    /**
     * @brief List of all possible camera streams
     * 
     */
    std::vector<std::string> camera_streams_;

    /**
     * @brief String indicating the primary camera stream
     * 
     */
    std::string primary_camera_stream_;

    /**
     * @brief Boolean indicating if the RCB state is done
     * 
     */
    bool rcb_done_ = false;

    /**
     * @brief Construct a new Root object
     * 
     */
    Root();

    /**
     * @brief Construct a new Root object
     * 
     * @param is_unit_fsm Boolean indicating if it is in display mode (normally false)
     */
    Root(bool is_unit_fsm);

    /**
     * @brief Destroy the Root object
     * 
     */
    ~Root();

    /**
     * @brief Will add a state to the state machine
     * 
     * @param the_state The state being added to the state machine
     */
    void addState(State *the_state);

    /**
     * @brief Get the current time in ms
     * 
     * @return double representing the current time
     */
    double getCurrentTime();

    /**
     * @brief Converts servo angle to pulse width through linear interpolation
     * 
     * @param pulse_max Maximum pulse width
     * @param pulse_min Minimum pulse width
     * @param range Angle range of the servo
     * @param angle Desired angle to move the servo
     * @return float representing the appropriate pulse width
     */
    float angleToPulseWidth(double pulse_max, double pulse_min, double range, float angle);

    /**
     * @brief Finds altitude based on pressure
     * 
     * @param t0 Baseline temperature in Kelvin
     * @param p0 Baseline pressure in kPa
     * @param g0 Baseline gravity in m/s^2
     * @param p Current pressure in kPa
     * @return float representing the current altitude above ground level in m
     */
    float pressure2Altitude(float t0, float p0, float g0, float p);

    // given a float array, calculates the average of all the arrays values
    /**
     * @brief Calculates average of all array values
     * 
     * @param array Given float array for calculation
     * @param size Number of elements in the float array
     * @return float representing the average array
     */
    float calcArrayAverage(float array[], int size);

    // disconnects IMUf
    // returns true if IMU was active but was successfuly disconnected; returns false if IMU was not active
    /**
     * @brief Disconnects IMU
     * 
     * @param imu VN sensor to be disconnected
     * @return true IMU was successfully disconnected
     * @return false IMU was not successfully disconnected
     */
    bool terminateConnections(VnSensor *imu);

    // functional the same as sleep() command but still records and logs IMU data
    // sleep_time given in seconds
    /**
     * @brief Similar to sleep() command but still records and logs IMU data
     * 
     * @param sleep_time Amount of time to sleep for
     * @param imu VN-100 sensor
     * @param response Register containing IMU data
     * @param log Log output file
     */
    void activeSleep(float sleep_time, VnSensor *imu, ImuMeasurementsRegister &response, Log &log);

    // checks if flight has gone on for too long, if so returns true to end program
    // launchTime given in milliseconds; triggerTime given in seconds
    /**
     * @brief Checks if flight has gone on for too long
     * 
     * @param launch_time Time of launch in ms
     * @param trigger_time Time threshold until the time has been exceeded
     * @return true Flight has gone on too long
     * @return false Flight is under time threshold
     */
    bool isTimeExceeded(double launch_time, double trigger_time);

    //check if a given camera stream is operational
    /**
     * @brief Checks if given camera stream is operational
     * 
     * @param camera_check Camera stream to check
     * @return true Camera stream is functional
     * @return false Camera stream is not functional
     */
    bool cameraCheck(std::string camera_check);

    /**
     * @brief Image capture thread for flight
     * 
     * @param cap VideoCapture object
     * @param videos List of videos that will be written out during flight
     * @param cam_number Camera stream number
     */
    void realCamThreadLanding(cv::VideoCapture *cap, std::vector<cv::VideoWriter> *videos, int cam_number);

    /**
     * @brief Image capture thread for RCB
     * 
     * @param cap VideoCapture object
     * @param video RCB output video
     */
    void realCamThreadRCB(cv::VideoCapture *cap, cv::VideoWriter *video);

    /**
     * @brief Image capture thread for Lift
     * 
     * @param cap VideoCapture object
     * @param video Lift output video
     */
    void realCamThreadLift(cv::VideoCapture *cap, cv::VideoWriter *video);

private:
};
#endif
