#ifndef SIRIUS_2_H
#define SIRIUS_2_H

#include "utils/types.h"
#include <vector>
#include <string>
#include "channels/channel_modbus/devices/modbus_device_base.h"
#include "channels/channel_modbus/devices/modbus_request.h"

namespace Channels
{
	namespace Modbus
	{
		class Sirius2 : public DeviceBase
		{
			// информация об устройстве
			std::string deviceId_;
			int32_t deviceType_;
			int32_t serialId_;
			int32_t modbusId_;
			bool passCheck_;
			int32_t passAccessDuration_;
			int32_t deviceStatus_;
			TagInfoArray tags;

			struct SystemTagsType
			{
				std::string request_time;
				std::string device_time;
				std::string deviceStatus;
			} systemTags;

			std::vector<std::string> tagNameArray_;

			// идентификация устройства
			RequestRTU identityRequest_;
			bool identRequested_;

			RequestContainer requestContainer_;

			void DoDataRequest(boost::shared_ptr<StreamRTU>& stream);

		public:
			Sirius2();
			~Sirius2();

// 			void DoReidentityRequest();
			void DoIdentityRequest(boost::shared_ptr<StreamRTU>& stream);

		};
	}
}

#endif // SIRIUS_2_H