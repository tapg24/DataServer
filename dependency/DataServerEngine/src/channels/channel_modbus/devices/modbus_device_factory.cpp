#include "channels/channel_modbus/devices/modbus_device_factory.h"
#include "channels/channel_modbus/devices/modbus_device_def.h"
#include "channels/channel_modbus/devices/generic_device.h"
#include "channels/channel_modbus/devices/radius/sirius2old_device.h"
#include "channels/channel_modbus/devices/radius/sirius2_device.h"

#include "utils/wxzip.h"
#include "utils/path.h"

namespace Channels
{
	namespace Modbus
	{
		Device CreateDevice( boost::shared_ptr<Channel>& parent, const Json::Value& jsonValue )
		{
			//string_t dump = jsonValue.toStyledString();

			Json::Reader reader;

			boost::filesystem::path template_path = Util::GetModulePath() / "templates.dst";
			string_t deviceInfo = Util::Zip::GetEntry(template_path.string(), jsonValue["template"].asString());
			Json::Value jsonDevice = jsonValue;
			Json::Value jsonDevicesInfo;
			if (reader.parse(deviceInfo, jsonDevicesInfo))
			{
				jsonDevice["device_info"] = jsonDevicesInfo;
			}

			const int family = jsonDevice["device_info"]["family"].asInt();
			switch ( family )
			{
			case Devices::Sirius2Old :
				{
					const string_t deviceId = jsonDevice["device_id"].asString();
					const int serialId = jsonDevice["serial_id"].asInt();
					const int modbusId = jsonDevice["modbus_id"].asInt();
					const int type = jsonDevice["device_info"]["type"].asInt();
					const bool calc = jsonDevice["device_info"]["calculate_param"].asBool();
					// теги, которые использует устройство
					TagInfoArray deviceTags;

					boost::shared_ptr<Sirius2Old> sirius( new Sirius2Old(parent, deviceId, type, serialId, modbusId, calc) );

					// запрос идентификации
					RequestRTU identRequest;
					identRequest.station = modbusId;
					identRequest.func = jsonDevice["device_info"]["identity_request"]["function"].asInt();
					identRequest.startAdress = jsonDevice["device_info"]["identity_request"]["address"].asInt();
					identRequest.byteCount = jsonDevice["device_info"]["identity_request"]["byte_count"].asInt();

					const Json::Value& identTags = jsonDevice["device_info"]["identity_request"]["tags"];
					for ( size_t idx = 0; idx < identTags.size(); ++idx )
					{
						//string_t dump = identTags[idx].toStyledString();

						string_t name = deviceId + "." + identTags[idx]["name"].asString();
						TagInfo tag(name, identTags[idx]["type"].asInt());

						identRequest.tags.push_back(tag);
						deviceTags.push_back(tag);
					}
					sirius->SetIdentityRequest(identRequest);

					// запрос данных
					const Json::Value& requests = jsonDevice["device_info"]["requests"];
					for ( size_t idx = 0; idx < requests.size(); ++idx )
					{
						RequestRTU request;
						request.station = modbusId;
						request.func = requests[idx]["function"].asInt();
						request.startAdress = requests[idx]["address"].asInt();
						request.byteCount = requests[idx]["byte_count"].asInt();

						const Json::Value& tags = requests[idx]["tags"];
						for ( size_t idx = 0; idx < tags.size(); ++idx )
						{
							//const string_t dump = tags[idx].toStyledString();

							string_t name = deviceId + "." + tags[idx]["name"].asString();
							TagInfo tag(name, tags[idx]["type"].asInt());

							request.tags.push_back(tag);
							deviceTags.push_back(tag);
						}

						sirius->AddRequest(request);
					}

					// теги, которые необходимо расчитать
					const Json::Value& calc_tags = jsonDevice["device_info"]["calculated_tags"];
					for ( size_t idx = 0; idx < calc_tags.size(); ++idx )
					{
						string_t name = deviceId + "." + calc_tags[idx]["name"].asString();
						TagInfo tag(name, calc_tags[idx]["type"].asInt());
						deviceTags.push_back(tag);
					}

					sirius->AddTags(deviceTags);

					return sirius;
				}
			//case Devices::Sirius2 :
			//	return Device(new Sirius2());
			default:
				return Device(new GenericDevice(parent, ""));
			}
		}

	}
}