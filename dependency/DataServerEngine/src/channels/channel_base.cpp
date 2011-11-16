#include "channels/channel_base.h"
#include "core/project_mgr.h"
#include "utils/id_mgr.h"
#include <boost/foreach.hpp>

namespace Channels
{

	Channel::Channel( const ChannelType type,  const string_t& name, const uint32_t id /*= 0*/ )
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
			onStateChange_(id_, state_);
		}
	}

	void Channel::AddTag(const string_t& name, const VARTYPE type)
	{
		boost::mutex::scoped_lock lock(itemGuard_);

		projectMgr::getInstance().GetCacheMgr()->InsertItem(name, type);
		itemsName_.push_back(name);

		std::sort( itemsName_.begin(), itemsName_.end() );
		itemsName_.erase(std::unique(itemsName_.begin(), itemsName_.end()), itemsName_.end());
	}

	void Channel::AddTag( const string_t& name, const VARIANT& variant, const WORD quality )
	{
		boost::mutex::scoped_lock lock(itemGuard_);

		projectMgr::getInstance().GetCacheMgr()->InsertItem(name, variant, quality);
		itemsName_.push_back(name);

		std::sort( itemsName_.begin(), itemsName_.end() );
		itemsName_.erase(std::unique(itemsName_.begin(), itemsName_.end()), itemsName_.end());
	}

	void Channel::AddTag( const TagInfo& tag )
	{
		boost::mutex::scoped_lock lock(itemGuard_);

		projectMgr::getInstance().GetCacheMgr()->InsertItem(tag);
		itemsName_.push_back(tag.name_);

		std::sort( itemsName_.begin(), itemsName_.end() );
		itemsName_.erase(std::unique(itemsName_.begin(), itemsName_.end()), itemsName_.end());
	}

	void Channel::AddTags( const TagInfoArray& tags )
	{
		BOOST_FOREACH(const TagInfo& tag, tags)
		{
			AddTag(tag);
		}
	}

	void Channel::ResetCache()
	{
		boost::mutex::scoped_lock lock(itemGuard_);

		BOOST_FOREACH(const string_t& itemName, itemsName_)
		{
			projectMgr::getInstance().GetCacheMgr()->GetItem(itemName)->Reset();
		}

		//projectMgr::getInstance().CacheMgr()->NotifyAll();
	}

	const std::vector<string_t> Channel::GetItemNames() const
	{
		boost::mutex::scoped_lock lock(itemGuard_);

		return itemsName_;
	}

	boost::signals2::connection Channel::Bind( StateSlot callback )
	{
		callback(id_, state_);
		return onStateChange_.connect(callback);
	}

}