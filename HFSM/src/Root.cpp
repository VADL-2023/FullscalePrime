#include "Root.h"
#include "State.h"

Root::Root() : Root(false)
{
}

Root::Root(bool is_unit_fsm) : start_time_(0), is_unit_fsm_(is_unit_fsm), m_log_(flight_log_name_, program_log_name_, m_vn_, start_time_)
{
    start_time_ = this->getCurrentTime();
    // Temporary initialization thing
    while (gpioInitialise() <= 0)
    {
        m_log_.write("GPIOS not initialized");
    }
    m_log_.write("GPIOS initialized");

    // Stepper 1 Initialization
    this->stepper_1_.initialize(this->steps_per_revolution_, this->stepper_1_pin_1_, this->stepper_1_pin_2_, this->stepper_1_pin_3_, this->stepper_1_pin_4_, true);
    gpioSetMode(this->stepper_1_standby_pin_, PI_OUTPUT);
    gpioWrite(this->stepper_1_standby_pin_, 0);
    this->stepper_1_.setSpeed(this->stepper_speed_);

    // Stepper 2 Initialization
    this->stepper_2_.initialize(this->steps_per_revolution_, this->stepper_2_pin_1_, this->stepper_2_pin_2_, this->stepper_2_pin_3_, this->stepper_2_pin_4_, true);
    gpioSetMode(this->stepper_2_standby_pin_, PI_OUTPUT);
    gpioWrite(this->stepper_2_standby_pin_, 0);
    this->stepper_2_.setSpeed(this->stepper_speed_);

    // Stepper 3 Initialization
    this->stepper_3_.initialize(this->steps_per_revolution_, this->stepper_3_pin_1_, this->stepper_3_pin_2_, this->stepper_3_pin_3_, this->stepper_3_pin_4_, true);
    gpioSetMode(this->stepper_3_standby_pin_, PI_OUTPUT);
    gpioWrite(this->stepper_3_standby_pin_, 0);
    this->stepper_3_.setSpeed(this->stepper_speed_);

    // RCB Motor Initialization
    gpioSetMode(this->rcb_p_, PI_OUTPUT);
    gpioSetMode(this->rcb_n_, PI_OUTPUT);
    gpioSetMode(this->rcb_enable_, PI_OUTPUT);
    gpioSetMode(this->rcb_lift_standby_, PI_OUTPUT);
    gpioWrite(this->rcb_lift_standby_, 0);
    gpioPWM(this->rcb_enable_, 0);

    // Nacelle Locking Servo Initializaiton
    gpioSetMode(this->nacelle_servo_, PI_OUTPUT);

    // Lift Locking Servo Initialization
    gpioSetMode(this->lift_servo_, PI_OUTPUT);

    // Lift Limit Switch Initialization
    gpioSetMode(this->lift_base_limit_switch_, PI_INPUT);
    gpioSetMode(this->lift_final_limit_switch_, PI_INPUT);

    // Lift Motor Initialization
    gpioSetMode(this->lift_p_, PI_OUTPUT);
    gpioSetMode(this->lift_n_, PI_OUTPUT);
    gpioSetMode(this->lift_enable_, PI_OUTPUT);
    gpioPWM(this->lift_enable_, 0);

    // Level Servo Initialization
    gpioSetMode(this->level_servo_, PI_OUTPUT);

    //Instantiate camera streams
    this->camera_streams_.push_back("/dev/videoLaserbeak");
    this->camera_streams_.push_back("/dev/videoRumble");
    this->camera_streams_.push_back("/dev/videoRavage");
}

Root::~Root()
{
    gpioTerminate();
    this->m_log_.write("GPIOs terminated");
    if (this->is_imu_connected_)
    {
        if (this->terminateConnections(this->m_vn_))
        {
            this->m_log_.writeDelim("IMU: Disconnected");
        }
        delete this->m_vn_;
        this->is_imu_connected_ = false;
    }
    this->m_log_.writeDelim("Program ending");
}

void Root::addState(State *the_state)
{
    this->states_.insert(std::pair<StateName, State *>(the_state->getName(), the_state));
}

double Root::getCurrentTime()
{
    return double(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
}

float Root::angleToPulseWidth(double pulse_max, double pulse_min, double range, float angle)
{
    return (pulse_max - pulse_min) * angle / range + (pulse_max + pulse_min) / 2.0;
}

float Root::pressure2Altitude(float t0, float p0, float g0, float p)
{
    return t0 / this->b_ * (pow(p / p0, -this->r_ * this->b_ / g0) - 1);
}

float Root::calcArrayAverage(float array[], int size)
{
    float sum;
    for (int i = 0; i < size; ++i)
    {
        sum += array[i];
    }
    return sum / size;
}

bool Root::terminateConnections(VnSensor *imu)
{
    bool out;
    if (IMU_ACTIVE)
    {
        out = true;
        imu->disconnect();
    }
    else
    {
        out = false;
    }

    return out;
}

void Root::activeSleep(float sleep_time, VnSensor *imu, ImuMeasurementsRegister &response, Log &log, double &start_time)
{
    double current_time = this->getCurrentTime();
    double end_time = sleep_time * 1000 + current_time;
    while (current_time < end_time)
    {
        try
        {
            response = imu->readImuMeasurements();
            log.write(response);
            current_time = getCurrentTime();
        }
        catch (const std::exception &e)
        {
            log.write("Exception: ");
            log.write(e.what());
            log.write("IMU failed to connect... restart program");
            this->time_delay_enabled_ = true;
            return;
        }
    }
}

bool Root::isTimeExceeded(double launch_time, double trigger_time)
{
    if (getCurrentTime() - launch_time > trigger_time * 1000)
    {
        return true;
    }
    else
    {
        return false;
    }
}