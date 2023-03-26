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

    // Instantiate camera streams
    this->camera_streams_.push_back("/dev/videoCam1");
    this->camera_streams_.push_back("/dev/videoCam2");
    this->camera_streams_.push_back("/dev/videoCam3");
}

Root::~Root()
{
    this->cap1.release();
    this->cap2.release();
    this->cap3.release();
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

void Root::camThreadLaunch(cv::VideoCapture *cap, int cam_number)
{
    while (!this->launch_detected_)
    {
        cv::Mat frame;
        cv::rotate(frame, frame, cv::ROTATE_180);
        (*cap).read(frame);
        if (frame.empty())
        {
            std::cerr << "ERROR! blank frame" << cam_number << " grabbed\n";
            break;
        }
        std::string folder_name_str = "SecondaryPayloadImages" + this->m_log_.getTimestamp();
        mkdir(folder_name_str.c_str(), 0777);
        std::string cam_str;
        if (cam_number == 1)
        {
            cam_str = folder_name_str + "/cam1";
            mkdir(cam_str.c_str(), 0777);
            std::string aac_num_string = std::to_string(this->aac_pic_num_cam_1_);
            int precision = this->n_photo_bit_size_ - std::min(this->n_photo_bit_size_, aac_num_string.size());
            aac_num_string.insert(0, precision, '0');
            std::string pic_name_str = cam_str + "/i" + aac_num_string + ".png";
            cv::imwrite(pic_name_str, frame);
            this->aac_pic_num_cam_1_++;
        }
        else if (cam_number == 2)
        {
            cam_str = folder_name_str + "/cam2";
            mkdir(cam_str.c_str(), 0777);
            std::string aac_num_string = std::to_string(this->aac_pic_num_cam_2_);
            int precision = this->n_photo_bit_size_ - std::min(this->n_photo_bit_size_, aac_num_string.size());
            aac_num_string.insert(0, precision, '0');
            std::string pic_name_str = cam_str + "/i" + aac_num_string + ".png";
            cv::imwrite(pic_name_str, frame);
            this->aac_pic_num_cam_2_++;
        }
        else if (cam_number == 3)
        {
            cam_str = folder_name_str + "/cam3";
            mkdir(cam_str.c_str(), 0777);
            std::string aac_num_string = std::to_string(this->aac_pic_num_cam_3_);
            int precision = this->n_photo_bit_size_ - std::min(this->n_photo_bit_size_, aac_num_string.size());
            aac_num_string.insert(0, precision, '0');
            std::string pic_name_str = cam_str + "/i" + aac_num_string + ".png";
            cv::imwrite(pic_name_str, frame);
            this->aac_pic_num_cam_3_++;
        }
    }
}

void Root::camThreadApogee(cv::VideoCapture *cap, int cam_number)
{
    while (!this->apogee_detected_)
    {
        cv::Mat frame;
        cv::rotate(frame, frame, cv::ROTATE_180);
        (*cap).read(frame);
        if (frame.empty())
        {
            std::cerr << "ERROR! blank frame" << cam_number << " grabbed\n";
            break;
        }
        std::string folder_name_str = "SecondaryPayloadImages" + this->m_log_.getTimestamp();
        mkdir(folder_name_str.c_str(), 0777);
        std::string cam_str;
        /*if (cam_number == 1)
        {
            cam_str = folder_name_str + "/cam1";
            mkdir(cam_str.c_str(), 0777);
            std::string aac_num_string = std::to_string(this->aac_pic_num_cam_1_);
            int precision = this->n_photo_bit_size_ - std::min(this->n_photo_bit_size_, aac_num_string.size());
            aac_num_string.insert(0, precision, '0');
            std::string pic_name_str = cam_str + "/i" + aac_num_string + ".png";
            cv::imwrite(pic_name_str, frame);
            this->aac_pic_num_cam_1_++;
        } else if (cam_number == 2)
        {
            cam_str = folder_name_str + "/cam2";
            mkdir(cam_str.c_str(), 0777);
            std::string aac_num_string = std::to_string(this->aac_pic_num_cam_2_);
            int precision = this->n_photo_bit_size_ - std::min(this->n_photo_bit_size_, aac_num_string.size());
            aac_num_string.insert(0, precision, '0');
            std::string pic_name_str = cam_str + "/i" + aac_num_string + ".png";
            cv::imwrite(pic_name_str, frame);
            this->aac_pic_num_cam_2_++;
        } else if (cam_number == 3)
        {
            cam_str = folder_name_str + "/cam3";
            mkdir(cam_str.c_str(), 0777);
            std::string aac_num_string = std::to_string(this->aac_pic_num_cam_3_);
            int precision = this->n_photo_bit_size_ - std::min(this->n_photo_bit_size_, aac_num_string.size());
            aac_num_string.insert(0, precision, '0');
            std::string pic_name_str = cam_str + "/i" + aac_num_string + ".png";
            cv::imwrite(pic_name_str, frame);
            this->aac_pic_num_cam_3_++;
        }*/
    }
}

void Root::camThreadLift(cv::VideoCapture *cap) {
    std::vector<cv::Mat> frames;
    std::string folder_name_str = "Lift" + this->m_log_.getTimestamp();
    mkdir(folder_name_str.c_str(), 0777);
    while(!this->lift_done_) {
        cv::Mat frame;
        
        (*cap).read(frame);
        cv::rotate(frame, frame, cv::ROTATE_180);
        if (frame.empty())
        {
            std::cerr << "ERROR! blank frame" << " grabbed\n";
            break;
        }
        frames.push_back(frame);
    }
    std::cout << "Frames size: " << frames.size() << std::endl;
    for (int i = 0; i < frames.size(); i++)
    {
        std::string rcb_num_string = std::to_string(i);
        int precision = this->n_photo_bit_size_ - std::min(this->n_photo_bit_size_, rcb_num_string.size());
        rcb_num_string.insert(0, precision, '0');
        std::string pic_name_str = folder_name_str + "/i" + rcb_num_string + ".png";
        cv::imwrite(pic_name_str, frames[i]);
    }
}

void Root::camThreadRCB(cv::VideoCapture *cap) {
    std::vector<cv::Mat> frames;
    std::string folder_name_str = "RCB" + this->m_log_.getTimestamp();
    mkdir(folder_name_str.c_str(), 0777);
    while(!this->rcb_done_) {
        cv::Mat frame;
        
        (*cap).read(frame);
        cv::rotate(frame, frame, cv::ROTATE_180);
        if (frame.empty())
        {
            std::cerr << "ERROR! blank frame" << " grabbed\n";
            break;
        }
        frames.push_back(frame);
    }
    std::cout << "Frames size: " << frames.size() << std::endl;
    for (int i = 0; i < frames.size(); i++)
    {
        std::string rcb_num_string = std::to_string(i);
        int precision = this->n_photo_bit_size_ - std::min(this->n_photo_bit_size_, rcb_num_string.size());
        rcb_num_string.insert(0, precision, '0');
        std::string pic_name_str = folder_name_str + "/i" + rcb_num_string + ".png";
        cv::imwrite(pic_name_str, frames[i]);
    }
}

void Root::camThreadLanding(cv::VideoCapture *cap, int cam_number)
{
    double prev_time = getCurrentTime();
    std::vector<cv::Mat> frames;
    std::string folder_name_str = "SecondaryPayloadImages" + this->m_log_.getTimestamp();
    mkdir(folder_name_str.c_str(), 0777);
    std::string cam_str;

    while (!this->landing_detected_)
    {
        double curr_time = getCurrentTime();
        prev_time = curr_time;
        cv::Mat frame;
        
        (*cap).read(frame);
        cv::rotate(frame, frame, cv::ROTATE_180);
        if (frame.empty())
        {
            std::cerr << "ERROR! blank frame" << cam_number << " grabbed\n";
            break;
        }
        frames.push_back(frame);
        /*if (cam_number == 1)
        {
            cam_str = folder_name_str + "/cam1";
            mkdir(cam_str.c_str(), 0777);
            std::string aac_num_string = std::to_string(this->aac_pic_num_cam_1_);
            int precision = this->n_photo_bit_size_ - std::min(this->n_photo_bit_size_, aac_num_string.size());
            aac_num_string.insert(0, precision, '0');
            std::string pic_name_str = cam_str + "/i" + aac_num_string + ".png";
            cv::imwrite(pic_name_str, frame);
            this->aac_pic_num_cam_1_++;
        } else if (cam_number == 2)
        {
            cam_str = folder_name_str + "/cam2";
            mkdir(cam_str.c_str(), 0777);
            std::string aac_num_string = std::to_string(this->aac_pic_num_cam_2_);
            int precision = this->n_photo_bit_size_ - std::min(this->n_photo_bit_size_, aac_num_string.size());
            aac_num_string.insert(0, precision, '0');
            std::string pic_name_str = cam_str + "/i" + aac_num_string + ".png";
            cv::imwrite(pic_name_str, frame);
            this->aac_pic_num_cam_2_++;
        } else if (cam_number == 3)
        {
            cam_str = folder_name_str + "/cam3";
            mkdir(cam_str.c_str(), 0777);
            std::string aac_num_string = std::to_string(this->aac_pic_num_cam_3_);
            int precision = this->n_photo_bit_size_ - std::min(this->n_photo_bit_size_, aac_num_string.size());
            aac_num_string.insert(0, precision, '0');
            std::string pic_name_str = cam_str + "/i" + aac_num_string + ".png";
            cv::imwrite(pic_name_str, frame);
            this->aac_pic_num_cam_3_++;
        }*/
    }
    if (cam_number == 1)
    {
        cam_str = folder_name_str + "/cam1";
        mkdir(cam_str.c_str(), 0777);
    }
    else if (cam_number == 2)
    {
        cam_str = folder_name_str + "/cam2";
        mkdir(cam_str.c_str(), 0777);
    } else if(cam_number == 3) {
        cam_str = folder_name_str + "/cam3";
        mkdir(cam_str.c_str(), 0777);
    }
    for (int i = 0; i < frames.size(); i++)
    {
        std::string aac_num_string = std::to_string(i);
        int precision = this->n_photo_bit_size_ - std::min(this->n_photo_bit_size_, aac_num_string.size());
        aac_num_string.insert(0, precision, '0');
        std::string pic_name_str = cam_str + "/i" + aac_num_string + ".png";
        cv::imwrite(pic_name_str, frames[i]);
    }
}

void Root::activeSleep(float sleep_time, VnSensor *imu, ImuMeasurementsRegister &response, Log &log, double &start_time)
{
    double current_time = this->getCurrentTime();
    double end_time = sleep_time * 1000 + current_time;
    // for(int i = 0; i < this->aac_camera_captures_.size(); i++) {
    for (int i = 0; i < this->aac_camera_captures_.size(); i++)
    {
        if (this->aac_camera_streams_[i] == "/dev/videoCam1")
        {
            std::thread t1(&Root::camThreadLanding, this, &this->aac_camera_captures_[i], 1);
            this->threads_.push_back(move(t1));
        }
        else if (this->aac_camera_streams_[i] == "/dev/videoCam2")
        {
            std::thread t2(&Root::camThreadLanding, this, &this->aac_camera_captures_[i], 2);
            this->threads_.push_back(move(t2));
        }
        else if (this->aac_camera_streams_[i] == "/dev/videoCam3")
        {
            std::thread t3(&Root::camThreadLanding, this, &this->aac_camera_captures_[i], 3);
            this->threads_.push_back(move(t3));
        }
    }
    std::cout << "Thread size: " << this->threads_.size() << std::endl;
    while (current_time < end_time)
    {
        try
        {
            response = imu->readImuMeasurements();
            log.write(response);
            current_time = getCurrentTime();
            /*for (int i = 0; i < this->aac_camera_captures_.size(); i++)
            {
                cv::Mat frame;
                cv::rotate(frame, frame, cv::ROTATE_180);
                this->aac_camera_captures_[i].read(frame);
                if (frame.empty())
                {
                    std::cerr << "ERROR! blank frame" << i << " grabbed\n";
                    break;
                }
                std::string folder_name_str = "SecondaryPayloadImages" + this->m_log_.getTimestamp();
                mkdir(folder_name_str.c_str(), 0777);
                std::string cam_str;
                if (this->aac_camera_streams_[i] == "/dev/videoCam1")
                {
                    cam_str = folder_name_str + "/cam1";
                    mkdir(cam_str.c_str(), 0777);
                }
                else if (this->aac_camera_streams_[i] == "/dev/videoCam2")
                {
                    cam_str = folder_name_str + "/cam2";
                    mkdir(cam_str.c_str(), 0777);
                }
                else if (this->aac_camera_streams_[i] == "/dev/videoCam3")
                {
                    cam_str = folder_name_str + "/cam3";
                    mkdir(cam_str.c_str(), 0777);
                }
                std::string aac_num_string = std::to_string(this->aac_pic_num_);
                int precision = this->n_photo_bit_size_ - std::min(this->n_photo_bit_size_, aac_num_string.size());
                aac_num_string.insert(0, precision, '0');
                std::string pic_name_str = cam_str + "/i" + aac_num_string + ".png";
                cv::imwrite(pic_name_str, frame);
            }*/
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
    this->launch_detected_ = true;
    std::cout << "DONE" << std::endl;
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

bool Root::cameraCheck(std::string camera_stream)
{
    cv::Mat frame;
    //--- INITIALIZE VIDEOCAPTURE
    cv::VideoCapture cap;
    // open the default camera using default API
    // cap.open(0);
    // OR advance usage: select any API backend
    int deviceID = 100;      // 0 = open default camera
    int apiID = cv::CAP_ANY; // 0 = autodetect default API
    bool isGray = false;
    bool isRotated = false;
    bool isBlurred = false;
    int numPics = 0;
    // open selected camera using selected API
    cap.open(camera_stream, apiID);
    // check if we succeeded
    if (!cap.isOpened())
    {
        std::cerr << "ERROR! Unable to open camera\n";
        return false;
    }
    std::cout << "Opened camera " << camera_stream << std::endl;
    // wait for a new frame from camera and store it into 'frame'
    cap.read(frame);
    // check if we succeeded
    if (frame.empty())
    {
        std::cerr << "ERROR! blank frame grabbed\n";
        return false;
    }
    return true;
}