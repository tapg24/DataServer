#ifndef CHANNEL_MGR_H
#define CHANNEL_MGR_H

#include "utils/types.h"
#include "utils/string.h"
#include "channel_base.h"
#include "channel_modbus/modbus_rtu_stream_fwd.h"

#include "../../jsoncpp-0.5.0/include/json/json.h"
#include "../../frl/include/frl_exception.h"

#include <map>
#include <boost/shared_ptr.hpp>

namespace Channels
{

	class ChannelMgr
	{
		//typedefs
		typedef std::vector<ChannelPtr> ChannelContainer;
		
		// members
		ChannelContainer channels_;
		StateSignal onStateChange_;

	protected:
		void OnChannelStateChange(const ChannelInfo info);

	public:
		FRL_EXCEPTION_CLASS( NotSupportedChannelType );
		FRL_EXCEPTION_CLASS( ChannelNotExist );

		ChannelMgr();
		~ChannelMgr();

		ChannelPtr Create(ChannelType type, const Json::Value& jsonValue);
		ChannelPtr CreateOPC(const string_t& name, const string_t& host, const string_t& server);
		ChannelPtr CreateModbusRtu(boost::shared_ptr<Modbus::StreamRTU> stream, const string_t& name, const int32_t& pause);

		boost::signals2::connection Bind(StateSlot callback);

		void Attach(ChannelPtr& ptr);
		void Detach(const uint32_t id);
		bool Exist(const uint32_t id) const;
		ChannelPtr Get(const uint32_t id);
		unsigned long Count() const;
		void Clear();

	private:
		// functor
		struct ChannelEquals
		{
			ChannelEquals(const unsigned long id) : m_id(id)
			{}

			bool operator() (const ChannelPtr const channel) const
			{
				return m_id == channel->GetId();
			}

		private:
			const unsigned long m_id;
		};
	};

}

#endif // CHANNEL_MGR_H