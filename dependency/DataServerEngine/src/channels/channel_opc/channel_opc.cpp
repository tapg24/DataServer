#include "channels/channel_opc/channel_opc.h"
#include "core/project_mgr.h"
#include "channels/channel_mgr.h"
#include "channels/cache_mgr.h"
#include "../../frl/include/frl_exception.h"
#include "utils/variant.h"

//////////////////////////////////////////////////////////////////////////
// OPC Client Library
#include "../../opc_client/include/OPCHost.h"
#include "../../opc_client/include/OPCItem.h"
//////////////////////////////////////////////////////////////////////////

#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/log/trivial.hpp>

namespace Channels
{

	namespace OPC
	{
		ChannelOPC::ChannelOPC( const string_t& name, const string_t& host, const string_t& server )
			: Channel(OPC_DA2, name),
			host_(host),
			server_(server),
			keepAlive_(10000),
			reviseKeepAlive_(0)
		{
			connected_ = false;

			systemTags.channelAlive = name + ".alive";
			Channel::AddTag(systemTags.channelAlive, ComVariant(bool(false)), OPC_QUALITY_GOOD);

			StateChanged(Created);
			
			boost::packaged_task<void> task( &COPCClient::init );
			ActiveObject::post_task<void>(boost::move(task)).wait();
		}


		ChannelOPC::~ChannelOPC()
		{
			Stop();
			StateChanged(Deleted);

			if ( checkThread_ ) checkThread_->join();
			if ( restartThread_ ) restartThread_->join();

			boost::packaged_task<void> task( &COPCClient::uninit );
			ActiveObject::post_task<void>(boost::move(task)).wait();
			ActiveObject::stop();
		}

		void ChannelOPC::StartImpl()
		{
			StateChanged(InProgress);

			try
			{
				Connect();
				checkKeepAlive_ = true;
				checkThread_.reset( new boost::thread(&ChannelOPC::CheckAliveThread, this) );

				if ( !itemNames_.empty() )
				{
					AddItemsImpl( itemNames_ );
				}
			}
			catch(frl::Exception& ex)
			{
				BOOST_LOG_TRIVIAL(error) << ex.GetDescription();
			}
		}

		void ChannelOPC::StopImpl()
		{
			if ( !connected_ )
			{
				return;
			}

			checkKeepAlive_ = false;
			doRestart_ = false;
			Disconnect();
			projectMgr::getInstance().GetCacheMgr()->GetItem(systemTags.channelAlive)->CopyFrom(OPC_QUALITY_GOOD, ComVariant(false));
		}

		void ChannelOPC::Connect()
		{
			FRL_EXCEPT_GUARD();

			BOOST_LOG_TRIVIAL(trace) << "ChannelOPC::Connect";

			if ( connected_ )
			{
				return;
			}

			std::auto_ptr<COPCHost> opcHost( COPCClient::makeHost( host_.c_str() ) );

			try
			{
				opcServer_.reset( opcHost->connectDAServer(server_.c_str()) );
				
				ServerStatus status;
				opcServer_->getStatus(status);

				if ( status.dwServerState == OPC_STATUS_RUNNING )
				{
					StateChanged(Connected);
					connected_ = true;
					projectMgr::getInstance().GetCacheMgr()->GetItem(systemTags.channelAlive)->CopyFrom(OPC_QUALITY_GOOD, ComVariant(true));
					UpdateLastTimeAccess();
					opcServer_->setShutdownRequestCallback(this);
				}
			}
			catch (OPCException& ex)
			{
				FRL_THROW( str( boost::format("%1%/%2%, ошибка соединения. %3%") % host_ % server_ % ex.reasonString() ));
			}
		}

		void ChannelOPC::Disconnect()
		{
			BOOST_LOG_TRIVIAL(trace) << "ChannelOPC::Disconnect";
			
			if ( !connected_ )
			{
				return;
			}

			try
			{
				StateChanged(Disconnected);
				ResetCache();
				DeleteItems(Channel::GetTagNames());
				DeleteGroups(m_groupNameVector);
				opcServer_.reset();
				connected_ = false;
			}
			catch (OPCException& e)
			{
				BOOST_LOG_TRIVIAL(error) << "ChannelOPC::Disconnect " << e.reasonString();
			}
		}

		void ChannelOPC::Reconnect()
		{
			BOOST_LOG_TRIVIAL(trace) << "ChannelOPC::Reconnect";

			StopImpl();
			StartImpl();

			if ( !connected_ )
			{
				return post(boost::bind(&ChannelOPC::Reconnect, this));;
			}
		}

