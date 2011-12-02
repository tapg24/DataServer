#ifndef cache_mgr_fwd_h__
#define cache_mgr_fwd_h__

#include <memory>

namespace Channels
{
	class CacheMgr;
}

typedef std::auto_ptr<Channels::CacheMgr> CacheMgrPtr;

#endif // cache_mgr_fwd_h__
