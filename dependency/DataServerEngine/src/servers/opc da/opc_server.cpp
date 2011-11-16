#include "servers/opc da/opc_server.h"
#include "core/project_mgr.h"
#include "core/state_checker.h"
//#include "core/bit_checker.h"
#include "channels/channel_mgr.h"
#include "channels/cache_item.h"
#include "entries/entry.h"
#include "entries/default_entry.h"
#include "entries/ts_entry.h"
#include "entries/ts_def.h"
#include "utils/locale.hpp"
#include "utils/variant.h"

#include <bitset>
#include <boost/typeof/typeof.hpp>
#include <boost/log/trivial.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>

using namespace frl::opc;

namespace Servers
{
	namespace OPC
	{

		OPCServer::OPCServer( const string_t& adressspace )
		{
			BOOST_LOG_TRIVIAL(trace) << "OPC-сервера создан.";

			opcAddressSpace::getInstance().finalConstruct( FRL_STR("."));

			// initialize server
			opcServer_.reset( new frl::opc::DAServer(frl::opc::ServerTypes::localSever32) );
			opcServer_->setCLSID( FRL_STR("{E712B16C-963C-4aac-AFF6-67CBBE658D9C}") );
			opcServer_->setVendor( FRL_STR("SibIng") );
			opcServer_->setDriverName( FRL_STR("SibIng DataServer") );
			opcServer_->setDescription( FRL_STR("OPC server for data store"));
			opcServer_->setVersion( 0.1 );

			boost::packaged_task<void> task( boost::bind(&OPCServer::Init, this) );
			ActiveObject::post_task<void>(boost::move(task)).wait();

			SetAdressSpaceData(adressspace);

			cache_connection_ = projectMgr::getInstance().GetCacheMgr()->Bind( boost::bind(&OPCServer::__UpdateNotify, this, _1) );
			channel_connection_ = projectMgr::getInstance().GetChannelMgr()->Bind( boost::bind(&OPCServer::__OnChannelStateChange, this, _1, _2) );
		}

		OPCServer::~OPCServer()
		{
			cache_connection_.disconnect();
			channel_connection_.disconnect();

			opcAddressSpace::getInstance().clearAdressSpace();

			boost::packaged_task<void> task( boost::bind(&OPCServer::Deinit, this) );
			ActiveObject::post_task<void>(boost::move(task)).wait();

			BOOST_LOG_TRIVIAL(trace) << "OPC-сервера удалён.";
		}

		bool OPCServer::OnBeforeDataChange( const frl::opc::address_space::Tag* const tagRawPtr )
		{
			const string_t serverTagName = tagRawPtr->getID();
			if ( boost::contains( serverTagName, "_TS" ) )
			{
				return false;
			}

			return true;
		}

		void OPCServer::OnDataChange( const address_space::Tag* const tagRawPtr )
		{
			const string_t serverTagName = tagRawPtr->getID();

			try
			{
				if ( entriesMgr.DstExist(serverTagName) )
				{
					Channels::EntryPtr entry = entriesMgr.GetEntryByDst(serverTagName);
					entry->Update(serverTagName);
				}
			}
			catch (frl::Exception& ex)
			{
				BOOST_LOG_TRIVIAL(trace) << ex.getFullDescription();
			}
		}

		void OPCServer::__OnChannelStateChange(const uint32_t id, Channels::ChannelState state)
		{
			Channels::Channel* channel = projectMgr::getInstance().GetChannelMgr()->GetElseNull(id);
			if ( channel == NULL ) return;

			string_t name = "System.Channels." + channel->GetName();

			switch(state)
			{
			case Channels::Connected:
				{
					if ( frl::opc::opcAddressSpace::getInstance().isExistTag(name) )
					{
						frl::opc::address_space::Tag* tag = opcAddressSpace::getInstance().getTag(name);
						tag->write( Channels::ComVariant(true).getConstRef() );
					}
					break;
				}
			case Channels::Disconnected:
				{
					if ( frl::opc::opcAddressSpace::getInstance().isExistTag(name) )
					{
						frl::opc::address_space::Tag* tag = opcAddressSpace::getInstance().getTag(name);
						tag->write( Channels::ComVariant(false).getConstRef() );
					}
					break;
				}
			default:
				break;
			}

		}

