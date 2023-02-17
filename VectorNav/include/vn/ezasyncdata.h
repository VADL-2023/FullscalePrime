#ifndef _VNSENSORS_EZASYNCDATA_H_
#define _VNSENSORS_EZASYNCDATA_H_

#include <string>

#include "int.h"
#include "nocopy.h"
#include "sensors.h"
#include "compositedata.h"
#include "criticalsection.h"
#include "event.h"
#include "export.h"

namespace vn {
namespace sensors {

/// \brief Provides easy and reliable access to asynchronous data from a
/// VectorNav sensor at the cost of a slight performance hit.
class vn_proglib_DLLEXPORT EzAsyncData : private util::NoCopy
{
private:
	

public:
	// ANDREW NOONAN 8/5/22 MOVED CONSTRUCTOR FROM PRIVATE TO PUBLIC
	//I'm probably just missing a dumb friend class somewhere, but time is tight and this is a quick way to interface w the IMU
	explicit EzAsyncData(VnSensor* sensor);

	/// \brief DTOR
	~EzAsyncData();

	/// \brief Returns the underlying sensor object.
	/// \return The sensor object.
	VnSensor* sensor();

	/// \brief Connects to a sensor with the specified connection parameters.
	///
	/// \param[in] portName The COM port name.
	/// \param[in] baudrate Baudrate to connect to the sensor at.
	/// \return New EzAsyncData object wrapping the connected sensor and
	///     providing easy access to asynchronous data.
	static EzAsyncData* connect(std::string portName, uint32_t baudrate);

	/// \brief Disconnects from the VectorNav sensor.
	void disconnect();

	/// \brief Gets the latest collection of current data received from asychronous
	/// messages from the sensor.
	///
	/// \return The latest current data.
	CompositeData currentData();

	/// \brief Gets the next data packet available and blocks until a data
	/// packet is received if there is currently not data available.
	///
	/// \return The received data packet.
	CompositeData getNextData();

	/// \brief This method will get the next data packet available and block until
	/// a data packet is received if there is currently not data available.
	///
	/// \param[in] timeoutMs The number of milliseconds to wait for the next available data.
	/// \return The next received packet of data.
	/// \exception timeout Did not receive new data by the timeout.
	CompositeData getNextData(int timeoutMs);

private:
	static void asyncPacketReceivedHandler(void* userData, protocol::uart::Packet& p, size_t index);

private:
	VnSensor* _sensor;
	xplat::CriticalSection _mainCS, _copyCS;
	CompositeData _persistentData, _nextData;
	xplat::Event _newDataEvent;
};

}
}

#endif
