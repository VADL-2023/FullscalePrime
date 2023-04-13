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
    this->camera_streams_.push_back("/dev/videoCam3");
    this->camera_streams_.push_back("/dev/videoCam2");
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
        if (this->date_timestamp_ == "")
        {
            auto end = std::chrono::system_clock::now();

            std::time_t end_time = std::chrono::system_clock::to_time_t(end);

            std::cout << "Date time " << std::ctime(&end_time)
                      << std::endl;
            this->date_timestamp_ = std::ctime(&end_time);
        }
        std::string base_folder = this->date_timestamp_;
        std::replace(base_folder.begin(), base_folder.end(), ' ', '_');
        if (!base_folder.empty())
        {
            base_folder.pop_back();
        }
        else
        {
            std::cout << "Got a weird date time error" << std::endl;
        }
        std::cout << "Base folder name: " << base_folder << std::endl;
        std::cout << "Length: " << base_folder.length() << std::endl;
        std::cout << "2023 Spot" << base_folder.find("2023") << std::endl;
        mkdir(base_folder.c_str(), 0777);
        std::string folder_name_str = base_folder + "/SecondaryPayloadImages" + this->m_log_.getTimestamp();
        std::cout << "Folder name string: " << folder_name_str << std::endl;
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
        if (this->date_timestamp_ == "")
        {
            auto end = std::chrono::system_clock::now();

            std::time_t end_time = std::chrono::system_clock::to_time_t(end);

            std::cout << "Date time " << std::ctime(&end_time)
                      << std::endl;
            this->date_timestamp_ = std::ctime(&end_time);
        }
        std::string base_folder = this->date_timestamp_;
        std::replace(base_folder.begin(), base_folder.end(), ' ', '_');
        if (!base_folder.empty())
        {
            base_folder.pop_back();
        }
        else
        {
            std::cout << "Got a weird date time error" << std::endl;
        }
        std::cout << "Base folder name: " << base_folder << std::endl;
        std::cout << "Length: " << base_folder.length() << std::endl;
        std::cout << "2023 Spot" << base_folder.find("2023") << std::endl;
        mkdir(base_folder.c_str(), 0777);
        std::string folder_name_str = base_folder + "/SecondaryPayloadImages" + this->m_log_.getTimestamp();
        std::cout << "Folder name string: " << folder_name_str << std::endl;
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

void Root::camThreadLift(cv::VideoCapture *cap)
{
    std::vector<cv::Mat> frames;
    std::vector<std::string> date_times;
    if (this->date_timestamp_ == "")
    {
        auto end = std::chrono::system_clock::now();

        std::time_t end_time = std::chrono::system_clock::to_time_t(end);

        this->date_timestamp_ = std::ctime(&end_time);
    }
    std::string base_folder = this->date_timestamp_;
    std::replace(base_folder.begin(), base_folder.end(), ' ', '_');
    if (!base_folder.empty())
    {
        base_folder.pop_back();
    }
    else
    {
        this->m_log_.write("Invalid Date time format");
    }

    mkdir(base_folder.c_str(), 0777);
    std::string folder_name_str = base_folder + "/Lift" + this->m_log_.getTimestamp();
    std::cout << "Folder name string: " << folder_name_str << std::endl;
    mkdir(folder_name_str.c_str(), 0777);

    while (!this->lift_done_)
    {
        cv::Mat frame;

        (*cap).read(frame);
        cv::rotate(frame, frame, cv::ROTATE_180);
        if (frame.empty())
        {
            this->m_log_.write("ERROR! Blank frame from camera");
            break;
        }
        auto end = std::chrono::system_clock::now();
        std::time_t end_time = std::chrono::system_clock::to_time_t(end);
        std::string date_time = std::ctime(&end_time);
        date_time.pop_back();
        date_times.push_back(date_time);
        frames.push_back(frame);
    }
    this->m_log_.write("Lift Frame Size: " + frames.size());
    for (int i = 0; i < frames.size(); i++)
    {
        std::string rcb_num_string = std::to_string(i);
        int precision = this->n_photo_bit_size_ - std::min(this->n_photo_bit_size_, rcb_num_string.size());
        rcb_num_string.insert(0, precision, '0');
        std::string pic_name_str = folder_name_str + "/i" + rcb_num_string + ".png";
        cv::Size text_size = cv::getTextSize(date_times[i], cv::FONT_HERSHEY_COMPLEX, 1, 4, 0);
        cv::Point the_org(0, text_size.height);
        cv::Scalar color1(0, 0, 0);
        cv::Scalar color2(255, 255, 255);
        auto the_frame = frames[i];
        cv::putText(the_frame, date_times[i], the_org, cv::FONT_HERSHEY_COMPLEX, 1, color1, 4, cv::LINE_AA);
        cv::putText(the_frame, date_times[i], the_org, cv::FONT_HERSHEY_COMPLEX, 1, color2, 2, cv::LINE_AA);
        cv::imwrite(pic_name_str, the_frame);
    }
}

