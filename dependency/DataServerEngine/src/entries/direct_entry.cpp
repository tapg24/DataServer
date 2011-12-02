#include "entries/direct_entry.h"
#include "core/project_mgr.h"
#include "channels/cache_mgr.h"

#include "../../frl/include/frl_opc.h"

namespace Channels
{
	DirectEntry::DirectEntry( const string_t& name, const string_t& src ):
		Entry(name, DirectEntry_Type), src_name_(src)
	{
	}

	DirectEntry::~DirectEntry()
	{
	}

	void DirectEntry::Update(const string_t& tag_name)
	{
		frl::opc::address_space::Tag* tag = frl::opc::opcAddressSpace::getInstance().getTag( tag_name );
		if ( !projectMgr::getInstance().GetCacheMgr()->ItemExist(src_name_) )
		{
			tag->setQuality( OPC_QUALITY_BAD );
			return;
		}

		const Channels::CacheItemPtr& src_value = projectMgr::getInstance().GetCacheMgr()->GetItem( src_name_ );
		tag->write( src_value->GetValue().getConstRef() );
		tag->setQuality( src_value->GetQuality() );
		tag->setTimeStamp( src_value->GetTimeStamp() );
	}

	std::list<string_t> DirectEntry::GetSrcList() const
	{
		std::list<string_t> src_list;
		src_list.push_back(src_name_);
		return src_list;
	}

	bool DirectEntry::IsChanged() const
	{
		return false;
	}

}