		void OPCServer::__UpdateNotify(const std::list<string_t>& clientTagList)
		{
			try
			{
				BOOST_FOREACH(const string_t& src_name, clientTagList)
				{
					if ( entriesMgr.SrcExist(src_name) )
					{
						std::list<Channels::EntryPtr> entry_list = entriesMgr.GetEntriesBySrc(src_name);
						BOOST_FOREACH(Channels::EntryPtr entry, entry_list)
						{
							entry->Update( entry->GetName() );
						}
					}
				}
			}
			catch (frl::Exception& ex)
			{
				BOOST_LOG_TRIVIAL(trace) << ex.getFullDescription();
			}
		}

		bool OPCServer::AddBranch( const string_t & tagName )
		{
			try
			{
				opcAddressSpace::getInstance().addBranch( tagName );
				return true;
			}
			catch (frl::Exception&)
			{
				return false;
			}
		}

		bool OPCServer::AddTag( const string_t & tagName, const frl::os::win32::com::Variant& tagValue )
		{
			try
			{
				address_space::Tag *tag = NULL;

				tag = opcAddressSpace::getInstance().addLeaf( tagName, true);
				tag->subscribeToBeforeOpcClientChange( boost::function< bool( const address_space::Tag* const ) >( boost::bind( &OPCServer::OnBeforeDataChange, this, _1 ) ) );
				tag->subscribeToOpcChange( boost::function< void( const address_space::Tag* const ) >( boost::bind( &OPCServer::OnDataChange, this, _1 ) ) );
				tag->isWritable( true );
				//tag->write( tagValue );
				tag->writeFromOPCClient( tagValue );
				ItemAddedEvt event(tagName);
				onItemAdded_.Invoke( event );
				return true;
			}
			catch (frl::Exception&)
			{
				return false;
			}
		}

		long OPCServer::GetItemCount() const
		{
			std::vector<frl::String> itemList;
			frl::opc::opcAddressSpace::getInstance().getAllLeafs(itemList, NULL);

			return itemList.size();
		}

		void OPCServer::RegisterServer()
		{
			opcServer_->registrerServer3();
		}

		void OPCServer::UnregisterServer()
		{
			opcServer_->unregisterServer();
		}

		boost::signals2::connection OPCServer::Bind( const ItemAddedSlot& slot )
		{
			return onItemAdded_.Bind(slot);
		}

		void OPCServer::SetAdressSpaceData( const string_t& content )
		{
			FRL_EXCEPT_GUARD();

			try
			{
				Json::Reader reader;

				if ( !reader.parse(content, data_) )
				{
					FRL_THROW_CLASS( InvalidAdressSpace, "Error while parsing the adressspace configuration file." );
				}

				int size = data_.size();
				for ( size_t idx = 0; idx < data_.size(); ++idx )
				{
					// NAME
					string_t name = data_[idx]["name"].asString();
					// CONVERT
					const Json::Value convert = data_[idx]["convert"];
					
					// GROUP
					string_t group = data_[idx]["group"].asString();
					if ( group == "TS" )
					{
						Channels::ComVariant value;
						value = static_cast<WORD>(data_[idx]["value"].asUInt());
						string_t ts_name = name + "_TS";
						string_t ts_cmd = ts_name + "_CMD";
						string_t ts_descr = ts_name + "_DESCR";
						Channels::EntryTsPtr entry( new Channels::EntryTS(ts_name, convert) );
						entriesMgr.AddEntry(entry);
						AddTag(ts_name, value.getConstRef());
						AddTag(ts_descr, Channels::ComVariant( data_[idx].get("descr", "").asString() ).getConstRef());
						AddTag(ts_cmd, Channels::ComVariant( (uint16_t)0 ).getConstRef() );
					}
					else if ( group == "default" )
					{
						Channels::ComVariant value;
						// TYPE
						VARTYPE type = static_cast<VARTYPE>(data_[idx]["type"].asInt());
						// VALUE
						switch (type)
						{
						case 4: // float
							value = static_cast<float>(data_[idx]["value"].asDouble());
							break;
						case 8: // string
							value = data_[idx]["value"].asString();
							break;
						case 11: // bool
							value = data_[idx]["value"].asBool();
							break;
						case 18: // word
							value = static_cast<WORD>(data_[idx]["value"].asUInt());
							break;
						default:
							throw std::runtime_error(str( boost::format("type not exists. index - %1% name - %2% type - %3%") % idx % name % type ));
							break;
						}
						Channels::DefaultEntryPtr entry( new Channels::DefaultEntry(name, convert) );
						entriesMgr.AddEntry(entry);
						AddTag(entry->GetName(), value.getConstRef());
						string_t descr = name + "_DESCR";
						AddTag(descr, Channels::ComVariant( data_[idx].get("descr", "").asString() ).getConstRef());
						// UNIT
						string_t unit = name + "_UNIT";
						AddTag(unit, Channels::ComVariant( data_[idx]["unit"].asString() ).getConstRef());
					}
					else if ( group == "system" )
					{
						Channels::ComVariant value;
						// TYPE
						VARTYPE type = static_cast<VARTYPE>(data_[idx]["type"].asInt());
						// VALUE
						switch (type)
						{
						case 4: // float
							value = static_cast<float>(data_[idx]["value"].asDouble());
							break;
						case 8: // string
							value = data_[idx]["value"].asString();
							break;
						case 11: // bool
							value = data_[idx]["value"].asBool();
							break;
						case 18: // word
							value = static_cast<WORD>(data_[idx]["value"].asUInt());
							break;
						default:
							throw std::runtime_error(str( boost::format("type not exists. index - %1% name - %2% type - %3%") % idx % name % type ));
							break;
						}
						AddTag(name, value.getConstRef());
					}
					else if ( group == "system.connections" )
					{
						AddTag(name, Channels::ComVariant(false).getConstRef());
					}
					else
					{
					}
				}
			}
			catch (std::runtime_error& exception)
			{
				FRL_THROW_CLASS( NotInitialize, exception.what() ); 
			}
		}

