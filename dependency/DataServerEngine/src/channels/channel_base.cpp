#include "channels/channel_base.h"
#include "channels/cache_mgr.h"
#include "core/project_mgr.h"
#include "utils/id_mgr.h"
#include <boost/foreach.hpp>

namespace Channels
{

	Channel::Channel( const ChannelType type,  const string_t& name )
		: type_(type), name_(name), state_(Created)
	{
		id_ = idGenerator::getInstance().Generate();
	}

	Channel::~Channel()
	{
		ResetCache();
		onStateChange_.disconnect_all_slots();
	}

	bool Channel::operator==(const Channel &other) const
	{
		return GetId() == other.GetId();
	}

	string_t Channel::GetName() const
	{
		return name_;
	}

	ChannelState Channel::GetState() const
	{
		return state_;
	}

	const uint32_t Channel::GetId() const
	{
		return id_;
	}

	const ChannelType Channel::GetType() const
	{
		return type_;
	}

	Channel* Channel::GetPtr()
	{
		return this;
	}

	void Channel::ReName( const string_t& name )
	{
		if ( name_ != name )
		{
			name_ = name;
			StateChanged(Renamed);
		}
	}

	void Channel::StateChanged( const ChannelState& state )
	{
		if ( state != state_ )
		{
			state_ = state;
			onStateChange_( ChannelInfo(id_, name_, type_, state_) );
		}
	}

	void Channel::AddTag( const string_t& name, const VARIANT& variant, const WORD quality )
	{
		boost::mutex::scoped_lock lock(itemGuard_);


		if ( !projectMgr::getInstance().GetCacheMgr()->ItemExist(name) )
		{
			projectMgr::getInstance().GetCacheMgr()->InsertItem(name, variant, quality);
			itemsName_.push_back(name);
		}
	}

	void Channel::ResetCache()
	{
		boost::mutex::scoped_lock lock(itemGuard_);

		BOOST_FOREACH(const string_t& itemName, itemsName_)
		{
			projectMgr::getInstance().GetCacheMgr()->GetItem(itemName)->Reset();
		}
	}

	const std::vector<string_t> Channel::GetTagNames() const
	{
		boost::mutex::scoped_lock lock(itemGuard_);

		return itemsName_;
	}

	boost::signals2::connection Channel::Bind( StateSlot callback )
	{
		callback( ChannelInfo(id_, name_, type_, state_) );
		return onStateChange_.connect(callback);
	}

}