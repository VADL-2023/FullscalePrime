/**
 * @file Stepper.h
 * @brief Stepper Library header file
 * 
 */
#include "pigpio.h"
#include <ctime>
#include <chrono>

// ensure this library description is only included once
#ifndef Stepper_h
#define Stepper_h

// library interface description
/**
 * @class Stepper
 * @brief Stepper Library Interface
 * 
 */
class Stepper {
  public:

    /**
     * @brief Construct a new Stepper object
     * 
     */
    Stepper();

    /**
     * @brief Construct a new Stepper object
     * 
     * @param number_of_steps Number of steps per revolution
     * @param motor_pin_1 GPIO Pin 1
     * @param motor_pin_2 GPIO Pin 2
     * @param motor_pin_3 GPIO Pin 3
     * @param motor_pin_4 GPIO Pin 4
     * @param gpio_initialized Boolean indicating if GPIO has been initialized yet
     */
    Stepper(int number_of_steps, int motor_pin_1, int motor_pin_2,
                                 int motor_pin_3, int motor_pin_4,
                                 bool gpio_initialized);

    /**
     * @brief Initializes Stepper motor
     * 
     * @param number_of_steps Number of steps per revolution
     * @param motor_pin_1 GPIO Pin 1
     * @param motor_pin_2 GPIO Pin 2
     * @param motor_pin_3 GPIO Pin 3
     * @param motor_pin_4 GPIO Pin 4
     * @param gpio_initialized Boolean indicating if GPIO has been initialized yet
     */
    void initialize(int number_of_steps, int motor_pin_1, int motor_pin_2,
                 int motor_pin_3, int motor_pin_4, bool gpio_initialized);

    /**
     * @brief Get the current time in ms
     * 
     * @return double representing the time in ms
     */
    double getCurrentTime();

    /**
     * @brief Set the stepper motor speed
     * 
     * @param whatSpeed Stepper speed
     */
    void setSpeed(double whatSpeed);

    /**
     * @brief Perform step for stepper
     * 
     * @param number_of_steps The number of steps the motor must rotate
     */
    void step(int number_of_steps);

    /**
     * @brief Returns stepper library version
     * 
     * @return int representing library version
     */
    int version(void);

  private:

    /**
     * @brief Individual motor step
     * 
     * @param this_step The individual step
     */
    void stepMotor(int this_step);

    /**
     * @brief Direction of rotation
     * 
     */
    int direction_;           

    /**
     * @brief Delay between steps in us based on speed
     * 
     */
    double step_delay_; 

    /**
     * @brief Total number of steps the motor can rotate
     * 
     */
    int number_of_steps_;     

    /**
     * @brief Number of GPIO pins used for stepper motor
     * 
     */
    int pin_count_;          

    /**
     * @brief Which step the motor is on
     * 
     */
    int step_number_;        

    /**
     * @brief GPIO Pin 1
     * 
     */
    int motor_pin_1_;

    /**
     * @brief GPIO Pin 2
     * 
     */
    int motor_pin_2_;

    /**
     * @brief GPIO Pin 3
     * 
     */
    int motor_pin_3_;

    /**
     * @brief GPIO Pin 4
     * 
     */
    int motor_pin_4_;

    /**
     * @brief GPIO Pin 5 (only for 5 phase motor)
     * 
     */
    int motor_pin_5_;      

    /**
     * @brief Timestamp in us of when last step was taken
     * 
     */
    double last_step_time_; 
};

#endif
