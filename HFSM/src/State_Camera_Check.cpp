#include "State_Camera_Check.h"
#include "Root.h"
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

State_Camera_Check::State_Camera_Check() : State()
{
}

State_Camera_Check::State_Camera_Check(StateName name, std::map<EventName, StateName> &state_transitions, Root *root) : State(name, state_transitions, root)
{
}

EventName State_Camera_Check::execute()
{
	system("sudo bash ../../cam_assignment.bash");
	bool cam1_online = this->root_->cameraCheck("/dev/videoCam1");
	bool cam2_online = this->root_->cameraCheck("/dev/videoCam2");
	bool cam3_online = this->root_->cameraCheck("/dev/videoCam3");

	/*for (int i = 0; i < this->root_->camera_streams_.size(); i++)
	{
		std::string camera_stream = this->root_->camera_streams_[i];
		if(i == 0) {
			cam1_online = this->root_->cameraCheck(camera_stream);
		} else if(i == 1) {
			cam2_online = this->root_->cameraCheck(camera_stream);
		}else if(i == 2) {
			cam3_online = this->root_->cameraCheck(camera_stream);
		}
	}*/
	if(cam2_online) {
		this->root_->primary_camera_stream_ = this->root_->camera_streams_[2];
	} else if(cam3_online) {
		this->root_->primary_camera_stream_ = this->root_->camera_streams_[1];
	} else if(cam1_online) {
		this->root_->primary_camera_stream_ = this->root_->camera_streams_[0];
	} else {
		this->root_->m_log_.write("Unable to open any of the camera streams");
		this->root_->primary_camera_stream_ = this->root_->camera_streams_[2];

	}
	this->root_->m_log_.write("Primary camera: " + this->root_->primary_camera_stream_ );
	return CAMERA_PICKED;
}

EventName State_Camera_Check::unitExecute()
{
	return this->execute();
}

