#include "pigpio.h"
#include <ctime>
#include <chrono>

// ensure this library description is only included once
#ifndef Stepper_h
#define Stepper_h

// library interface description
class Stepper {
  public:
    // constructors:
    Stepper();
    Stepper(int number_of_steps, int motor_pin_1, int motor_pin_2,
                                 int motor_pin_3, int motor_pin_4,
                                 bool gpio_initialized);

    void initialize(int number_of_steps, int motor_pin_1, int motor_pin_2,
                 int motor_pin_3, int motor_pin_4, bool gpio_initialized);
    double getCurrentTime();

    // speed setter method:
    void setSpeed(double whatSpeed);

    // mover method:
    void step(int number_of_steps);

    int version(void);

  private:
    void stepMotor(int this_step);

    int direction_;            // Direction of rotation
    double step_delay_; // delay between steps, in us, based on speed
    int number_of_steps_;      // total number of steps this motor can take
    int pin_count_;            // how many pins are in use.
    int step_number_;          // which step the motor is on

    // motor pin numbers:
    int motor_pin_1_;
    int motor_pin_2_;
    int motor_pin_3_;
    int motor_pin_4_;
    int motor_pin_5_;          // Only 5 phase motor

    double last_step_time_; // timestamp in us of when the last step was taken
};

#endif
