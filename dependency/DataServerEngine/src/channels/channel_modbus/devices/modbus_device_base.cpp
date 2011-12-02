#include "channels/channel_modbus/devices/modbus_device_base.h"

#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>

namespace Channels
{
	namespace Modbus
	{
		ModbusDeviceBase::ModbusDeviceBase( ChannelWPtr& parent, const string_t& deviceId )
			: parentChannel_(parent), deviceId_(deviceId)
		{
			BOOST_ASSERT( !parentChannel_.expired() );
		}

		void ModbusDeviceBase::AddTag( const string_t& name, const VARIANT& variant, const WORD quality )
		{
			BOOST_ASSERT( !parentChannel_.expired() );
			ChannelPtr parentChannel = parentChannel_.lock();
			tags_.push_back(TagInfo(name, variant.vt));
			parentChannel->AddTag(name, variant, quality);
		}

		const bool ModbusDeviceBase::TagExist(const string_t& name) const
		{
			BOOST_FOREACH(const TagInfo& tagInfo, tags_)
			{
				if ( tagInfo.name_ == name )
				{
					return true;
				}
			}

			return false;
		}

		string_t ModbusDeviceBase::WithPrefix( const string_t& name )
		{
			return boost::lexical_cast<std::string>(deviceId_) + "." + name;
		}

		Channels::ChannelPtr ModbusDeviceBase::GetChannel()
		{
			return parentChannel_.lock();
		}

	}

}