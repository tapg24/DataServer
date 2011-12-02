#include "channels/channel_modbus/devices/generic_device.h"
#include "core/project_mgr.h"
#include "channels/cache_mgr.h"
#include "channels/channel_modbus/modbus_rtu_stream.h"
#include "utils/timer.hpp"
#include <boost/foreach.hpp>

namespace Channels
{
	namespace Modbus
	{

		GenericDevice::GenericDevice(ChannelWPtr parent, const string_t& deviceId)
			: ModbusDeviceBase(parent, deviceId)
		{
			deviceStatus_ = 0;
		}

		GenericDevice::~GenericDevice()
		{

		}

		const std::vector<string_t> GenericDevice::GetRegistredTags() const
		{
			return tagNameArray_;
		}

		void GenericDevice::DoDataRequest(boost::shared_ptr<StreamRTU>& stream)
		{
			CXTimer devicePerformance;

			BOOST_FOREACH ( const RequestRTU& request, requestContainer_)
			{
				CXTimer requestPerformance;

				ModbusRequest::DoRequest(request, stream);

				projectMgr::getInstance().GetCacheMgr()->GetItem(systemTags.request_time)->CopyFrom(OPC_QUALITY_GOOD, ComVariant(requestPerformance.GetElapsedSec()));
			}

			projectMgr::getInstance().GetCacheMgr()->GetItem(systemTags.device_time)->CopyFrom(OPC_QUALITY_GOOD, ComVariant(devicePerformance.GetElapsedSec()));
		}
	}
}