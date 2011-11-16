#include "channels/channel_modbus/devices/generic_device.h"
#include "core/project_mgr.h"
#include "channels/cache_mgr.h"
#include "utils/timer.hpp"
#include <boost/foreach.hpp>

namespace Channels
{
	namespace Modbus
	{

		GenericDevice::GenericDevice(boost::shared_ptr<Channel>& parent, const string_t& deviceId)
			: DeviceBase(parent, deviceId)
		{
			deviceStatus_ = 0;
		}

		GenericDevice::~GenericDevice()
		{

		}

		const std::vector<std::string> GenericDevice::GetRegistredTags() const
		{
			return tagNameArray_;
		}

		void GenericDevice::DoDataRequest(boost::shared_ptr<StreamRTU>& stream)
		{
			CXTimer devicePerformance;

			BOOST_FOREACH ( const RequestRTU& request, requestContainer_)
			{
				CXTimer requestPerformance;

				DoRequest(request, stream);

				projectMgr::getInstance().GetCacheMgr()->GetItem(systemTags.request_time)->CopyFrom(OPC_QUALITY_GOOD, ComVariant(requestPerformance.GetElapsedSec()));
			}

			projectMgr::getInstance().GetCacheMgr()->GetItem(systemTags.device_time)->CopyFrom(OPC_QUALITY_GOOD, ComVariant(devicePerformance.GetElapsedSec()));
		}
	}
}