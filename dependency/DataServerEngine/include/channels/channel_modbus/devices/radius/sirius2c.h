#ifndef sirius2c_h__
#define sirius2c_h__

#include "utils/types.h"
#include "utils/string.h"
#include "channels/channel_modbus/devices/radius/sirius2_base.h"

namespace Channels
{
	namespace Modbus
	{
		class Sirius2C : public Sirius2Base
		{
			struct ParamTag {
				string_t I, Ia, Ib, Ic;
				string_t DI16;
			} paramTags;

			void Calculate();

		public:
			Sirius2C(ChannelWPtr parent
					, const string_t& deviceId
					, const int32_t deviceType
					, const int32_t serialId
					, const int32_t modbusId
					, const int32_t primeCurrent
					, const int32_t secondCurrent
					);
			~Sirius2C();
		};
	}
}

#endif // sirius2c_h__
