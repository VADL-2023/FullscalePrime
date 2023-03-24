#include "State_RAFCO_Mission.h"
#include "Root.h"
#include "SerialObject.h"
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

State_RAFCO_Mission::State_RAFCO_Mission() : State()
{
}

State_RAFCO_Mission::State_RAFCO_Mission(StateName name, std::map<EventName, StateName> &state_transitions, Root *root) : State(name, state_transitions, root)
{
}

EventName State_RAFCO_Mission::execute()
{
	this->root_->m_log_.write("Starting SDRs");
	// this->root_->radio1.startSDR();
	// this->root_->radio2.startSDR();

	this->root_->m_log_.write("Waiting for packets");

	std::string sdr1_output = "";
	std::string sdr2_output = "";
	bool sdr1_valid = true;
	bool sdr2_valid = true;
	bool got_packet = false;

	//--- INITIALIZE VIDEOCAPTURE
	cv::VideoCapture cap;
	// open the default camera using default API
	// cap.open(0);
	// OR advance usage: select any API backend
	int apiID = cv::CAP_ANY; // 0 = autodetect default API
	int numPics = 0;
	// open selected camera using selected API
	cap.open(this->root_->primary_camera_stream_, apiID);
	// check if we succeeded
	if (!cap.isOpened())
	{
		std::cerr << "ERROR! Unable to open camera\n";
	}
	/*SerialObject testObject("/dev/ttyUSB0");
	while (!got_packet)
	{
		// std::cout << "I: " << i << std::endl;
		testObject.writeSerial("Autobots! Transform and roll out");
		std::string thing = testObject.readSerial();
		std::cout << thing << std::endl;
		if (thing.find("Autobots! Transform and roll out") != std::string::npos)
		{
			std::cout << "found!" << '\n';
			got_packet = true;
		}

		// std::cout << "Serial Read: " << testObject.readSerial() << std::endl;
	}*/
	while (sdr1_valid || sdr2_valid)
	{
		/*if (sdr1_valid && this->root_->radio1.packetAvailable())
		{
			sdr1_output = this->root_->radio1.getPacket();
			this->root_->m_log_.write("Radio 1 Received: " + sdr1_output);
			if (sdr1_output.find("shutdown") != std::string::npos)
			{
				this->root_->m_log_.write("Shutting down SDRs");
				this->root_->radio1.stopSDR();
				this->root_->radio2.stopSDR();
				return RAFCO_COMPLETE;
			}
			else if (sdr1_output.find("ERROR") != std::string::npos)
			{
				sdr1_valid = false;
			}
		}
		if (sdr2_valid && this->root_->radio2.packetAvailable())
		{
			sdr2_output = this->root_->radio2.getPacket();
			this->root_->m_log_.write("Radio 2 Received: " + sdr2_output);
			if (sdr2_output.find("shutdown") != std::string::npos)
			{
				this->root_->m_log_.write("Shutting down SDRs");
				this->root_->radio1.stopSDR();
				this->root_->radio2.stopSDR();
				return RAFCO_COMPLETE;
			}
			else if (sdr2_output.find("ERROR") != std::string::npos)
			{
				sdr2_valid = false;
			}
		}*/
		std::string command = "";
		// std::string rafco_command = sdr1_output;
		std::string rafco_command = "C3 A1 D4 C3 B2 E5 B2 F6 B2 C3 A1 A1 E5 A1 A1 G7 A1 C3 A1 D4 F6 C3 H8 C3";
		std::stringstream rafco_stream(rafco_command);
		bool is_gray = false;
		bool is_blur = false;
		bool is_rotate = false;
		int pic_num = 1;
		std::cout << "Start" << std::endl;

		while (rafco_stream >> command)
		{ // Extract word from the stream.
			std::cout << "Command: " << command << std::endl;
			if (command == "A1")
			{
				this->root_->m_log_.write("Swivel clockwise");
				gpioWrite(this->root_->stepper_2_standby_pin_, 1);
				usleep(1000000);
				// std::cout << "Standby: " << gpioRead(standby_pin) << std::endl;
				this->root_->stepper_2_.step(this->root_->num_steps_);
				usleep(1000000);
				// gpioWrite(this->root_->stepper_2_standby_pin_, 0);
			}
			else if (command == "B2")
			{
				this->root_->m_log_.write("Swivel counterclockwise");
				gpioWrite(this->root_->stepper_2_standby_pin_, 1);
				usleep(1000000);
				// std::cout << "Standby: " << gpioRead(standby_pin) << std::endl;
				this->root_->stepper_2_.step(-this->root_->num_steps_);
				usleep(1000000);
				// gpioWrite(this->root_->stepper_2_standby_pin_, 0);
			}
			else if (command == "C3")
			{
				cv::Mat frame;
				usleep(1000000);
				int i = 0;
				while (i < 10)
				{
					auto the_thing = cap.read(frame);
					i++;
				}
				if (frame.empty())
				{
					std::cerr << "ERROR! blank frame grabbed\n";
				}
				else
				{

					cv::Mat display_frame = frame;
					// Need to rotate frame because of how camera is mounted
					cv::rotate(display_frame, display_frame, cv::ROTATE_180);
					// cv::cvtColor(frame, display_frame, cv::COLOR_BGR2RGB);
					if (is_gray)
					{
						cv::cvtColor(display_frame, display_frame, cv::COLOR_RGB2GRAY);
					}
					if (is_rotate)
					{
						cv::rotate(display_frame, display_frame, cv::ROTATE_180);
					}
					if (is_blur)
					{
						cv::GaussianBlur(display_frame, display_frame, cv::Size(51, 51), 0);
					}
					std::string folder_name_str = "PrimaryPayloadImages" + this->root_->m_log_.getTimestamp();
					mkdir(folder_name_str.c_str(), 0777);
					std::string pic_name_str = folder_name_str + "/primary_image_" + std::to_string(pic_num) + ".png";
					std::cout << "PIC NAME STR: " << pic_name_str << std::endl;
					cv::imwrite(pic_name_str, display_frame);
					pic_num++;
				}
			}
			else if (command == "D4")
			{
				is_gray = true;
			}
			else if (command == "E5")
			{
				is_gray = false;
			}
			else if (command == "F6")
			{
				is_rotate = !is_rotate;
			}
			else if (command == "G7")
			{
				is_blur = true;
			}
			else if (command == "H8")
			{
				is_blur = false;
				is_rotate = false;
				is_gray = false;
			}
		}
		std::cout << "Reached the end" << std::endl;
		sdr1_valid = false;
		sdr2_valid = false;
		/*if (sdr1_output.find("A1") == 0 || sdr2_output.find("A1") == 0)
		{
			this->root_->m_log_.write("Swivel one way");
			gpioWrite(this->root_->stepper_2_standby_pin_, 1);
			usleep(1000000);
			// std::cout << "Standby: " << gpioRead(standby_pin) << std::endl;
			this->root_->stepper_2_.step(this->root_->num_steps_);
			usleep(1000000);
			gpioWrite(this->root_->stepper_2_standby_pin_, 0);
		}
		else if (sdr1_output.find("B2") == 0 || sdr2_output.find("B2") == 0)
		{
			this->root_->m_log_.write("Swivel other way");
			gpioWrite(this->root_->stepper_2_standby_pin_, 1);
			usleep(1000000);
			// std::cout << "Standby: " << gpioRead(standby_pin) << std::endl;
			this->root_->stepper_2_.step(-this->root_->num_steps_);
			usleep(1000000);
			gpioWrite(this->root_->stepper_2_standby_pin_, 0);
		}
		sdr1_output = "";
		sdr2_output = "";*/
	}
	gpioWrite(this->root_->stepper_2_standby_pin_, 0);
	this->root_->m_log_.write("Shutting down SDRs");
	this->root_->radio1.stopSDR();
	this->root_->radio2.stopSDR();
	return RAFCO_COMPLETE;
}

EventName State_RAFCO_Mission::unitExecute()
{
	return this->execute();
}