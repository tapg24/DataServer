#ifndef MODBUS_DEVICE_FACTORY_H
#define MODBUS_DEVICE_FACTORY_H

#include "utils/string.h"
#include "channels/channel_modbus/devices/modbus_device_base.h"
#include "../../jsoncpp-0.5.0/include/json/json.h"
#include "../../FRL/include/frl_exception.h"

namespace Channels
{
	namespace Modbus
	{
		class DeviceFactory
		{
		public:
			FRL_EXCEPTION_CLASS( TypeNotValid );

			static DevicePtr Create(ChannelWPtr parent, const Json::Value& jsonValue);
		};
	}
}

#endif // MODBUS_DEVICE_FACTORY_H