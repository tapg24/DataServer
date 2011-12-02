#include "channels/channel_modbus/devices/modbus_device_factory.h"

// radius
// sirius
#include "channels/channel_modbus/devices/radius/sirius2c.h"
#include "channels/channel_modbus/devices/radius/sirius2tn.h"
#include "channels/channel_modbus/devices/radius/sirius2yv.h"
#include "channels/channel_modbus/devices/radius/sirius2v.h"
#include "channels/channel_modbus/devices/radius/sirius2ml.h"
#include "channels/channel_modbus/devices/radius/sirius2d.h"

#include "utils/types.h"
#include "utils/wxzip.h"
#include "utils/path.h"

#include <boost/lexical_cast.hpp>
#include <boost/make_shared.hpp>

namespace Channels
{
	namespace Modbus
	{
		DevicePtr DeviceFactory::Create( ChannelWPtr parent, const Json::Value& jsonValue )
		{
			string_t dump = jsonValue.toStyledString();

			const string_t vendor = jsonValue["vendor"].asString();
			const int32_t type = jsonValue["type"].asInt();
			const string_t deviceId = jsonValue["device_id"].asString();
			const int32_t serialId = jsonValue["serial_id"].asInt();
			const int32_t modbusId = jsonValue["modbus_id"].asInt();

			if ( vendor == "radius" )
			{
				if ( type == 107 ) // Сириус-2-ТН
				{
					const int32_t primeVoltage = jsonValue["prime_voltage"].asInt();
					const int32_t secondVoltage = jsonValue["second_voltage"].asInt();
					return boost::make_shared<Sirius2TN>(parent, deviceId, type, serialId, modbusId, primeVoltage, secondVoltage);
				}
				else if ( type == 93 ) // Сириус-2-УВ
				{
					const int32_t primeVoltage = jsonValue["prime_voltage"].asInt();
					const int32_t secondVoltage = jsonValue["second_voltage"].asInt();
					const int32_t primeCurrent = jsonValue["prime_current"].asInt();
					const int32_t secondCurrent = jsonValue["second_current"].asInt();
					return boost::make_shared<Sirius2YV>(parent, deviceId, type, serialId, modbusId, primeCurrent, secondCurrent, primeVoltage, secondVoltage);
				}
				else if ( type == 99 ) // Сириус-2-В
				{
					const int32_t primeVoltage = jsonValue["prime_voltage"].asInt();
					const int32_t secondVoltage = jsonValue["second_voltage"].asInt();
					const int32_t primeCurrent = jsonValue["prime_current"].asInt();
					const int32_t secondCurrent = jsonValue["second_current"].asInt();
					return boost::make_shared<Sirius2V>(parent, deviceId, type, serialId, modbusId, primeCurrent, secondCurrent, primeVoltage, secondVoltage);
				}
				else if ( type == 98 ) // Сириус-2-С
				{
					const int32_t primeCurrent = jsonValue["prime_current"].asInt();
					const int32_t secondCurrent = jsonValue["second_current"].asInt();
					return boost::make_shared<Sirius2C>(parent, deviceId, type, serialId, modbusId, primeCurrent, secondCurrent);
				}
				else if ( type == 100 ) // Сириус-2-МЛ
				{
					const int32_t primeVoltage = jsonValue["prime_voltage"].asInt();
					const int32_t secondVoltage = jsonValue["second_voltage"].asInt();
					const int32_t primeCurrent = jsonValue["prime_current"].asInt();
					const int32_t secondCurrent = jsonValue["second_current"].asInt();
					return boost::make_shared<Sirius2ML>(parent, deviceId, type, serialId, modbusId, primeCurrent, secondCurrent, primeVoltage, secondVoltage);
				}
				else if ( type == 102 ) // Сириус-2-Д
				{
					const int32_t primeVoltage = jsonValue["prime_voltage"].asInt();
					const int32_t secondVoltage = jsonValue["second_voltage"].asInt();
					const int32_t primeCurrent = jsonValue["prime_current"].asInt();
					const int32_t secondCurrent = jsonValue["second_current"].asInt();
					return boost::make_shared<Sirius2D>(parent, deviceId, type, serialId, modbusId, primeCurrent, secondCurrent, primeVoltage, secondVoltage);
				}
				else
				{
					FRL_THROW_CLASS(TypeNotValid, boost::lexical_cast<string_t>(type));
				}
			}
		}
	}
}