void Root::camThreadRCB(cv::VideoCapture *cap)
{
    std::vector<cv::Mat> frames;
    std::vector<std::string> date_times;
    if (this->date_timestamp_ == "")
    {
        auto end = std::chrono::system_clock::now();

        std::time_t end_time = std::chrono::system_clock::to_time_t(end);

        this->date_timestamp_ = std::ctime(&end_time);
    }
    std::string base_folder = this->date_timestamp_;
    std::replace(base_folder.begin(), base_folder.end(), ' ', '_');
    if (!base_folder.empty())
    {
        base_folder.pop_back();
    }
    else
    {
        this->m_log_.write("Invalid Date time format");
    }

    mkdir(base_folder.c_str(), 0777);
    std::string folder_name_str = base_folder + "/RCB" + this->m_log_.getTimestamp();
    std::cout << "Folder name string: " << folder_name_str << std::endl;
    mkdir(folder_name_str.c_str(), 0777);
    while (!this->rcb_done_)
    {
        cv::Mat frame;

        (*cap).read(frame);
        cv::rotate(frame, frame, cv::ROTATE_180);
        if (frame.empty())
        {
            this->m_log_.write("ERROR! Blank frame from camera");
            break;
        }
        auto end = std::chrono::system_clock::now();
        std::time_t end_time = std::chrono::system_clock::to_time_t(end);
        std::string date_time = std::ctime(&end_time);
        date_time.pop_back();
        date_times.push_back(date_time);
        frames.push_back(frame);
    }
    this->m_log_.write("RCB Frames " + frames.size());
    for (int i = 0; i < frames.size(); i++)
    {
        std::string rcb_num_string = std::to_string(i);
        int precision = this->n_photo_bit_size_ - std::min(this->n_photo_bit_size_, rcb_num_string.size());
        rcb_num_string.insert(0, precision, '0');
        std::string pic_name_str = folder_name_str + "/i" + rcb_num_string + ".png";
        cv::Size text_size = cv::getTextSize(date_times[i], cv::FONT_HERSHEY_COMPLEX, 1, 4, 0);
        cv::Point the_org(0, text_size.height);
        cv::Scalar color1(0, 0, 0);
        cv::Scalar color2(255, 255, 255);
        auto the_frame = frames[i];
        cv::putText(the_frame, date_times[i], the_org, cv::FONT_HERSHEY_COMPLEX, 1, color1, 4, cv::LINE_AA);
        cv::putText(the_frame, date_times[i], the_org, cv::FONT_HERSHEY_COMPLEX, 1, color2, 2, cv::LINE_AA);
        cv::imwrite(pic_name_str, the_frame);
    }
}

void Root::camThreadLanding(cv::VideoCapture *cap, int cam_number, int max_photos)
{
    double prev_time = getCurrentTime();
    std::vector<cv::Mat> frames;
    std::vector<std::string> date_times;
    if (this->date_timestamp_ == "")
    {
        auto end = std::chrono::system_clock::now();

        std::time_t end_time = std::chrono::system_clock::to_time_t(end);

        this->date_timestamp_ = std::ctime(&end_time);
    }
    std::string base_folder = this->date_timestamp_;
    std::replace(base_folder.begin(), base_folder.end(), ' ', '_');
    if (!base_folder.empty())
    {
        base_folder.pop_back();
    }
    else
    {
        this->m_log_.write("Invalid Date time format");
    }

    mkdir(base_folder.c_str(), 0777);
    std::string folder_name_str = base_folder + "/SecondaryPayloadImages" + this->m_log_.getTimestamp();
    this->m_log_.write("AAC Video Folder: " + folder_name_str);
    mkdir(folder_name_str.c_str(), 0777);
    std::string cam_str;

    try
    {
        while (!this->landing_detected_ && frames.size() < max_photos)
        {
            double curr_time = getCurrentTime();
            prev_time = curr_time;
            cv::Mat frame;

            (*cap).read(frame);
            cv::rotate(frame, frame, cv::ROTATE_180);
            if (frame.empty())
            {
                this->m_log_.write("ERROR! Blank frame from camera " + cam_number);
                break;
            }
            auto end = std::chrono::system_clock::now();
            std::time_t end_time = std::chrono::system_clock::to_time_t(end);
            std::string date_time = std::ctime(&end_time);
            date_time.pop_back();
            frames.push_back(frame);
            date_times.push_back(date_time);
            //std::cout << "Thread " << cam_number << ": " << frames.size() << std::endl;
        }
    }
    catch (...)
    {
        std::string error_message = "Exception thrown capturing frames for camera " + std::to_string(cam_number);
        this->m_log_.write(error_message);
    }
    if (cam_number == 1)
    {
        cam_str = folder_name_str + "/cam1";
        mkdir(cam_str.c_str(), 0777);
        this->m_log_.write("Camera 1 Folder: " + cam_str);
    }
    else if (cam_number == 2)
    {
        cam_str = folder_name_str + "/cam2";
        mkdir(cam_str.c_str(), 0777);
        this->m_log_.write("Camera 2 Folder: " + cam_str);
    }
    else if (cam_number == 3)
    {
        cam_str = folder_name_str + "/cam3";
        mkdir(cam_str.c_str(), 0777);
        this->m_log_.write("Camera 3 Folder: " + cam_str);
    }
    try
    {
        for (int i = 0; i < frames.size(); i++)
        {
            std::string aac_num_string = std::to_string(i);
            int precision = this->n_photo_bit_size_ - std::min(this->n_photo_bit_size_, aac_num_string.size());
            aac_num_string.insert(0, precision, '0');
            std::string pic_name_str = cam_str + "/i" + aac_num_string + ".png";
            auto the_frame = frames[i];
            cv::Size text_size = cv::getTextSize(date_times[i], cv::FONT_HERSHEY_COMPLEX, 1, 4, 0);
            cv::Point the_org(0, text_size.height);
            cv::Scalar color1(0, 0, 0);
            cv::Scalar color2(255, 255, 255);
            cv::putText(the_frame, date_times[i], the_org, cv::FONT_HERSHEY_COMPLEX, 1, color1, 4, cv::LINE_AA);
            cv::putText(the_frame, date_times[i], the_org, cv::FONT_HERSHEY_COMPLEX, 1, color2, 2, cv::LINE_AA);
            cv::imwrite(pic_name_str, the_frame);
        }
    }
    catch (...)
    {
        std::string error_message = "Exception thrown writing frames for camera " + std::to_string(cam_number);
        this->m_log_.write(error_message);
    }
}

