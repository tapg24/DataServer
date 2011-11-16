#include "channels/channel_mgr.h"
#include "channels/channel_opc/channel_opc.h"
#include "channels/channel_modbus/channel_modbus_rtu.h"
#include "utils/wxzip.h"

namespace Channels
{

	ChannelMgr::ChannelMgr()
	{
	}

	ChannelMgr::~ChannelMgr()
	{
		channels_.release();
		onStateChange_.disconnect_all_slots();
	}

	Channel* ChannelMgr::CreateOPC(const string_t& name, const string_t& host, const string_t& server)
	{
		Channel* channel = new OPC::ChannelOPC(name, host, server);
		Attach(channel);

		Channels::OPC::ChannelOPC* opc = static_cast<Channels::OPC::ChannelOPC*>(channel);

		return channel;
	}

	Channel* ChannelMgr::Create(ChannelType type, const Json::Value& jsonValue)
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
					int port = jsonValue["port"].asInt();
					int speed = jsonValue["speed"].asInt();
					int timeout = jsonValue["timeout"].asInt();
					int pause = jsonValue["pause"].asInt();

					boost::shared_ptr<Modbus::StreamRTU> stream( new ModbusRTU_NPort(server, port, speed, timeout) );
					Channel* channel = new Modbus::ModbusRTU(stream, name, pause);
					Attach(channel);
					return channel;
				}
			case MODBUS_RTU_SERIAL:
				{
					//string_t name = jsonValue["name"].asString();
					//boost::shared_ptr<Modbus::StreamRTU> stream( new ModbusRTU_Serial() );
					//Channel* channelPtr = new Modbus::ModbusRTU(stream, name);
					//Attach(channelPtr);
					//channelPtr->Init(name);
					//return channelPtr;
					return NULL;
				}
			default:
				{
					BOOST_ASSERT(false);
					return NULL;
				}
			}
		}
		catch(...)
		{
			FRL_THROW_S();
		}
	}

	void ChannelMgr::Attach( Channel* ptr )
	{
		channels_.push_back(ptr);
		ptr->Bind( boost::bind(&ChannelMgr::OnChannelStateChange, this, _1, _2) );
	}

	void ChannelMgr::Detach(const unsigned long id)
	{
		ChannelContainer::iterator iter = channels_.begin();
		ChannelContainer::iterator end = channels_.end();
		ChannelContainer::iterator toDell = channels_.end();
		for (; iter != end; ++iter)
		{
			if ( iter->GetId() == id )
			{
				toDell = iter;
			}
		}

		if ( toDell != end )
		{
			channels_.release(toDell);
		}
	}

	bool ChannelMgr::Exist(const unsigned long id) const
	{
		for (ChannelContainer::const_iterator iter = channels_.begin(); iter != channels_.end(); ++iter)
		{
			if ( iter->GetId() == id )
			{
				return true; 
			}
		}
		return false;
	}

	Channel* ChannelMgr::Get( const unsigned long id )
	{
		for (ChannelContainer::iterator iter = channels_.begin(); iter != channels_.end(); ++iter)
		{
			if ( iter->GetId() == id )
			{
				return iter->GetPtr(); 
			}
		}

		BOOST_ASSERT(false);
		return NULL;
	}

	Channel* ChannelMgr::GetElseNull( const unsigned long id )
	{
		for (ChannelContainer::iterator iter = channels_.begin(); iter != channels_.end(); ++iter)
		{
			if ( iter->GetId() == id )
			{
				return iter->GetPtr(); 
			}
		}
		return NULL;
	}

	unsigned long ChannelMgr::Count() const
	{
		return channels_.size();
	}

	void ChannelMgr::OnChannelStateChange( const uint32_t id, const ChannelState state )
	{
		onStateChange_(id, state);
	}

	boost::signals2::connection ChannelMgr::Bind( StateSlot callback )
	{
		return onStateChange_.connect(callback);
	}

}