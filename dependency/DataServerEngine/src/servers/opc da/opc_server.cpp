#include "servers/opc da/opc_server.h"

#include "core/project_mgr.h"
#include "channels/cache_item.h"
#include "channels/cache_mgr.h"
#include "channels/channel_mgr.h"
#include "channels/channel_mgr.h"
#include "entries/entry.h"
#include "entries/entry_factory.h"
#include "utils/locale.hpp"
#include "utils/variant.h"
#include "utils/json_variant_helper.h"

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
			channel_connection_ = projectMgr::getInstance().GetChannelMgr()->Bind( boost::bind(&OPCServer::__OnChannelStateChange, this, _1) );
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

		void OPCServer::__OnChannelStateChange(const Channels::ChannelInfo info)
		{
			switch( info.state )
			{
			case Channels::Connected:
				{
					if ( frl::opc::opcAddressSpace::getInstance().isExistTag(info.name) )
					{
						frl::opc::address_space::Tag* tag = opcAddressSpace::getInstance().getTag(info.name);
						tag->write( ComVariant(true).getConstRef() );
					}
					break;
				}
			case Channels::Disconnected:
				{
					if ( frl::opc::opcAddressSpace::getInstance().isExistTag(info.name) )
					{
						frl::opc::address_space::Tag* tag = opcAddressSpace::getInstance().getTag(info.name);
						tag->write( ComVariant(false).getConstRef() );
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

		bool OPCServer::AddTag(const string_t& name, const ComVariant& value)
		{
			try
			{
				address_space::Tag *tag = NULL;

				tag = opcAddressSpace::getInstance().addLeaf( name, true);
				tag->subscribeToBeforeOpcClientChange( boost::function< bool( const address_space::Tag* const ) >( boost::bind( &OPCServer::OnBeforeDataChange, this, _1 ) ) );
				tag->subscribeToOpcChange( boost::function< void( const address_space::Tag* const ) >( boost::bind( &OPCServer::OnDataChange, this, _1 ) ) );
				tag->isWritable( true );
				//tag->write( tagValue );
				tag->writeFromOPCClient( value.getConstRef() );
				ItemAddedEvt event(name);
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
					const Json::Value& item = data_[idx];

					string_t item_name = item["name"].asString();
					string_t group_name = item["group"].asString();

					if ( !item["descr"].isNull() )
					{
						string_t item_descr = item_name + "_DESCR";
						AddTag(item_descr, JsonHelper::GetComVariant(item["descr"], VT_BSTR));
					}

					if ( group_name == "terminal" )
					{
						// vars
						if ( !item["vars"].isNull() )
						{
							const Json::Value& vars = item["vars"];
							for ( size_t idx_var = 0; idx_var < vars.size(); ++idx_var )
							{
								const Json::Value& var = vars[idx_var];
								string_t var_name = item_name + "_" + var["name"].asString();
								
								Channels::EntryPtr var_entry = Channels::EntryFactory::Create(var_name, var);
								entriesMgr.AddEntry(var_entry);
								AddTag(var_name, JsonHelper::GetComVariant(var["dump"], var["type"]));
								
								if ( !var["unit"].isNull() )
								{
									const Json::Value& unit = var["unit"];
									string_t var_unit = var_name + "_UNIT";
									Channels::EntryPtr unit_entry = Channels::EntryFactory::Create(var_unit, unit);
									entriesMgr.AddEntry(unit_entry);
									AddTag(var_unit, JsonHelper::GetComVariant(unit["dump"], VT_BSTR));
								}

								if ( !var["descr"].isNull() )
								{
									const Json::Value& descr = var["descr"];
									string_t var_descr = var_name + "_DESCR";
									AddTag(var_descr, JsonHelper::GetComVariant(descr, VT_BSTR));
								}
							}
						}
					}
					else if ( group_name == "system" )
					{
						AddTag(item_name, JsonHelper::GetComVariant(item["dump"], item["type"]));
					}
					else if ( group_name == "system.connections" )
					{
						AddTag(item_name, ComVariant(false));
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

			const int size = data_.size();
			for ( size_t idx = 0; idx < data_.size(); ++idx )
			{
				Json::Value& item = data_[idx];

				const string_t item_name = item["name"].asString();
				const string_t group_name = item["group"].asString();

				if ( !item["descr"].isNull() )
				{
					string_t item_descr = item_name + "_DESCR";
					if ( frl::opc::opcAddressSpace::getInstance().isExistTag( item_descr ) )
					{
						const ComVariant value = (VARIANT&)frl::opc::opcAddressSpace::getInstance().getTag( item_descr )->read();
						JsonHelper::copy(value, item["descr"]);
					}
					else BOOST_LOG_TRIVIAL(error) << item_descr << " " << __T("не существует");
				}

				if ( group_name == "terminal" )
				{
					// vars
					if ( !item["vars"].isNull() )
					{
						Json::Value& vars = item["vars"];
						for ( size_t idx_var = 0; idx_var < vars.size(); ++idx_var )
						{
							Json::Value& var = vars[idx_var];

							const string_t var_name = item_name + "_" + var["name"].asString();
							if ( frl::opc::opcAddressSpace::getInstance().isExistTag( var_name ) )
							{
								const ComVariant value = (VARIANT&)frl::opc::opcAddressSpace::getInstance().getTag( var_name )->read();
								JsonHelper::copy(value, var["dump"]);
							}
							else BOOST_LOG_TRIVIAL(error) << var_name << " " << __T("не существует");

							if ( !var["unit"].isNull() )
							{
								const string_t var_unit = var_name + "_UNIT";
								if ( frl::opc::opcAddressSpace::getInstance().isExistTag( var_unit ) )
								{
									const ComVariant value = (VARIANT&)frl::opc::opcAddressSpace::getInstance().getTag( var_unit )->read();
									JsonHelper::copy(value, var["unit"]["dump"]);
								}
								else BOOST_LOG_TRIVIAL(error) << var_unit << " " << __T("не существует");
							}

							if ( !var["descr"].isNull() )
							{
								const string_t var_descr = var_name + "_DESCR";
								if ( frl::opc::opcAddressSpace::getInstance().isExistTag( var_descr ) )
								{
									const ComVariant value = (VARIANT&)frl::opc::opcAddressSpace::getInstance().getTag( var_descr )->read();
									JsonHelper::copy(value, var["descr"]);
								}
								else BOOST_LOG_TRIVIAL(error) << var_descr << " " << __T("не существует");
							}
						}
					}
				}
				else if ( group_name == "system" )
				{
					if ( frl::opc::opcAddressSpace::getInstance().isExistTag( item_name ) )
					{
						const ComVariant value = (VARIANT&)frl::opc::opcAddressSpace::getInstance().getTag( item_name )->read();
						JsonHelper::copy(value, item["dump"]);
					}
				}
				else
				{

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

		Channels::EntriesMgr& OPCServer::GetEntriesMgr()
		{
			return entriesMgr;
		}

		void OPCServer::GetAllLeafs( std::vector<string_t>& namesList, const DWORD accessFilter )
		{
			opcAddressSpace::getInstance().getAllLeafs(namesList, accessFilter);
		}

		frl::opc::address_space::Tag* OPCServer::GetTag( string_t fullPath )
		{
			return opcAddressSpace::getInstance().getTag(fullPath);
		}

	}
}