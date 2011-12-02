#ifndef sirius2_base_h__
#define sirius2_base_h__

#include "utils/types.h"
#include "utils/string.h"
#include "channels/channel_modbus/devices/modbus_device_base.h"
#include "channels/channel_modbus/devices/modbus_request.h"

namespace Channels
{
	namespace Modbus
	{
		enum DeviceError
		{
			TypeNotValid = 1,
			SerialNotValid
		};

		class Sirius2Base : public ModbusDeviceBase
		{
			// информация об устройстве
			int32_t deviceType_, serialId_, modbusId_;
			string_t type_string_;
//			bool passCheck_;
//			int32_t passAccessDuration_;
//			bool calculateParam_;
			int32_t deviceStatus_;
			int32_t checkCount_;

			// системные теги
			struct SystemTags {
				// идентификация
				string_t type, serial_code;
				// performance
				string_t request_time, device_time, deviceStatus;
			} systemTags;

			// идентификация устройства
			RequestRTU identityRequest_;
			bool identRequested_;

			// запросы
			RequestContainer requestContainer_;

		protected:
			using ModbusDeviceBase::AddTag;
			using ModbusDeviceBase::WithPrefix;

		public:
			Sirius2Base(ChannelWPtr parent, const string_t& deviceId, const int32_t deviceType, const string_t deviceTypeString, const int32_t serialId, const int32_t modbusId);
			~Sirius2Base() {};

			void DoIdentityRequest(boost::shared_ptr<StreamRTU>& stream);
			void DoDataRequest(boost::shared_ptr<StreamRTU>& stream);

			void SetIdentityRequest(const RequestRTU& request);
			void AddRequest(const RequestRTU& request);

			virtual void Calculate() = 0;
		};
	}
}

#endif // sirius2_base_h__
