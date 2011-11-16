#ifndef CHANNEL_BASE_H
#define CHANNEL_BASE_H

#include "utils/types.h"
#include "utils/string.h"

#include <vector>
#include <boost/thread.hpp>
#include <boost/signals2.hpp>

#include "cache_mgr.h"

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

	// forward declaration
	class Channel;

	// typedefs
	typedef boost::signals2::signal< void(const uint32_t, const ChannelState) > StateSignal;
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
		Channel(const ChannelType type, const string_t& name, const unsigned long id = 0);
		void StateChanged(const ChannelState& state);

		void AddTag(const string_t& name, const VARTYPE type);
		void AddTag(const string_t& name, const VARIANT& variant, const WORD quality);
		void AddTag(const TagInfo& tag);
		void AddTags(const TagInfoArray& tags);
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
		const std::vector<string_t> GetItemNames() const;

		boost::signals2::connection Bind(StateSlot callback);

		// channel interface
		virtual void Start() = 0;
		virtual void Stop() = 0;
	};

	typedef boost::shared_ptr<Channel> ChannelPtr;
	//typedef std::vector<ChannelPtr> 
}

#endif // CHANNEL_BASE_H