		string_t OPCServer::GetAdressSpaceData()
		{
			FRL_EXCEPT_GUARD();

			LocaleSaveRestore_C c;

			int size = data_.size();
			for ( size_t idx = 0; idx < data_.size(); ++idx )
			{
				string_t name = data_[idx]["name"].asString();
				if ( data_[idx]["group"].asString() == "TS")
				{
					name.append("_TS");
				}
				
				frl::opc::address_space::Tag* tag = NULL;

				try
				{
					tag = frl::opc::opcAddressSpace::getInstance().getTag( name );
				}
				catch(const frl::opc::address_space::Tag::NotExistTag& ex)
				{
					BOOST_LOG_TRIVIAL(error) << ex.getFullDescription();
					continue;
				}

				VARIANT variant = tag->read();
				Channels::ComVariant var( variant );

				switch (var.getType())
				{
				case VT_R4: // float
					data_[idx]["value"] = var.AsFloat();
					break;
				case VT_BSTR: // string
					data_[idx]["value"] = var.AsString();
					break;
				case VT_BOOL: // bool
					data_[idx]["value"] = var.AsBool();
					break;
				case VT_UI2: // word
					data_[idx]["value"] = var.AsUShort();
					break;
				default:
					//throw std::runtime_error(str( boost::format("type not exists. index - %1% name - %2% type - %3%") % idx % name % type ));
					break;
				}
			}

			return Json::StyledWriter().write( data_ );
		}

		void OPCServer::Init()
		{
			opcServer_->init();
		}

		void OPCServer::Deinit()
		{
			if ( opcServer_->ServerIsLive() )
			{
				opcServer_->SendShutdownRequest();

				int count = 0;
				while( opcServer_->hasConnectedClient() )
				{
					if (++count == 2000)
					{
						break;
					}

					boost::this_thread::sleep(boost::posix_time::seconds(1));
				}

				opcServer_->ShutdownServer();
			}

			opcServer_->uninit();
		}

		void OPCServer::GetAllLeafs( std::vector<string_t>& namesList, const DWORD accessFilter )
		{
			opcAddressSpace::getInstance().getAllLeafs(namesList, accessFilter);
		}

		frl::opc::address_space::Tag* OPCServer::GetTag( string_t fullPath )
		{
			return opcAddressSpace::getInstance().getTag(fullPath);
		}

		Channels::EntriesMgr& OPCServer::GetEntriesMgr()
		{
			return entriesMgr;
		}

	}
}