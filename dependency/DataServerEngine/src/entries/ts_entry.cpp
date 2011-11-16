#include "entries/ts_entry.h"
#include "entries/ts_def.h"
#include "core/project_mgr.h"
#include "core/bit_checker.h"
#include "channels/cache_mgr.h"
#include "utils/types.h"

#include <boost/typeof/typeof.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>

#include "../../frl/include/frl_opc.h"

namespace Channels
{
	EntryTS::EntryTS( const string_t& ts_name, const Json::Value& convert )
		: Entry(ts_name, TS_Type)
	{
		cmd_name_ = ts_name + "_CMD";

		if ( !convert.isNull() )
		{
			for ( size_t idx = 0; idx < convert.size(); ++idx )
			{
				const string_t src = convert[idx]["src"].asString();
				const string_t type_conversion = convert[idx]["type"].asString();
				if ( type_conversion == _STR("bit") )
				{
					const Json::Value states = convert[idx]["states"];
					for ( size_t idx = 0; idx < states.size(); ++idx )
					{
						std::list<int8_t> set;
						const Json::Value bit_set = states[idx]["set"];
						for ( size_t idx = 0; idx < bit_set.size(); ++idx )
						{
							set.push_back( bit_set[idx].asInt() );
						}

						std::list<int8_t> unset;
						const Json::Value bit_unset = states[idx]["unset"];
						for ( size_t idx = 0; idx < bit_unset.size(); ++idx )
						{
							unset.push_back( bit_unset[idx].asInt() );
						}

						string_t stateName = states[idx]["state"].asString();

						StateCheckerPtr checker( new BitChecker(stateName, set, unset) );
						AddSrcChecker( src, checker );
					}
				}
			}
		}
	}

	EntryTS::~EntryTS()
	{
		src_checkers_map_.clear();
	}

	bool EntryTS::IsChanged() const
	{
		return true;
	}

	std::list<string_t> EntryTS::GetSrcList() const
	{
		std::list<string_t> src_list;
		src_list.push_back(cmd_name_);

		BOOST_AUTO(src_iter, src_checkers_map_.begin());
		BOOST_AUTO(src_end, src_checkers_map_.end());
		for (; src_iter != src_end; ++src_iter)
		{
			src_list.push_back(src_iter->first);
		}

		return src_list;
	}

	void EntryTS::Update(const string_t& tag_name)
	{
		if ( boost::ends_with(tag_name, "_TS") )
		{
			frl::opc::address_space::Tag* ts_tag = frl::opc::opcAddressSpace::getInstance().getTag( tag_name );
			uint16_t new_state = ExecuteConvert( ts_tag->read() );
			ts_tag->write( new_state );
		}
		else // boost::ends_with(tag_name, "_CMD")
		{
			frl::opc::address_space::Tag* cmd_tag = frl::opc::opcAddressSpace::getInstance().getTag( tag_name );
			frl::opc::address_space::Tag* ts_tag = frl::opc::opcAddressSpace::getInstance().getTag( name_ );
			uint16_t new_state = ExecuteCmd( cmd_tag->read(), ts_tag->read() );
			ts_tag->write( new_state );
			cmd_tag->write( (uint16_t)0 );
		}
	}

	uint16_t EntryTS::ExecuteConvert( uint16_t state )
	{
		uint16_t prev_state = state;

		if ( state & TS_STATE::MANUAL ) return state;

		BOOST_AUTO( iter, src_checkers_map_.begin() );
		BOOST_AUTO( end, src_checkers_map_.end() );
		for (; iter != end; ++iter )
		{
			size_t hit_count = 0;

			if ( projectMgr::getInstance().GetCacheMgr()->ItemExist(iter->first) )
			{
				const Channels::CacheItemPtr& src_value = projectMgr::getInstance().GetCacheMgr()->GetItem( iter->first );
				ComVariant variant = src_value->GetValue();
				variant.setType( VT_UI4 );
				uint32_t state_value = variant.AsULong();

				BOOST_AUTO(checkers, iter->second);
				BOOST_FOREACH( StateCheckerPtr checker, checkers )
				{
					if ( checker->Check(state_value) )
					{
						++hit_count;
						state &= ~0xFF;
						state |= TS_STATE_HOLDER::GET_VALUE(checker->GetStateName());
					}
				}
			}

			if ( hit_count != 1 )
			{
				state &= ~0xFF;
				state |= TS_STATE::BAD;
			}
		}

		if ( (state & 0xFF) != (prev_state & 0xFF) )
		{
			state |= TS_STATE::CHANGED;
		}

		return state;
	}

	uint16_t EntryTS::ExecuteCmd( uint16_t cmd, uint16_t state )
	{
		if ( cmd == TS_CMD::SET_AUTO ) // первести в автоматический режим
		{
			if ( state & TS_STATE::AUTO ) return state;

			state |= TS_STATE::AUTO;
			state &= ~TS_STATE::MANUAL;
			
			state = ExecuteConvert(state);

		}
		else if ( cmd == TS_CMD::SET_MANUAL ) // первести в ручной режим
		{
			if ( state & TS_STATE::MANUAL ) return state;

			state |= TS_STATE::MANUAL;
			state &= ~TS_STATE::AUTO;

			if ( state & TS_STATE::BAD )
			{
				state &= ~0xFF;
				state |= TS_STATE::AMBIGUITY;
			}
		}
		else if ( cmd == TS_CMD::SET_ACKN_ED )
		{
			state &= ~TS_STATE::ACKN_ED;
			state &= ~TS_STATE::CHANGED;
		}
		else if ( cmd == TS_CMD::SET_RACKED )
		{
			if ( state & TS_STATE::AUTO ) return state;

			state &= ~0xFF;
			state |= TS_STATE::RACKED;
		}
		else if ( cmd == TS_CMD::SET_AMBIGUITY )
		{
			if ( state & TS_STATE::AUTO ) return state;

			state &= ~0xFF;
			state |= TS_STATE::AMBIGUITY;
		}
		else if ( cmd == TS_CMD::SET_OFF )
		{
			if ( state & TS_STATE::AUTO ) return state;

			state &= ~0xFF;
			state |= TS_STATE::OFF;
		}
		else if ( cmd == TS_CMD::SET_ON )
		{
			if ( state & TS_STATE::AUTO ) return state;

			state &= ~0xFF;
			state |= TS_STATE::ON;
		}

		return state;
	}

	void EntryTS::AddSrcChecker( const string_t& src_name, StateCheckerPtr checker )
	{
		src_checkers_map_[src_name].push_back( checker );
	}

	string_t EntryTS::GetCmdName() const
	{
		return cmd_name_;
	}

}