void Root::realCamThreadLift(cv::VideoCapture *cap, cv::VideoWriter *video)
{
    bool is_first = true;
    int i = 0;
    int index = 0;
    double max_time = 0;
    double min_time = 10000000;
    while (!this->lift_done_)
    {
        cv::Mat frame;
        auto start_time = getCurrentTime();
        (*cap) >> frame;
        if (frame.empty())
        {
            break;
        }
        cv::rotate(frame, frame, cv::ROTATE_180);
        auto end = std::chrono::system_clock::now();
        std::time_t end_time = std::chrono::system_clock::to_time_t(end);
        std::string date_time = std::ctime(&end_time);
        date_time.pop_back();

        cv::Size text_size = cv::getTextSize(date_time, cv::FONT_HERSHEY_COMPLEX, 1, 4, 0);
        cv::Point the_org(0, text_size.height);
        cv::Scalar color1(0, 0, 0);
        cv::Scalar color2(255, 255, 255);
        cv::putText(frame, date_time, the_org, cv::FONT_HERSHEY_COMPLEX, 1, color1, 4, cv::LINE_AA);
        cv::putText(frame, date_time, the_org, cv::FONT_HERSHEY_COMPLEX, 1, color2, 2, cv::LINE_AA);
        (*video).write(frame);
        auto write_time = getCurrentTime();
        /*if(cam_number == 2) {
          std::cout << "Time: " << write_time - start_time << std::endl;
        }*/
        if (!is_first && write_time - start_time > max_time)
        {
            max_time = write_time - start_time;
        }
        if (!is_first && write_time - start_time < min_time)
        {
            min_time = write_time - start_time;
        }
        is_first = false;
        i++;
    }
    std::cout << "Lift Max time: " << max_time << std::endl;
    std::cout << "Lift Min time: " << min_time << std::endl;
    (*video).release();
    cap->release();
}

