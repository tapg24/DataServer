#ifndef sirius2tn_h__
#define sirius2tn_h__

#include "utils/types.h"
#include "utils/string.h"
#include "channels/channel_modbus/devices/radius/sirius2_base.h"

namespace Channels
{
	namespace Modbus
	{
		class Sirius2TN : public Sirius2Base
		{
			struct ParamTag {
				string_t f, U, Ua, Ub, Uc;
			} paramTags;

			void Calculate();

		public:
			Sirius2TN(ChannelWPtr parent
				, const string_t& deviceId
				, const int32_t deviceType
				, const int32_t serialId
				, const int32_t modbusId
				, const int32_t primeVoltage
				, const int32_t secondVoltage
				);
			~Sirius2TN();
		};
	}
}

#endif // sirius2tn_h__
