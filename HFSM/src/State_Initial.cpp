#include "State_Initial.h"
#include "Root.h"

State_Initial::State_Initial(): State() {

}

State_Initial::State_Initial(StateName name, std::map<EventName, StateName> &state_transitions, Root* root) : State(name, state_transitions, root) {

}

EventName State_Initial::execute() {
	// Temporary initialization thing
    while(gpioInitialise() <= 0) {
        std::cout << "Not initialized" << std::endl;
    }
    std::cout << "Initialized" << std::endl;
	// Stepper 1 Initialization
    this->root_->stepper_1_.initialize(this->root_->steps_per_revolution_,this->root_->stepper_1_pin_1_,this->root_->stepper_1_pin_2_,this->root_->stepper_1_pin_3_,this->root_->stepper_1_pin_4_,true);
    gpioSetMode(this->root_->stepper_1_standby_pin_,PI_OUTPUT);
    gpioWrite(this->root_->stepper_1_standby_pin_,0);
    this->root_->stepper_1_.setSpeed(this->root_->stepper_speed_);

    // Stepper 2 Initialization
    this->root_->stepper_2_.initialize(this->root_->steps_per_revolution_,this->root_->stepper_2_pin_1_,this->root_->stepper_2_pin_2_,this->root_->stepper_2_pin_3_,this->root_->stepper_2_pin_4_,true);
    gpioSetMode(this->root_->stepper_2_standby_pin_,PI_OUTPUT);
    gpioWrite(this->root_->stepper_2_standby_pin_,0);
    this->root_->stepper_2_.setSpeed(this->root_->stepper_speed_);

    // Stepper 3 Initialization
    this->root_->stepper_3_.initialize(this->root_->steps_per_revolution_,this->root_->stepper_3_pin_1_,this->root_->stepper_3_pin_2_,this->root_->stepper_3_pin_3_,this->root_->stepper_3_pin_4_,true);
    gpioSetMode(this->root_->stepper_3_standby_pin_,PI_OUTPUT);
    gpioWrite(this->root_->stepper_3_standby_pin_,0);
    this->root_->stepper_3_.setSpeed(this->root_->stepper_speed_);

    //RCB Motor Initialization
    gpioSetMode(this->root_->rcb_p_,PI_OUTPUT);
	gpioSetMode(this->root_->rcb_n_,PI_OUTPUT);
	gpioSetMode(this->root_->rcb_enable_,PI_OUTPUT);
    gpioSetMode(this->root_->rcb_lift_standby_,PI_OUTPUT);
    gpioWrite(this->root_->rcb_lift_standby_,0);
    gpioPWM(this->root_->rcb_enable_,0);

    //Nacelle Locking Servo Initializaiton
    gpioSetMode(this->root_->nacelle_servo_,PI_OUTPUT);

    //Lift Locking Servo Initialization
    gpioSetMode(this->root_->lift_servo_,PI_OUTPUT);

    //Lift Motor Initialization
    gpioSetMode(this->root_->lift_p_,PI_OUTPUT);
	gpioSetMode(this->root_->lift_n_,PI_OUTPUT);
	gpioSetMode(this->root_->lift_enable_,PI_OUTPUT);
    gpioPWM(this->root_->lift_enable_,0);

    //Level Servo Initialization
    gpioSetMode(this->root_->level_servo_,PI_OUTPUT);

	return INITIALIZE;
}

// We may not need this, the default for unitExecute is that we call execute
// So hopefully inheritance covers this?
EventName State_Initial::unitExecute() {
    return execute();
}