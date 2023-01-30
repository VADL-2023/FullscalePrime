#include "../include/Stepper.h"
#include <cmath>
#include <iostream>

Stepper::Stepper() {
    std::cout << "Stepper Not initialized yet so please don't call stepper stuff yet" << std::endl;
}
/*
 *   constructor for four-pin version
 *   Sets which wires should control the motor.
 */
Stepper::Stepper(int number_of_steps, int motor_pin_1, int motor_pin_2,
                 int motor_pin_3, int motor_pin_4, bool gpio_initialized = true)
{
    initialize(number_of_steps,motor_pin_1,motor_pin_2,motor_pin_3,motor_pin_4,gpio_initialized);
}

void Stepper::initialize(int number_of_steps, int motor_pin_1, int motor_pin_2,
                 int motor_pin_3, int motor_pin_4, bool gpio_initialized = true) {
    if(!gpio_initialized) {
        gpioInitialise();
    }
    this->step_number_ = 0;                   // which step the motor is on
    this->direction_ = 0;                     // motor direction
    this->last_step_time_ = 0;                // timestamp in us of the last step taken
    this->number_of_steps_ = number_of_steps; // total number of steps for this motor

    // Arduino pins for the motor control connection:
    this->motor_pin_1_ = motor_pin_1;
    this->motor_pin_2_ = motor_pin_2;
    this->motor_pin_3_ = motor_pin_3;
    this->motor_pin_4_ = motor_pin_4;

    // setup the pins on the microcontroller:
    gpioSetMode(this->motor_pin_1_,PI_OUTPUT);
    gpioSetMode(this->motor_pin_2_,PI_OUTPUT);
    gpioSetMode(this->motor_pin_3_,PI_OUTPUT);
    gpioSetMode(this->motor_pin_4_,PI_OUTPUT);

    // pin_count is used by the stepMotor() method:
    this->pin_count_ = 4;

}

