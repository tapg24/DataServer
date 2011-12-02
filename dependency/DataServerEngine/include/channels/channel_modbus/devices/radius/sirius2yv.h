#ifndef sirius2yv_h__
#define sirius2yv_h__

#include "utils/types.h"
#include "utils/string.h"
#include "channels/channel_modbus/devices/radius/sirius2_base.h"

#include <boost/circular_buffer.hpp>

namespace Channels
{
	namespace Modbus
	{
		class Sirius2YV : public Sirius2Base
		{
			struct ParamTag {
				string_t U, Ua, Ub, Uc, I, Ia, Ib, Ic, P, Q;
				string_t Iax, Iay, Ibx, Iby, Icx, Icy, I2x, I2y, Uax, Uay, Ubx, Uby, Ucx, Ucy;
				string_t DI16;
			} paramTags;

			boost::circular_buffer<float> circularBuf_P, circularBuf_Q;

			void Calculate();

		public:
			Sirius2YV(ChannelWPtr parent
				, const string_t& deviceId
				, const int32_t deviceType
				, const int32_t serialId
				, const int32_t modbusId
				, const int32_t primeCurrent
				, const int32_t secondCurrent
				, const int32_t primeVoltage
				, const int32_t secondVoltage
				);
			~Sirius2YV() {};
		};
	}
}

#endif // sirius2yv_h__
