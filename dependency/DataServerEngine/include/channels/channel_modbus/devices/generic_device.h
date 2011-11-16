#ifndef GENERIC_DEVICE_H
#define GENERIC_DEVICE_H

#include "utils/types.h"
#include "utils/string.h"
#include <vector>
#include <boost/shared_ptr.hpp>
#include "channels/tag_info.h"
#include "channels/channel_modbus/modbus_rtu_stream.h"
#include "modbus_device_base.h"
#include "modbus_request.h"

namespace Channels
{
	namespace Modbus
	{
		class GenericDevice : public DeviceBase
		{
			// информация об устройстве
			//std::string deviceId_;
			int32_t deviceType_;
			int32_t serialId_;
			int32_t modbusId_;
			int32_t deviceStatus_;
			std::vector<TagInfo> tags;

			struct SystemTagsType
			{
				string_t request_time;
				string_t device_time;
				string_t deviceStatus;
			} systemTags;

			std::vector<string_t> tagNameArray_;

			RequestContainer requestContainer_;

		public:
			GenericDevice(boost::shared_ptr<Channel>& parent, const string_t& deviceId);
			~GenericDevice();

			//void Init(const boost::property_tree::ptree& ptree);
			void DoDataRequest(boost::shared_ptr<StreamRTU>& stream);
			const std::vector<string_t> GetRegistredTags() const;
		};
	}
}

#endif // GENERIC_DEVICE_H