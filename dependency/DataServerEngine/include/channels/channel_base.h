#ifndef CHANNEL_BASE_H
#define CHANNEL_BASE_H

#include "utils/types.h"
#include "utils/string.h"
#include "channels/cache_mgr_fwd.h"

#include <OaIdl.h>
#include <wtypes.h>

#include <vector>
#include <boost/thread.hpp>
#include <boost/signals2.hpp>

namespace Channels
{
	enum ChannelType
	{
		OPC_DA2 = 0,
		MODBUS_RTU_SERIAL,
		MODBUS_RTU_NPORT,
		MODBUS_RTU_TCP,
		MODBUS_ASCII,
		MODBUS_TCP
	};

	enum ChannelState
	{
		Created = 0,
		Deleted,
		Disconnected,
		Connected,
		InProgress,
		Renamed
	};

	struct ChannelInfo
	{
		uint32_t id;
		string_t name;
		ChannelType type;
		ChannelState state;

		ChannelInfo(uint32_t idChannel, string_t nameChannel, ChannelType typeChannel, ChannelState stateChannel)
			: id(idChannel), name(nameChannel), type(typeChannel), state(stateChannel)
		{}
	};

	// forward declaration
	class Channel;

	// typedefs
	typedef boost::signals2::signal< void(const ChannelInfo) > StateSignal;
	typedef StateSignal::slot_type StateSlot;

	class Channel
	{
		// members
		//bool initialized;
		ChannelState state_;
		ChannelType type_;
		uint32_t id_;
		string_t name_;
		std::vector<string_t> itemsName_;
		StateSignal onStateChange_;
		mutable boost::mutex itemGuard_;

	protected:
		Channel(const ChannelType type, const string_t& name);
		void StateChanged(const ChannelState& state);
		void ResetCache();

	public:
		virtual ~Channel();
		bool operator==(const Channel &other) const;

		// base interface
		const uint32_t GetId() const;
		const ChannelType GetType() const;
		Channel* GetPtr();
		void ReName( const string_t& name );
		string_t GetName() const;
		ChannelState GetState() const;
		void AddTag( const string_t& name, const VARIANT& variant, const WORD quality );
		const std::vector<string_t> GetTagNames() const;

		boost::signals2::connection Bind(StateSlot callback);

		// channel interface
		virtual void Start() = 0;
		virtual void Stop() = 0;
	};

	typedef boost::shared_ptr<Channel> ChannelPtr;
	typedef boost::weak_ptr<Channel> ChannelWPtr;
}

#endif // CHANNEL_BASE_H