void Root::realCamThreadRCB(cv::VideoCapture *cap, cv::VideoWriter *video)
{
    bool is_first = true;
    int i = 0;
    int index = 0;
    double max_time = 0;
    double min_time = 10000000;
    while (!this->rcb_done_)
    {
        cv::Mat frame;
        auto start_time = getCurrentTime();
        (*cap) >> frame;
        if (frame.empty())
        {
            break;
        }
        cv::rotate(frame, frame, cv::ROTATE_180);
        auto end = std::chrono::system_clock::now();
        std::time_t end_time = std::chrono::system_clock::to_time_t(end);
        std::string date_time = std::ctime(&end_time);
        date_time.pop_back();
        cv::Size text_size = cv::getTextSize(date_time, cv::FONT_HERSHEY_COMPLEX, 1, 4, 0);
        cv::Point the_org(0, text_size.height);
        cv::Scalar color1(0, 0, 0);
        cv::Scalar color2(255, 255, 255);
        cv::putText(frame, date_time, the_org, cv::FONT_HERSHEY_COMPLEX, 1, color1, 4, cv::LINE_AA);
        cv::putText(frame, date_time, the_org, cv::FONT_HERSHEY_COMPLEX, 1, color2, 2, cv::LINE_AA);
        (*video).write(frame);
        auto write_time = getCurrentTime();
        /*if(cam_number == 2) {
          std::cout << "Time: " << write_time - start_time << std::endl;
        }*/
        if (!is_first && write_time - start_time > max_time)
        {
            max_time = write_time - start_time;
        }
        if (!is_first && write_time - start_time < min_time)
        {
            min_time = write_time - start_time;
        }
        is_first = false;
        i++;
    }
    std::cout << "RCB Max time: " << max_time << std::endl;
    std::cout << "RCB Min time: " << min_time << std::endl;
    (*video).release();
    cap->release();
}
void Root::realCamThreadLanding(cv::VideoCapture *cap, std::vector<cv::VideoWriter> *videos, int cam_number)
{
    bool is_first = true;
    int i = 0;
    int index = 0;
    double max_time = 0;
    double min_time = 10000000;
    while (!this->landing_detected_)
    {
        cv::Mat frame;
        auto start_time = getCurrentTime();
        (*cap) >> frame;
        if (frame.empty())
        {
            break;
        }
        cv::rotate(frame, frame, cv::ROTATE_180);
        auto end = std::chrono::system_clock::now();
        std::time_t end_time = std::chrono::system_clock::to_time_t(end);
        std::string date_time = std::ctime(&end_time);
        date_time.pop_back();
        cv::Size text_size = cv::getTextSize(date_time, cv::FONT_HERSHEY_COMPLEX, 1, 4, 0);
        cv::Point the_org(0, text_size.height);
        cv::Scalar color1(0, 0, 0);
        cv::Scalar color2(255, 255, 255);
        cv::putText(frame, date_time, the_org, cv::FONT_HERSHEY_COMPLEX, 1, color1, 4, cv::LINE_AA);
        cv::putText(frame, date_time, the_org, cv::FONT_HERSHEY_COMPLEX, 1, color2, 2, cv::LINE_AA);
        index = i / this->frames_per_vid_;
        if (index >= this->max_proper_flight_time_ / this->vid_clip_time_)
        {
            index = (this->max_flight_time_ / this->vid_clip_time_) - 1;
        }
        (*videos)[index].write(frame);
        auto write_time = getCurrentTime();
        /*if(cam_number == 2) {
          std::cout << "Time: " << write_time - start_time << std::endl;
        }*/
        if (!is_first && write_time - start_time > max_time)
        {
            max_time = write_time - start_time;
        }
        if (!is_first && write_time - start_time < min_time)
        {
            min_time = write_time - start_time;
        }
        is_first = false;
        i++;
    }
    std::cout << "Thread " << cam_number << ": Max time: " << max_time << std::endl;
    std::cout << "Thread " << cam_number << ": Min time: " << min_time << std::endl;
    for (int i = 0; i < this->max_proper_flight_time_ / this->vid_clip_time_; i++)
    {
        (*videos)[i].release();
    }
    cap->release();
    std::cout << "Thread " << cam_number << ": Done" << std::endl;
}

void Root::activeSleep(float sleep_time, VnSensor *imu, ImuMeasurementsRegister &response, Log &log, double &start_time)
{
    double current_time = this->getCurrentTime();
    double end_time = sleep_time * 1000 + current_time;
    // for(int i = 0; i < this->aac_camera_captures_.size(); i++) {
    /*for (int i = 0; i < this->aac_camera_captures_.size(); i++)
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
    std::cout << "Thread size: " << this->threads_.size() << std::endl;*/
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
    this->launch_detected_ = true;
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
    // open the default camera using default API
    // cap.open(0);
    // OR advance usage: select any API backend
    cv::VideoCapture cap;
    int deviceID = 100;      // 0 = open default camera
    int apiID = cv::CAP_ANY; // 0 = autodetect default API
    bool isGray = false;
    bool isRotated = false;
    bool isBlurred = false;
    int numPics = 0;
    this->m_log_.write("Trying to open camera stream: " + camera_stream);
    // open selected camera using selected API
    if (!cap.isOpened())
    {
        cap.open(camera_stream);

        cap.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
        // check if we succeeded
        if (!cap.isOpened())
        {
            this->m_log_.write("ERROR! Unable to open camera " + camera_stream);
            return false;
        }
    }
    this->m_log_.write("Opened camera " + camera_stream);
    // wait for a new frame from camera and store it into 'frame'
    cap.read(frame);
    // check if we succeeded
    if (frame.empty())
    {
        this->m_log_.write("ERROR! Blank frame grabbed from " + camera_stream);
        return false;
    }
    cap.release();
    usleep(1000000);
    return true;
}