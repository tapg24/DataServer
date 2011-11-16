#ifndef SIRIUS_2_OLD_H
#define SIRIUS_2_OLD_H

#include "utils/types.h"
#include "utils/string.h"
#include "channels/channel_modbus/devices/modbus_device_base.h"
#include "channels/channel_modbus/devices//modbus_request.h"

#include <boost/circular_buffer.hpp>

namespace Channels
{
	namespace Modbus
	{
		class Sirius2Old : public DeviceBase
		{
			// информация об устройстве
			string_t deviceId_;
			int32_t deviceType_, serialId_, modbusId_;
			bool passCheck_;
			int32_t passAccessDuration_;
			bool calculateParam_;
			int32_t deviceStatus_;
			int32_t checkCount_;

			struct SystemTagsType
			{
				string_t request_time, device_time, deviceStatus;
			} systemTags;

			struct ParamTagTypes
			{
				string_t I, Ia, Iax, Iay, Ib, Ibx, Iby, Ic, Icx, Icy;
				string_t U, Ua, Uax, Uay, Ub, Ubx, Uby, Uc, Ucx, Ucy;
				string_t P, Q;
			} paramTags;

			// усреднение
			boost::circular_buffer<float> circularBuf_P;
			boost::circular_buffer<float> circularBuf_Q;

			// идентификация устройства
			RequestRTU identityRequest_;
			bool identRequested_;

			RequestContainer requestContainer_;

  			string_t SetTagPrefix(const string_t& tagName);
			void SetTagPrefix(std::vector<string_t>& tagNameArray);
			// Определяем зарегистрированы ли все значения, для вычесления полной, активной и реактивной мощностей
			void InitParamTags();
			void InitDeviceTypeTN();
			void InitDeviceTypeC();
			void InitDeviceTypeOthers();
			void CalcParamTags();
			void CalcDeviceTypeTN();
			void CalcDeviceTypeC();
			void CalcDeviceTypeOthers();
			void DoIdentityRequest(boost::shared_ptr<StreamRTU>& stream);
			void DoDataRequest(boost::shared_ptr<StreamRTU>& stream);

		public:
			Sirius2Old(boost::shared_ptr<Channel>& parent, const string_t& deviceId, const int32_t deviceType, const int32_t serialId, const int32_t modbusId, const bool calc);
			~Sirius2Old();

			void SetIdentityRequest(const RequestRTU& request);
			void AddRequest(const RequestRTU& request);

		};
	}
}

#endif // SIRIUS_2_OLD_H