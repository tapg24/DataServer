#ifndef CHANNEL_OPC_H
#define CHANNEL_OPC_H

#include "utils/types.h"
#include "utils/string.h"
#include <vector>
#include <set>
#include <map>
#include <memory>

#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/noncopyable.hpp>

#include "channels/channel_base.h"
#include "utils/active_object.hpp"

////////////////////////////////////////////////////////////////////////////
//// OPC Client Library
#include <atlcoll.h>
#include "../../opc_client/include/OPCServer.h"
#include "../../opc_client/include/OPCGroup.h"
////////////////////////////////////////////////////////////////////////////
class COPCItem;

namespace Channels
{
	//class ChannelMgr;

	namespace OPC
	{

		class ChannelOPC : public Channel, public IAsynchDataCallback, public IShutdownRequestCallback, public ActiveObject
		{
			string_t host_;
			string_t server_;
			std::auto_ptr<COPCServer> opcServer_;

			typedef std::map<string_t, COPCGroup*> GroupMap;
			GroupMap groups_;
			typedef std::map<string_t, COPCItem*> ItemMap;
			ItemMap items_;

			std::vector<string_t> m_groupNameVector;
			std::set<string_t> itemNames_;

			bool connected_;

			DWORD keepAlive_;
			DWORD reviseKeepAlive_;
			FILETIME lastUpdate_;

			struct SystemTags
			{
				string_t channelAlive;
			} systemTags;

			boost::shared_ptr<boost::thread> checkThread_;
			boost::shared_ptr<boost::thread> reastartThread_;
			boost::mutex update_;
			boost::mutex access_;
			bool checkKeepAlive_;
			bool doRestart_;

		public:
			ChannelOPC(const string_t& name, const string_t& host, const string_t& server);
			~ChannelOPC();
			// запуск канала
			void Start();
			// остановка канала
			void Stop();
			// Add item
			bool AddItems(const std::set<string_t>& itemNameList, const string_t& groupName = "rootGroup");
			size_t ItemCount();

		private:
			void Connect();
			void Disconnect();
			void Reconnect();
			// IShutdownRequestCallback
			void ShutdownRequest(const CString& reason);
			// IAsynchDataCallback
			void OnDataChange(COPCGroup & group, CAtlMap<COPCItem *, OPCItemData *> & changes);
			bool GroupExist(const string_t& groupName);
			COPCGroup* GetGroup(const string_t& groupName);
			ULONGLONG GetLastUpdateTick() const;

			// Get last time updated
			void UpdateLastTimeAccess();

			void CheckAliveThread();
			void RestartThread();

			void StartImpl();
			void StopImpl();

			//////////////////////////////////////////////////////////////////////////
			// opc channel interface
			//////////////////////////////////////////////////////////////////////////

			bool AddItemsImpl(const std::set<string_t>& itemNameList, const string_t& groupName = "rootGroup");
			bool AddGroup(const string_t& groupName  = "rootGroup");
			void DeleteItems(const std::vector<string_t> itemNameList, const string_t& groupName = "rootGroup");
			void DeleteGroups(const std::vector<string_t> groupNameList);
		};

	}

	typedef boost::shared_ptr<Channels::OPC::ChannelOPC> ChannelOPCPtr;
}

#endif // CHANNEL_OPC_H