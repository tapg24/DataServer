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

		class DeviceBase : boost::noncopyable
		{
			string_t deviceId_;
			std::vector<TagInfo> tags_;

		protected:
			boost::shared_ptr<Channel> parentChannel_;

			string_t CatTagPrefix(const string_t& name);
			void CatTagPrefix(std::vector<string_t>& names);

		public:
			void AddTag(const string_t& tagName, const VARTYPE tagType);
			void AddTag(const TagInfo& tag);
			void AddTags(const TagInfoArray& tags);
			const bool TagExist(const string_t& tagName) const;

		public:
			DeviceBase(boost::shared_ptr<Channel>& parent, const string_t& deviceId);
			virtual ~DeviceBase() {}

			virtual void DoDataRequest(boost::shared_ptr<StreamRTU>& stream) = 0;
			const std::vector<TagInfo> GetTags() const;
		};

		typedef boost::shared_ptr<DeviceBase> Device;
		typedef std::vector<Device> DeviceContainer;
	}
}

#endif // MODBUS_DEVICE_BASE_H