		bool ChannelOPC::AddGroup( const string_t& groupName  /*= "rootGroup"*/)
		{
			try
			{
				if ( opcServer_.get() == NULL )
				{
					return false;
				}

				GroupMap::const_iterator groupIter = groups_.find(groupName);
				if (groupIter != groups_.end())
				{
					return true;
				}

				unsigned long refreshRate = 0;
				COPCGroup* group = opcServer_->makeGroup(groupName.c_str(), true, 1000, refreshRate, 0.0, keepAlive_, reviseKeepAlive_);
				if (group)
				{
					group->enableAsynch( this );
					groups_.insert(std::make_pair(groupName, group));
					m_groupNameVector.push_back(groupName);
				}
			}
			catch (OPCException&)
			{
			}

			return true;
		}


		bool ChannelOPC::GroupExist( const string_t& groupName )
		{
			GroupMap::const_iterator group_iter = groups_.find(groupName);
			if (group_iter == groups_.end())
			{
				return false;
			}

			return true;
		}

		void ChannelOPC::DeleteItems( const std::vector<string_t> itemNameList, const string_t& groupName /*= "rootGroup"*/ )
		{
			if ( opcServer_.get() == NULL )
			{
				return;
			}

			if ( !GroupExist(groupName) )
			{
				return;
			}

			BOOST_FOREACH(const string_t& itemName, itemNameList)
			{
				// костыль!!! исправить!!!
				if( boost::contains(itemName, ".alive") )
				{
					continue;
				}

				// удалим из словаря итемов
				ItemMap::iterator mapIter = items_.find(itemName);
				delete mapIter->second;
				items_.erase(mapIter);
			}
		}

		void ChannelOPC::DeleteGroups(const std::vector<string_t> groupNameList)
		{
			if ( opcServer_.get() == NULL )
			{
				return;
			}

			BOOST_FOREACH(const string_t& groupName, groupNameList)
			{
				if ( GroupExist(groupName) )
				{
					// удалим из списка итемов
					m_groupNameVector.erase(std::remove(m_groupNameVector.begin(), m_groupNameVector.end(), groupName), m_groupNameVector.end());

					// удалим из словаря групп
					GroupMap::iterator mapIter = groups_.find(groupName);
					delete mapIter->second;
					groups_.erase(mapIter);
				}
			}
		}

		COPCGroup* ChannelOPC::GetGroup( const string_t& groupName )
		{
			GroupMap::const_iterator group_iter = groups_.find(groupName);
			if (group_iter == groups_.end())
			{
				return NULL;
			}

			return group_iter->second;
		}

		bool ChannelOPC::AddItemsImpl( const std::set<string_t>& itemNameList, const string_t& groupName /*= "rootGroup"*/ )
		{
			BOOST_LOG_TRIVIAL(trace) << "ChannelOPC::AddItems";

			if ( opcServer_.get() == NULL )
			{
				return false;
			}

			if ( !GroupExist(groupName) )
			{
				AddGroup(groupName);
			}

			COPCGroup* group = GetGroup(groupName);

			CAtlArray<CString> itemName;
			CAtlArray<COPCItem *> itemsCreated;
			CAtlArray<HRESULT> errors;

			BOOST_FOREACH(const string_t& item, itemNameList)
			{
				itemName.Add(item.c_str());
			}

			try
			{
				group->disableAsynch();
				group->addItems(itemName, itemsCreated, errors, true);
				int32_t tagCount = 0;

				CAtlArray<COPCItem *> validItems;
				for (size_t idx = 0; idx < itemsCreated.GetCount(); ++idx)
				{
					if ( ERROR_SUCCESS == errors[idx] )
					{
						validItems.Add(itemsCreated[idx]);
					}
					else
					{
						BOOST_LOG_TRIVIAL(warning) << "Канал " << GetName() << " Тег [" << itemName[idx] << "] на сервере не найден. Ошибка: " << errors[idx];
					}
				}
				COPCItem_DataMap data;
				data.InitHashTable(validItems.GetCount());
				group->readSync(validItems, data, OPC_DS_DEVICE);

				POSITION pos = data.GetStartPosition();
				while (pos != NULL)
				{
					COPCItem* item;
					OPCItemData* itemData;
					data.GetNextAssoc(pos, item, itemData);

					itemNames_.insert(item->getName().GetString());
					items_.insert(std::make_pair(item->getName(), item));
					Channel::AddTag(item->getName().GetString(), itemData->vDataValue, itemData->wQuality);
				}
				group->enableAsynch( this );

				BOOST_LOG_TRIVIAL(trace) << "Канал " << GetName() << " Добавлено " << validItems.GetCount() << " тегов.";
			}
			catch (OPCException& e)
			{
				BOOST_LOG_TRIVIAL(error) << "ChannelOPC::AddItems " << e.reasonString() << " " << e.descriptionString();
			}

			return true;
		}

