#include "entries/default_entry.h"
#include "core/project_mgr.h"
#include "channels/cache_mgr.h"

#include "../../frl/include/frl_opc.h"

namespace Channels
{
	DefaultEntry::DefaultEntry( const string_t& name, const Json::Value& convert ):
		Entry(name, DefaultEntry_Type)
	{
		for ( size_t idx = 0; idx < convert.size(); ++idx )
		{
			const string_t src = convert[idx]["src"].asString();
			const string_t type_conversion = convert[idx]["type"].asString();
			if ( type_conversion == _STR("direct") )
			{
				SetSrc(src);
			}
		}
	}

	DefaultEntry::~DefaultEntry()
	{

	}

	void DefaultEntry::Update(const string_t& tag_name)
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

	void DefaultEntry::SetSrc( const string_t& src_name )
	{
		src_name_ = src_name;
	}

	std::list<string_t> DefaultEntry::GetSrcList() const
	{
		std::list<string_t> src_list;
		src_list.push_back(src_name_);
		return src_list;
	}

	bool DefaultEntry::IsChanged() const
	{
		return false;
	}

}

