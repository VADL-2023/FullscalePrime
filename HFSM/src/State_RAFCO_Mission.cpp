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
	system("sudo ../../kill_direwolf_and_rtl.bash");
	this->root_->m_log_.write("Starting the SDRs");
	this->root_->radio1_.startSDR();
	this->root_->radio2_.startSDR();

	this->root_->m_log_.write("Waiting for packets");

	// These structs hold the output from the radios
	AX25Packet p1;
	AX25Packet p2;

	// These indicate whether or not errors have occurred on either SDR
	bool sdr1_valid = true;
	bool sdr2_valid = true;
	bool backup_valid = false;
	int angle = 0;
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

	// Save the time that the radios are started
	auto start_time = this->root_->getCurrentTime();

	std::string prev_command = "";

	while ((sdr1_valid || sdr2_valid) && this->root_->getCurrentTime() - start_time < this->root_->length_collect_rafco_ * 1000)
	{
		std::string rafco_command = "";
		if (this->root_->getCurrentTime() - start_time > this->root_->length_collect_rafco_ * 1000 * 0.8)
		{
			backup_valid = true;
		}
		if (sdr1_valid && this->root_->radio1_.packetAvailable())
		{
			// Read the packet from the SDR and print it out
			p1 = this->root_->radio1_.getPacket();
			this->root_->m_log_.write("Radio 1 Received: " + p1.msg);

			// An error will be returned if the SDR becomes disconnected
			// or if there is an issue with the ports
			if (p1.msg.find("ERROR") != std::string::npos)
			{
				sdr1_valid = false;
			}
			else
			{
				rafco_command = p1.msg;
				got_packet = true;
			}
		}

		if (sdr2_valid && this->root_->radio2_.packetAvailable())
		{
			// Read the packet from the SDR and print it out
			p2 = this->root_->radio2_.getPacket();
			this->root_->m_log_.write("Radio 2 Received: " + p2.msg);

			// An error will be returned if the SDR becomes disconnected
			// or if there is an issue with the ports
			if (p2.msg.find("ERROR") != std::string::npos)
			{
				sdr2_valid = false;
			}
			else
			{
				rafco_command = p2.msg;
				got_packet = true;
			}
		}

		std::string command = "";
		// std::string rafco_command = sdr1_output;
		std::string backup_rafco_command = "C3 A1 D4 C3 B2 E5 B2 F6 B2 C3 A1 A1 E5 A1 A1 G7 A1 C3 A1 D4 F6 C3 H8 C3";
		//std::string backup_rafco_command = "B2 B2 B2 B2 B2 B2 B2 B2 B2 B2 B2 B2";
		//std::string backup_rafco_command = "A1 C3 A1 A1 C3 A1 A1 A1 A1 C3 A1 A1 C3 A1";
		std::stringstream rafco_stream(rafco_command);
		bool is_gray = false;
		bool is_blur = false;
		bool is_rotate = false;
		int pic_num = 1;
		if (backup_valid && !got_packet)
		{
			rafco_stream.str(backup_rafco_command);
		}
		if (rafco_stream.str() != prev_command)
		{
			prev_command = rafco_stream.str();
			gpioWrite(this->root_->stepper_2_standby_pin_, 1);
			usleep(5000000);
			std::cout << "Standby Read: " << gpioRead(this->root_->stepper_2_standby_pin_) << std::endl;
			if(this->root_->date_timestamp_ == "") {
				auto end = std::chrono::system_clock::now();

				std::time_t end_time = std::chrono::system_clock::to_time_t(end);

				std::cout << "Date time " << std::ctime(&end_time)
						  << std::endl;
				this->root_->date_timestamp_ = std::ctime(&end_time);
			}
			std::string base_folder = this->root_->date_timestamp_;
			std::replace(base_folder.begin(), base_folder.end(), ' ', '_');
			if (!base_folder.empty())
			{
				base_folder.pop_back();
			}
			else
			{
				std::cout << "Got a weird date time error" << std::endl;
			}

			mkdir(base_folder.c_str(), 0777);
			std::string folder_name_str;
			if (backup_valid && !(sdr1_valid && this->root_->radio1_.packetAvailable()) && !(sdr2_valid && this->root_->radio2_.packetAvailable()))
			{
				folder_name_str = base_folder + "/BackupPrimaryPayloadImages" + this->root_->m_log_.getTimestamp();
			}
			else
			{
				folder_name_str = base_folder + "/PrimaryPayloadImages" + this->root_->m_log_.getTimestamp();
			}
			mkdir(folder_name_str.c_str(), 0777);
			while (rafco_stream >> command)
			{ // Extract word from the stream.
				std::cout << "Command: " << command << std::endl;
				std::cout << "Current angle: " << angle << std::endl;
				if (command == "A1")
				{
					// std::cout << "Standby: " << gpioRead(standby_pin) << std::endl;
					if (angle <= -180)
					{
						this->root_->m_log_.write("Swivel 300 degrees left");
						this->root_->stepper_2_.step(-5 * this->root_->num_steps_);
						std::cout << "Standby Read: " << gpioRead(this->root_->stepper_2_standby_pin_) << std::endl;
						angle += 300;
					}
					else
					{
						this->root_->m_log_.write("Swivel Right");
						this->root_->stepper_2_.step(this->root_->num_steps_);
						std::cout << "Standby Read: " << gpioRead(this->root_->stepper_2_standby_pin_) << std::endl;
						angle -= 60;
					}
					usleep(1000000);
					// gpioWrite(this->root_->stepper_2_standby_pin_, 0);
				}
				else if (command == "B2")
				{
					if (angle >= 180)
					{
						this->root_->m_log_.write("Swivel 300 degrees right");
						this->root_->stepper_2_.step(5 * this->root_->num_steps_);
						std::cout << "Standby Read: " << gpioRead(this->root_->stepper_2_standby_pin_) << std::endl;
						angle -= 300;
					}
					else
					{
						this->root_->m_log_.write("Swivel Left");
						this->root_->stepper_2_.step(-this->root_->num_steps_);
						std::cout << "Standby Read: " << gpioRead(this->root_->stepper_2_standby_pin_) << std::endl;
						angle += 60;
					}
					usleep(1000000);
					// gpioWrite(this->root_->stepper_2_standby_pin_, 0);
				}
				else if (command == "C3")
				{
					cv::Mat frame;
					usleep(1000000);
					int i = 0;
					while (i < 5)
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

						std::string pic_name_str = folder_name_str + "/primary_image_" + std::to_string(pic_num) + ".png";
						std::cout << "PIC NAME STR: " << pic_name_str << std::endl;
						auto end = std::chrono::system_clock::now();
						std::time_t end_time = std::chrono::system_clock::to_time_t(end);
						std::string date_time = std::ctime(&end_time);
						date_time.pop_back();
						cv::Size text_size = cv::getTextSize(date_time, cv::FONT_HERSHEY_COMPLEX, 1, 4, 0);
						cv::Point the_org(0, text_size.height);
						cv::Scalar color1(0, 0, 0);
						cv::Scalar color2(255, 255, 255);
						cv::putText(display_frame, date_time, the_org, cv::FONT_HERSHEY_COMPLEX, 1, color1, 4, cv::LINE_AA);
						cv::putText(display_frame, date_time, the_org, cv::FONT_HERSHEY_COMPLEX, 1, color2, 2, cv::LINE_AA);
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
			usleep(5000000);
			gpioWrite(this->root_->stepper_2_standby_pin_, 0);
			std::cout << "Standby Read: " << gpioRead(this->root_->stepper_2_standby_pin_) << std::endl;
		}

		// TODO: compare if the received values are the same

		p1.msg = "";
		p2.msg = "";
	}

	gpioWrite(this->root_->stepper_2_standby_pin_, 0);
	// Shut down the SDRs
	this->root_->m_log_.write("Shutting down SDRs");
	this->root_->radio1_.stopSDR();
	this->root_->radio2_.stopSDR();
	return RAFCO_COMPLETE;
}

EventName State_RAFCO_Mission::unitExecute()
{
	return this->execute();
}