#ifndef sirius2c_h__
#define sirius2c_h__

#include "utils/types.h"
#include "utils/string.h"
#include "channels/channel_modbus/devices/modbus_device_base.h"
#include "channels/channel_modbus/devices/modbus_request.h"

#include <boost/circular_buffer.hpp>

namespace Channels
{
	namespace Modbus
	{
		class Sirius2C : public DeviceBase
		{
			// информация об устройстве
			int32_t deviceType_, serialId_, modbusId_;
			bool passCheck_;
			int32_t passAccessDuration_;
			bool calculateParam_;
			int32_t deviceStatus_;
			int32_t checkCount_;

			struct SystemTags
			{
				string_t request_time, device_time, deviceStatus;
			} systemTags;

			struct ParamTag
			{
				string_t I, Ia, Ib, Ic;
			} paramTags;

			// идентификация устройства
			RequestRTU identityRequest_;
			bool identRequested_;

			RequestContainer requestContainer_;

			void Calculate();

		public:
			Sirius2C(boost::shared_ptr<Channel>& parent
					, const string_t& deviceId
					, const int32_t deviceType
					, const int32_t serialId
					, const int32_t modbusId
					, const int32_t primeCurrent
					, const int32_t secondCurrent
					);
			~Sirius2C();

			void DoIdentityRequest(boost::shared_ptr<StreamRTU>& stream);
			void DoDataRequest(boost::shared_ptr<StreamRTU>& stream);

			void SetIdentityRequest(const RequestRTU& request);
			void AddRequest(const RequestRTU& request);
		};
	}
}

#endif // sirius2c_h__
