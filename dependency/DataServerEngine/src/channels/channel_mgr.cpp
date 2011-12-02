#include "channels/channel_mgr.h"
#include "channels/channel_opc/channel_opc.h"
#include "channels/channel_modbus/channel_modbus_rtu.h"
#include "utils/wxzip.h"

#include <boost/typeof/typeof.hpp>
#include <boost/make_shared.hpp>

namespace Channels
{

	ChannelMgr::ChannelMgr()
	{
	}

	ChannelMgr::~ChannelMgr()
	{
		Clear();
	}

	ChannelPtr ChannelMgr::CreateOPC(const string_t& name, const string_t& host, const string_t& server)
	{
		ChannelPtr channel = boost::make_shared<OPC::ChannelOPC>(name, host, server);
		Attach(channel);
		return channel;
	}

	ChannelPtr ChannelMgr::CreateModbusRtu(boost::shared_ptr<Modbus::StreamRTU> stream, const string_t& name, const int32_t& pause)
	{
		ChannelPtr channel = boost::make_shared<Modbus::ModbusRTU>(stream, name, pause);
		Attach(channel);
		return channel;
	}

	ChannelPtr ChannelMgr::Create(ChannelType type, const Json::Value& jsonValue)
	{
		FRL_EXCEPT_GUARD();

		try
		{
			switch(type)
			{
			case OPC_DA2:
				{
					string_t name = jsonValue["name"].asString();
					string_t host = jsonValue["host"].asString();
					string_t server = jsonValue["server"].asString();

					return CreateOPC(name, host, server);
				}
			case MODBUS_RTU_NPORT:
				{
					string_t name = jsonValue["name"].asString();
					string_t server = jsonValue["server"].asString();
					int32_t port = jsonValue["port"].asInt();
					int32_t speed = jsonValue["speed"].asInt();
					int32_t timeout = jsonValue["timeout"].asInt();
					int32_t pause = jsonValue["pause"].asInt();

					boost::shared_ptr<Modbus::StreamRTU> stream = boost::make_shared<ModbusRTU_NPort>(server, port, speed, timeout);
					return CreateModbusRtu(stream, name, pause);
				}
			case MODBUS_RTU_SERIAL:
				{
					//string_t name = jsonValue["name"].asString();
					//boost::shared_ptr<Modbus::StreamRTU> stream( new ModbusRTU_Serial() );
					//Channel* channelPtr = new Modbus::ModbusRTU(stream, name);
					//Attach(channelPtr);
					//channelPtr->Init(name);
					//return channelPtr;
					FRL_THROW_CLASS( NotSupportedChannelType, "MODBUS_RTU_SERIAL not supported yet." );
					//return NULL;
				}
			default:
				{
					FRL_THROW_CLASS( NotSupportedChannelType, "Type is not recognized." );
				}
			}
		}
		catch(...)
		{
			FRL_THROW_S();
		}
	}

	void ChannelMgr::Attach( ChannelPtr& ptr )
	{
		channels_.push_back(ptr);
		ptr->Bind( boost::bind(&ChannelMgr::OnChannelStateChange, this, _1) );
	}

	void ChannelMgr::Detach(const uint32_t id)
	{
		ChannelContainer::iterator iter = channels_.begin();
		ChannelContainer::iterator end = channels_.end();
		ChannelContainer::iterator toDell = channels_.end();
		for (; iter != end; ++iter)
		{
			if ( (*iter)->GetId() == id )
			{
				toDell = iter;
			}
		}

		if ( toDell != end )
		{
			BOOST_ASSERT(toDell->use_count() == 1);
			channels_.erase(toDell);
		}
	}

	bool ChannelMgr::Exist(const uint32_t id) const
	{
		BOOST_FOREACH(const ChannelPtr& channel, channels_)
		{
			if ( channel->GetId() == id )
			{
				return true;
			}
		}
		return false;
	}

	ChannelPtr ChannelMgr::Get( const uint32_t id )
	{
		BOOST_FOREACH(const ChannelPtr& channel, channels_)
		{
			if ( channel->GetId() == id )
			{
				return channel;
			}
		}

		FRL_THROW_S_CLASS( ChannelNotExist );
	}

	unsigned long ChannelMgr::Count() const
	{
		return channels_.size();
	}

	void ChannelMgr::OnChannelStateChange( const ChannelInfo info )
	{
		onStateChange_(info);
	}

	boost::signals2::connection ChannelMgr::Bind( StateSlot callback )
	{
		return onStateChange_.connect(callback);
	}

	void ChannelMgr::Clear()
	{
		channels_.clear();
		onStateChange_.disconnect_all_slots();
	}

}