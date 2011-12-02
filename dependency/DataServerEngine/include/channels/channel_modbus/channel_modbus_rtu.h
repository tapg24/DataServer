#ifndef CHANNEL_MODBUS_RTU
#define CHANNEL_MODBUS_RTU

#include "channels/channel_base.h"

#include "modbus_rtu_nport.h"
//#include "modbus_rtu_serial.h"
#include "devices/modbus_request.h"
#include "devices/modbus_device_base.h"
#include "devices/modbus_device_factory.h"
#include "channels/cache_mgr_fwd.h"
#include "utils/timer.hpp"
#include "utils/event.h"

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

namespace Channels
{
	class ChannelMgr;

	namespace Modbus
	{
		class StreamRTU;

		class ModbusRTU : public Channel
		{
			friend class ChannelMgr;
			typedef std::vector<DevicePtr> DeviceContainer;

			boost::thread streamThread_;
			bool streamThreadStoped_;
			Event stopStreamThread_;
			boost::shared_ptr<StreamRTU> stream_;
			DeviceContainer devices_;
			bool connectToConverter_;
			bool converterAlive_;
			bool firstStart;
			CXTimer cyclePerformance;
			int32_t pauseDuration_ms;
			bool notifyAfterRequest;

			struct SystemTags
			{
				std::string cycleTime;
				std::string channelAlive;
			} systemTags;

			std::vector<TagInfo> tagInfoArray_;

		public:
			ModbusRTU(const boost::shared_ptr<StreamRTU> modbusStream,  const std::string& name, const int pause);
			~ModbusRTU();

			//////////////////////////////////////////////////////////////////////////
			// virtual base. override
			//////////////////////////////////////////////////////////////////////////

			// запуск канала на сбор
			void Start();
			// остановка сбора канала
			void Stop();

			//////////////////////////////////////////////////////////////////////////
			// this channel interface
			//////////////////////////////////////////////////////////////////////////
			
			void AddDevice( const DevicePtr& device );

		private:
			void WatchThread();
			void StopWatchThread();
			void StreamThread();
			void StopStreamThread();
		};

		typedef boost::shared_ptr<ModbusRTU> ModbusRTUSPtr;
		typedef boost::weak_ptr<ModbusRTU> ModbusRTUWPtr;
	}
}

template <typename T>
inline T accuracy(T value, int acc)
{
	return floor( value * acc ) / acc;
}

#endif // CHANNEL_MODBUS_RTU