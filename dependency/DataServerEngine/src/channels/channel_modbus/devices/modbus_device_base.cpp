#include "channels/channel_modbus/devices/modbus_device_base.h"

#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>

namespace Channels
{
	namespace Modbus
	{
		DeviceBase::DeviceBase( boost::shared_ptr<Channel>& parent, const string_t& deviceId )
			: parentChannel_(parent), deviceId_(deviceId)
		{
		}

		void DeviceBase::AddTag(const string_t& tagName, const VARTYPE tagType)
		{
			AddTag(TagInfo(tagName, tagType));
		}

		void DeviceBase::AddTag( const TagInfo& tag )
		{
			tags_.push_back(tag);
		}

		void DeviceBase::AddTags( const TagInfoArray& tags )
		{
			tags_.insert(tags_.end(), tags.begin(), tags.end());
		}

		const bool DeviceBase::TagExist(const string_t& tagName) const
		{
			BOOST_FOREACH(const TagInfo& tagInfo, tags_)
			{
				if ( tagInfo.name_ == tagName )
				{
					return true;
				}
			}

			return false;
		}

		const std::vector<TagInfo> DeviceBase::GetTags() const
		{
			return tags_;
		}

		string_t DeviceBase::CatTagPrefix( const string_t& name )
		{
			return boost::lexical_cast<std::string>(deviceId_) + "." + name;
		}

		void DeviceBase::CatTagPrefix( std::vector<string_t>& names )
		{
			BOOST_FOREACH(std::string& name, names)
			{
				name = CatTagPrefix(name);
			}
		}

	}

}