		void ChannelOPC::UpdateLastTimeAccess()
		{
			::GetSystemTimeAsFileTime( &lastUpdate_ );
		}

		ULONGLONG ChannelOPC::GetLastUpdateTick() const
		{
			return (((ULONGLONG) lastUpdate_.dwHighDateTime) << 32) + lastUpdate_.dwLowDateTime;
		}

		void ChannelOPC::Start()
		{
			boost::packaged_task<void> task( boost::bind(&ChannelOPC::StartImpl, this) );
			ActiveObject::post_task<void>(boost::move(task)).wait();
		}

		void ChannelOPC::Stop()
		{
			boost::packaged_task<void> task( boost::bind(&ChannelOPC::StopImpl, this) );
			ActiveObject::post_task<void>(boost::move(task)).wait();
		}

		bool ChannelOPC::AddItems( const std::set<string_t>& itemNameList, const string_t& groupName /*= "rootGroup"*/ )
		{
			boost::packaged_task<bool> task( boost::bind(&ChannelOPC::AddItemsImpl, this, itemNameList, groupName) );
			return ActiveObject::post_task<bool>(boost::move(task)).get();
		}

		void ChannelOPC::CheckAliveThread()
		{
			while(connected_ && checkKeepAlive_)
			{
				if (!reviseKeepAlive_)
				{
					continue;
				}

				FILETIME ftCurrent;
				::GetSystemTimeAsFileTime( &ftCurrent );
				ULONGLONG curTimeCount = (((ULONGLONG) ftCurrent.dwHighDateTime) << 32) + ftCurrent.dwLowDateTime;
				ULONGLONG lastUpdate = (curTimeCount - GetLastUpdateTick()) / 10000;


				if ( ((curTimeCount - GetLastUpdateTick())/10000) >= (reviseKeepAlive_ + 100) )
				{
					Stop();
					BOOST_LOG_TRIVIAL(trace) << GetName() << ". Server not alive.";
					doRestart_ = true;
					restartThread_.reset( new boost::thread(&ChannelOPC::RestartThread, this) );
					return;
				}

				boost::this_thread::sleep(boost::posix_time::seconds(1));
			}
		}

		void ChannelOPC::RestartThread()
		{
			BOOST_LOG_TRIVIAL(trace) << GetName() << ". Waiting for 60 secconds, before reconnect...";
			size_t seconds_count = 60;
			while( seconds_count )
			{
				--seconds_count;
				if ( connected_ || !doRestart_ )
				{
					BOOST_LOG_TRIVIAL(trace) << GetName() << ". Auto reconnect canceled.";
					return;
				}
				boost::this_thread::sleep(boost::posix_time::seconds(1));
			}
			BOOST_LOG_TRIVIAL(trace) << GetName() << ". Auto reconnect.";
			Start();
			if ( !connected_)
			{
				restartThread_.reset( new boost::thread(&ChannelOPC::RestartThread, this) );
			}
			return;
		}

		void ChannelOPC::ShutdownRequest( const CString& reason )
		{
			BOOST_LOG_TRIVIAL(trace) << GetName() << ". Shutdown request, reason: " << reason;

			boost::packaged_task<void> task( boost::bind(&ChannelOPC::StopImpl, this) );
			ActiveObject::post_task<void>(boost::move(task)).wait();
		}

		void ChannelOPC::OnDataChange( COPCGroup & group, CAtlMap<COPCItem *, OPCItemData *> & changes )
		{
			boost::mutex::scoped_lock guard(update_);
			UpdateLastTimeAccess();

			POSITION pos = changes.GetStartPosition();
			while (pos != NULL)
			{
				COPCItem* item;
				OPCItemData* itemData;
				changes.GetNextAssoc(pos, item, itemData);
				CacheItemPtr cacheItem = projectMgr::getInstance().GetCacheMgr()->GetItem(item->getName().GetString());
				if ( FAILED(itemData->error) )
				{
					if ( cacheItem->GetQuality() != OPC_QUALITY_BAD )
					{
						cacheItem->Reset();
					}
				}
				else
				{
					cacheItem->CopyFrom(itemData->ftTimeStamp, itemData->wQuality, itemData->vDataValue);
				}
			}
		}

		size_t ChannelOPC::ItemCount()
		{
			return items_.size();
		}

	}
}