#include "State_Prelaunch.h"
#include "Root.h"

State_Prelaunch::State_Prelaunch() : State()
{
}

State_Prelaunch::State_Prelaunch(StateName name, std::map<EventName, StateName> &state_transitions, Root *root) : State(name, state_transitions, root)
{
}

EventName State_Prelaunch::execute()
{
    std::cout << "In State_Prelaunch and will return end state." << std::endl;
    // variables for initializing pressure, temperature, and gravity
    float pressure_sum;
    float temp_sum;
    float grav_sum;
    auto start = std::chrono::system_clock::now();
    // Some computation here
    auto end = std::chrono::system_clock::now();

    std::chrono::duration<double> elapsed_seconds = end - start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);

    std::cout << "Date time " << std::ctime(&end_time)
              << std::endl;
    this->root_->date_timestamp_ = std::ctime(&end_time);

    // Get start time and date
    time_t raw_time;
    struct tm *time_info;
    time(&raw_time);
    time_info = localtime(&raw_time);
    double start_time = this->root_->getCurrentTime();
    this->root_->m_log_.write("Date: " + std::to_string(time_info->tm_mon + 1) + "/" + std::to_string(time_info->tm_mday) + "\n");
    this->root_->m_log_.write("Flight Name: Subscale\n");
    this->root_->m_log_.write("Verify Critical Parameters: ");
    this->root_->m_log_.write("Motor Burn Time: " + to_string(this->root_->t_burn_) + " s");
    this->root_->m_log_.write("Trigger Acceleration: " + to_string(this->root_->accel_roof_) + " g");
    this->root_->m_log_.write("Launch Detection Samples: " + to_string(this->root_->num_data_points_checked_4_launch_));
    this->root_->m_log_.write("Apogee Detection Samples: " + to_string(this->root_->num_data_points_checked_4_apogee_));
    this->root_->m_log_.write("Landing Detection Wait Time: " + to_string(this->root_->num_seconds_no_new_minimum_) + " s");
    this->root_->m_log_.write("Landing Detection Samples: " + to_string(this->root_->num_data_points_checked_4_landing_));
    this->root_->m_log_.write("Landing Vertical Threshold: " + to_string(this->root_->z_threshold_for_landing_ * this->root_->m_2_ft_) + " ft");
    this->root_->m_log_.write("Max Flight Time For Landing Detection: " + to_string(this->root_->max_flight_time_) + " s");
    this->root_->m_log_.write("Max Parachute Detach Wait Time: " + to_string(this->root_->max_parachute_detach_wait_time_) + " s");
    this->root_->m_log_.write("Callsign to Look For: " + this->root_->callsign_);
    this->root_->m_log_.write("RAFCO Frequency: " + this->root_->rafco_freq_ + "Hz");
    this->root_->m_log_.write("Length of Time to Collect RAFCO: " + to_string(this->root_->length_collect_rafco_) + " s");
    this->root_->m_log_.write("Time Delay Enabled: " + std::to_string(this->root_->time_delay_enabled_));
    this->root_->m_log_.write("-----------------------------------\n\n\n");
    sleep(3);

    // begin GO-NOGO protocol
    bool nogo = true;
    string userInput = "";

    while (nogo)
    {

        // disconnect VN if we had a NOGO response
        if (this->root_->restart_)
        {
            this->root_->m_vn_->disconnect();
            this->root_->restart_ = false;
            this->root_->aac_camera_streams_.clear();
            this->root_->aac_camera_captures_.clear();
        }

        // IMU connection and configuration
        this->root_->m_vn_ = new VnSensor();

        // connect to IMU
        this->root_->m_log_.write("IMU Connecting");
        try
        {
            this->root_->m_vn_->connect(IMU_PORT, IMU_BAUD_RATE);
            this->root_->m_log_.write("IMU Connected");
        }
        catch (const std::exception &e)
        {
            this->root_->m_log_.write("Exception: ");
            this->root_->m_log_.write(e.what());
            this->root_->m_log_.write("IMU failed to connect... restart program");
            return TERMINATE;
        }
        this->root_->is_imu_connected_ = true;

        // flush IMU data during init
        this->root_->m_log_.write("IMU Flushing");
        for (int i = 0; i < this->root_->imu_wait_; ++i)
        {
            try
            {
                this->root_->response_ = this->root_->m_vn_->readImuMeasurements();
                this->root_->m_log_.write(this->root_->response_);
            }
            catch (const std::exception &e)
            {
                this->root_->m_log_.write("Exception: ");
                this->root_->m_log_.write(e.what());
                this->root_->m_log_.write("IMU disconnected... restart program");
                return TERMINATE;
            }
        }
        this->root_->m_log_.write("IMU Flushed");

        // calibrate ground level, pressure, temperature, and gravity
        pressure_sum = 0;
        temp_sum = 0;
        grav_sum = 0;
        this->root_->m_log_.writeTime("Calibrating Baseline Parameters...");

        for (int i = 0; i < this->root_->num_sample_readings_; ++i)
        {
            try
            {
                this->root_->response_ = this->root_->m_vn_->readImuMeasurements();
                this->root_->m_log_.write(this->root_->response_);
                pressure_sum += this->root_->response_.pressure;
                temp_sum += this->root_->response_.temp;
                grav_sum += sqrt(pow(this->root_->response_.accel[0], 2) + pow(this->root_->response_.accel[1], 2) + pow(this->root_->response_.accel[2], 2));
            }
            catch (const std::exception &e)
            {
                this->root_->m_log_.write("Exception: ");
                this->root_->m_log_.write(e.what());
                this->root_->m_log_.write("IMU disconnected... restart program");
                return TERMINATE;
            }
        }

        this->root_->p0_ = pressure_sum / this->root_->num_sample_readings_;
        this->root_->t0_ = temp_sum / this->root_->num_sample_readings_ + this->root_->c_2_k_;
        this->root_->g0_ = grav_sum / this->root_->num_sample_readings_;

        this->root_->m_log_.write("Calibrated Temperature: " + to_string(this->root_->t0_ - this->root_->c_2_k_) + " C");
        this->root_->m_log_.write("Calibrated Pressure: " + to_string(this->root_->p0_) + " kPa");
        this->root_->m_log_.write("Calibrated Gravity: " + to_string(this->root_->g0_) + " m/s^2");
        this->root_->m_log_.saveBaselineParameters(this->root_->r_, this->root_->b_, this->root_->p0_, this->root_->t0_, this->root_->g0_);
        system("sudo bash ../../cam_assignment.bash");
        for (int i = 0; i < this->root_->camera_streams_.size(); i++)
        {
            std::string camera_stream = this->root_->camera_streams_[i];
            if (i == 0)
            {
                if (this->root_->cameraCheck(camera_stream))
                {
                    this->root_->aac_camera_streams_.push_back(camera_stream);
                }
            }
            else if (i == 1)
            {
                if (this->root_->cameraCheck(camera_stream))
                {
                    this->root_->aac_camera_streams_.push_back(camera_stream);
                }
            }
            else if (i == 2)
            {
                if (this->root_->cameraCheck(camera_stream))
                {
                    this->root_->aac_camera_streams_.push_back(camera_stream);
                }
            }
        }

        // RADIO TEST BLOCK: Maybe gotta make a separate silent script?
        /*
        pid_t pid_radio_test = radio();
        // If the child process returns, end the process
        if (pid_radio_test == 0) {
            this->root_->m_log_.write("Radio is not turning on");
        } else {
            this->root_->m_log_.write("Radio on. Process: " + pid_radio_test);
        }
        sleep(10);
        */
        this->root_->m_log_.writeTime("Are we a GO for flight?");
        std::cin >> userInput;
        if (userInput == "GO")
        {
            nogo = false;
        }
        this->root_->m_log_.writeTime(nogo ? "NOGO" : "GO");
        this->root_->m_log_.write("");

        if (nogo)
        {
            this->root_->restart_ = true;
        }
        // kill(pid_radio_test + 3, SIGTERM);
    }
    for (int i = 0; i < this->root_->aac_camera_streams_.size(); ++i)
    {
        std::cout << this->root_->aac_camera_streams_[i] << std::endl;
        if (i == 0)
        {
            if (!this->root_->cap1.isOpened())
            {
                this->root_->cap1.open(this->root_->aac_camera_streams_[i]);
                this->root_->cap1.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
            }
            if (!this->root_->cap1.isOpened())
            {
                std::cerr << "ERROR! Unable to open camera " << i << std::endl;
            }
            else
            {
                for (int j = 0; j < 3; j++)
                {
                    cv::Mat temp_frame;
                    this->root_->cap1.read(temp_frame);
                }
                this->root_->aac_camera_captures_.push_back(this->root_->cap1);
            }
        }
        else if (i == 1)
        {
            if (!this->root_->cap2.isOpened())
            {
                this->root_->cap2.open(this->root_->aac_camera_streams_[i]);
                this->root_->cap2.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
            }
            if (!this->root_->cap2.isOpened())
            {
                std::cerr << "ERROR! Unable to open camera " << i << std::endl;
            }
            else
            {
                for (int j = 0; j < 3; j++)
                {
                    cv::Mat temp_frame;
                    this->root_->cap2.read(temp_frame);
                }
                this->root_->aac_camera_captures_.push_back(this->root_->cap2);
            }
        }
        else if (i == 2)
        {
            if (!this->root_->cap3.isOpened())
            {
                this->root_->cap3.open(this->root_->aac_camera_streams_[i]);
                this->root_->cap3.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
            }
            if (!this->root_->cap3.isOpened())
            {
                std::cerr << "ERROR! Unable to open camera " << i << std::endl;
            }
            else
            {
                for (int j = 0; j < 3; j++)
                {
                    cv::Mat temp_frame;
                    this->root_->cap3.read(temp_frame);
                }
                this->root_->aac_camera_captures_.push_back(this->root_->cap3);
            }
        }
    }

    this->root_->m_log_.writeTime("Pre-Flight Stage Completed");
    return PRELAUNCH_COMPLETE;
}

EventName State_Prelaunch::unitExecute()
{
    return this->execute();
}