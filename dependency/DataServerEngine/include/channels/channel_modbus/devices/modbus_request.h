#ifndef MODBUS_REQUEST_H
#define MODBUS_REQUEST_H

#include "utils/types.h"
#include "channels/tag_info.h"
#include <boost/shared_ptr.hpp>

namespace Channels
{
	namespace Modbus
	{
		struct RequestRTU
		{
			int32_t station;
			int32_t startAdress;
			int32_t func;
			int32_t byteCount;

			bool reqiredPassword;
			int32_t passwordAdress;
			int32_t password;

			std::vector<TagInfo> tags;
		};

		typedef std::vector<RequestRTU> RequestContainer;

		class StreamRTU;
		class ModbusRequest
		{
		public:
			static void DoRequest(const RequestRTU& request, boost::shared_ptr<StreamRTU>& stream);
			static void ProcessData(const RequestRTU& request, boost::shared_ptr<StreamRTU>& stream, const bool swapEndian = false);
		};
	}
}

#endif // MODBUS_REQUEST_H