double Stepper::getCurrentTime() {
    return double(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
}

/*
 * Sets the speed in revs per minute
 */
void Stepper::setSpeed(double whatSpeed)
{
    this->step_delay_ = 60 * 1000 * 1000 / this->number_of_steps_ / whatSpeed;
}

/*
 * Moves the motor steps_to_move steps.  If the number is negative,
 * the motor moves in the reverse direction.
 */
void Stepper::step(int steps_to_move)
{
    int steps_left = std::abs(steps_to_move); // how many steps to take

    // determine direction based on whether steps_to_mode is + or -:
    if (steps_to_move > 0)
    {
        this->direction_ = 1;
    }
    if (steps_to_move < 0)
    {
        this->direction_ = 0;
    }

    // decrement the number of steps, moving one step each time:
    while (steps_left > 0)
    {
        double now = getCurrentTime();
        //std::cout << "Time space: " << now - this->last_step_time_ << std::endl;
        //std::cout << "Step Delay: " << this->step_delay_ << std::endl;
        
        // move only if the appropriate delay has passed:
        if (now - this->last_step_time_ >= this->step_delay_)
        {
            //std::cout << "In if" <<std::endl;
            // get the timeStamp of when you stepped:
            this->last_step_time_ = now;
            // increment or decrement the step number,
            // depending on direction:
            if (this->direction_ == 1)
            {
                this->step_number_++;
                if (this->step_number_ == this->number_of_steps_)
                {
                    this->step_number_ = 0;
                }
            }
            else
            {
                if (this->step_number_ == 0)
                {
                    this->step_number_ = this->number_of_steps_;
                }
                this->step_number_--;
            }
            // decrement the steps left:
            steps_left--;
            // step the motor to step number 0, 1, ..., {3 or 10}
            if (this->pin_count_ == 5)
                stepMotor(this->step_number_ % 10);
            else
                stepMotor(this->step_number_ % 4);
        }
    }
}

/*
 * Moves the motor forward or backwards.
 */
void Stepper::stepMotor(int thisStep)
{
    if (this->pin_count_ == 2)
    {
        switch (thisStep)
        {
        case 0: // 01
            gpioWrite(motor_pin_1_, 0);
            gpioWrite(motor_pin_2_, 1);
            break;
        case 1: // 11
            gpioWrite(motor_pin_1_, 1);
            gpioWrite(motor_pin_2_, 1);
            break;
        case 2: // 10
            gpioWrite(motor_pin_1_, 1);
            gpioWrite(motor_pin_2_, 0);
            break;
        case 3: // 00
            gpioWrite(motor_pin_1_, 0);
            gpioWrite(motor_pin_2_, 0);
            break;
        }
    }
    if (this->pin_count_ == 4)
    {
        switch (thisStep)
        {
        case 0: // 1010
            gpioWrite(motor_pin_1_, 1);
            gpioWrite(motor_pin_2_, 0);
            gpioWrite(motor_pin_3_, 1);
            gpioWrite(motor_pin_4_, 0);
            break;
        case 1: // 0110
            gpioWrite(motor_pin_1_, 0);
            gpioWrite(motor_pin_2_, 1);
            gpioWrite(motor_pin_3_, 1);
            gpioWrite(motor_pin_4_, 0);
            break;
        case 2: // 0101
            gpioWrite(motor_pin_1_, 0);
            gpioWrite(motor_pin_2_, 1);
            gpioWrite(motor_pin_3_, 0);
            gpioWrite(motor_pin_4_, 1);
            break;
        case 3: // 1001
            gpioWrite(motor_pin_1_, 1);
            gpioWrite(motor_pin_2_, 0);
            gpioWrite(motor_pin_3_, 0);
            gpioWrite(motor_pin_4_, 1);
            break;
        }
    }

    if (this->pin_count_ == 5)
    {
        switch (thisStep)
        {
        case 0: // 01101
            gpioWrite(motor_pin_1_, 0);
            gpioWrite(motor_pin_2_, 1);
            gpioWrite(motor_pin_3_, 1);
            gpioWrite(motor_pin_4_, 0);
            gpioWrite(motor_pin_5_, 1);
            break;
        case 1: // 01001
            gpioWrite(motor_pin_1_, 0);
            gpioWrite(motor_pin_2_, 1);
            gpioWrite(motor_pin_3_, 0);
            gpioWrite(motor_pin_4_, 0);
            gpioWrite(motor_pin_5_, 1);
            break;
        case 2: // 01011
            gpioWrite(motor_pin_1_, 0);
            gpioWrite(motor_pin_2_, 1);
            gpioWrite(motor_pin_3_, 0);
            gpioWrite(motor_pin_4_, 1);
            gpioWrite(motor_pin_5_, 1);
            break;
        case 3: // 01010
            gpioWrite(motor_pin_1_, 0);
            gpioWrite(motor_pin_2_, 1);
            gpioWrite(motor_pin_3_, 0);
            gpioWrite(motor_pin_4_, 1);
            gpioWrite(motor_pin_5_, 0);
            break;
        case 4: // 11010
            gpioWrite(motor_pin_1_, 1);
            gpioWrite(motor_pin_2_, 1);
            gpioWrite(motor_pin_3_, 0);
            gpioWrite(motor_pin_4_, 1);
            gpioWrite(motor_pin_5_, 0);
            break;
        case 5: // 10010
            gpioWrite(motor_pin_1_, 1);
            gpioWrite(motor_pin_2_, 0);
            gpioWrite(motor_pin_3_, 0);
            gpioWrite(motor_pin_4_, 1);
            gpioWrite(motor_pin_5_, 0);
            break;
        case 6: // 10110
            gpioWrite(motor_pin_1_, 1);
            gpioWrite(motor_pin_2_, 0);
            gpioWrite(motor_pin_3_, 1);
            gpioWrite(motor_pin_4_, 1);
            gpioWrite(motor_pin_5_, 0);
            break;
        case 7: // 10100
            gpioWrite(motor_pin_1_, 1);
            gpioWrite(motor_pin_2_, 0);
            gpioWrite(motor_pin_3_, 1);
            gpioWrite(motor_pin_4_, 0);
            gpioWrite(motor_pin_5_, 0);
            break;
        case 8: // 10101
            gpioWrite(motor_pin_1_, 1);
            gpioWrite(motor_pin_2_, 0);
            gpioWrite(motor_pin_3_, 1);
            gpioWrite(motor_pin_4_, 0);
            gpioWrite(motor_pin_5_, 1);
            break;
        case 9: // 00101
            gpioWrite(motor_pin_1_, 0);
            gpioWrite(motor_pin_2_, 0);
            gpioWrite(motor_pin_3_, 1);
            gpioWrite(motor_pin_4_, 0);
            gpioWrite(motor_pin_5_, 1);
            break;
        }
    }
}

/*
  version() returns the version of the library:
*/
int Stepper::version(void)
{
    return 5;
}