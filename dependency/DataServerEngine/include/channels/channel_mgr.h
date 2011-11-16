#ifndef CHANNEL_MGR_H
#define CHANNEL_MGR_H

#include "channel_base.h"
#include "utils/string.h"

#include <boost/ptr_container/ptr_vector.hpp>
#include "../../jsoncpp-0.5.0/include/json/json.h"

namespace Channels
{

	class ChannelMgr
	{
		//typedefs
		//typedef boost::ptr_vector<Channel> ChannelContainer;
		typedef boost::ptr_vector<Channel> ChannelContainer;
		
		// members
		ChannelContainer channels_;
		StateSignal onStateChange_;

	protected:
		void OnChannelStateChange(const uint32_t id, const ChannelState state);

	public:
		ChannelMgr();
		~ChannelMgr();

		Channel* Create(ChannelType type, const Json::Value& jsonValue);
		Channel* CreateOPC(const string_t& name, const string_t& host, const string_t& server);

		boost::signals2::connection Bind(StateSlot callback);

		void Attach(Channel* ptr);
		void Detach(const unsigned long id);
		bool Exist(const unsigned long id) const;
		Channel* Get(const unsigned long id);
		Channel* ChannelMgr::GetElseNull(const unsigned long id);
		unsigned long Count() const;

	private:
		// functor
		struct ChannelEquals
		{
			ChannelEquals(const unsigned long id) : m_id(id)
			{}

			bool operator() (const Channel* const channel) const
			{
				return m_id == channel->GetId();
			}

		private:
			const unsigned long m_id;
		};
	};

}

#endif // CHANNEL_MGR_H