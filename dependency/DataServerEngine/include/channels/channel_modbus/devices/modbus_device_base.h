#ifndef MODBUS_DEVICE_BASE_H
#define MODBUS_DEVICE_BASE_H

#include "utils/types.h"
#include "utils/string.h"
#include "channels/channel_base.h"
#include "channels/tag_info.h"

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

namespace Channels
{
	namespace Modbus
	{
		class StreamRTU;

		class ModbusDeviceBase : boost::noncopyable
		{
			ChannelWPtr parentChannel_;
			string_t deviceId_;
			std::vector<TagInfo> tags_;

		protected:
			void AddTag(const string_t& name, const VARIANT& variant, const WORD quality);
			string_t WithPrefix(const string_t& name);
			const bool TagExist(const string_t& name) const;
			ChannelPtr GetChannel();

		public:
			ModbusDeviceBase(ChannelWPtr& parent, const string_t& deviceId);
			virtual ~ModbusDeviceBase() {}

			virtual void DoDataRequest(boost::shared_ptr<StreamRTU>& stream) = 0;
		};

		typedef boost::shared_ptr<ModbusDeviceBase> DevicePtr;
	}
}

#endif // MODBUS_DEVICE_BASE_H