#ifndef channel_mgr_fwd_h__
#define channel_mgr_fwd_h__

#include <memory>

namespace Channels
{
	class ChannelMgr;
}

typedef std::auto_ptr<Channels::ChannelMgr> ChannelMgrPtr;

#endif // channel_mgr_fwd_h__
