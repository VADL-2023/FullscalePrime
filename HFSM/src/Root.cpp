#include "Root.h"
#include "State.h"

Root::Root():Root(false) {
}

Root::Root(bool is_unit_fsm):start_time_(0),is_unit_fsm_(is_unit_fsm) {
    // Temporary initialization thing
    while(gpioInitialise() <= 0) {
        std::cout << "GPIOs not initialized" << std::endl;
    }

    std::cout << "GPIOs initialized" << std::endl;

	// Stepper 1 Initialization
    this->stepper_1_.initialize(this->steps_per_revolution_,this->stepper_1_pin_1_,this->stepper_1_pin_2_,this->stepper_1_pin_3_,this->stepper_1_pin_4_,true);
    gpioSetMode(this->stepper_1_standby_pin_,PI_OUTPUT);
    gpioWrite(this->stepper_1_standby_pin_,0);
    this->stepper_1_.setSpeed(this->stepper_speed_);

    // Stepper 2 Initialization
    this->stepper_2_.initialize(this->steps_per_revolution_,this->stepper_2_pin_1_,this->stepper_2_pin_2_,this->stepper_2_pin_3_,this->stepper_2_pin_4_,true);
    gpioSetMode(this->stepper_2_standby_pin_,PI_OUTPUT);
    gpioWrite(this->stepper_2_standby_pin_,0);
    this->stepper_2_.setSpeed(this->stepper_speed_);

    // Stepper 3 Initialization
    this->stepper_3_.initialize(this->steps_per_revolution_,this->stepper_3_pin_1_,this->stepper_3_pin_2_,this->stepper_3_pin_3_,this->stepper_3_pin_4_,true);
    gpioSetMode(this->stepper_3_standby_pin_,PI_OUTPUT);
    gpioWrite(this->stepper_3_standby_pin_,0);
    this->stepper_3_.setSpeed(this->stepper_speed_);

    //RCB Motor Initialization
    gpioSetMode(this->rcb_p_,PI_OUTPUT);
	gpioSetMode(this->rcb_n_,PI_OUTPUT);
	gpioSetMode(this->rcb_enable_,PI_OUTPUT);
    gpioSetMode(this->rcb_lift_standby_,PI_OUTPUT);
    gpioWrite(this->rcb_lift_standby_,0);
    gpioPWM(this->rcb_enable_,0);

    //Nacelle Locking Servo Initializaiton
    gpioSetMode(this->nacelle_servo_,PI_OUTPUT);

    //Lift Locking Servo Initialization
    gpioSetMode(this->lift_servo_,PI_OUTPUT);

    //Lift Motor Initialization
    gpioSetMode(this->lift_p_,PI_OUTPUT);
	gpioSetMode(this->lift_n_,PI_OUTPUT);
	gpioSetMode(this->lift_enable_,PI_OUTPUT);
    gpioPWM(this->lift_enable_,0);

    //Level Servo Initialization
    gpioSetMode(this->level_servo_,PI_OUTPUT);

    radio1.setDeviceIndex(0);
    radio2.setDeviceIndex(1);
}

Root::~Root() {
    gpioTerminate();
	std::cout << "GPIOs terminated" << std::endl;
}

void Root::addState(State* the_state) {
    this->states_.insert(std::pair<StateName,State*>(the_state->getName(),the_state));
}

double Root::getCurrentTime() {
    return double(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
}

float Root::angleToPulseWidth(double pulse_max,double pulse_min,double range,float angle) {
    return (pulse_max - pulse_min) * angle / range + (pulse_max + pulse_min) / 2.0;
}