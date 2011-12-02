#ifndef OPCSERVERTHREAD_H
#define OPCSERVERTHREAD_H

#include "utils/types.h"
#include "core/event_handler.hpp"
#include "channels/channel_base.h"
#include "entries/entries_mgr.h"
#include "utils/active_object.hpp"
#include "channels/channel_mgr_fwd.h"
#include "channels/cache_mgr_fwd.h"

#include <string>
#include <map>
#include <list>
#include <vector>
#include <utility>

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

#include "../../frl/include/frl_opc.h"
#include "../../frl/include/frl_exception.h"
#include "../../jsoncpp-0.5.0/include/json/value.h"

#include <wx/wx.h>
#include <wx/event.h>
#include <wx/thread.h>

namespace Servers
{
	namespace OPC
	{

		class ItemAddedEvt : public Core::EvtType
		{
		private:
			const string_t name_;

		public:
			ItemAddedEvt(const string_t& name) : EvtType(0), name_(name) {}
			~ItemAddedEvt() {}

			const string_t ItemName() const { return name_; }
		};

		typedef Core::EvtHandler<ItemAddedEvt>::Slot ItemAddedSlot;

		class OPCServer : public ActiveObject, boost::noncopyable
		{
			void __OnChannelStateChange(const Channels::ChannelInfo info);
			void __UpdateNotify(const std::list<string_t>& clientTagList);

		public:
			FRL_EXCEPTION_CLASS( NotInitialize );
			FRL_EXCEPTION_CLASS( InvalidAdressSpace );
			FRL_EXCEPTION_CLASS( InvalidConvertInfo );

			OPCServer(const string_t& adressspace);
			~OPCServer();

			boost::signals2::connection Bind(const ItemAddedSlot& slot);

			bool AddBranch(const string_t& tagName);
			bool AddTag(const string_t& name, const ComVariant& value);
			void GetAllLeafs(std::vector<string_t>& namesList, const DWORD accessFilter);
			frl::opc::address_space::Tag* GetTag(string_t fullPath);
			long GetItemCount() const;
			Channels::EntriesMgr& GetEntriesMgr();

			void RegisterServer();
			void UnregisterServer();
			string_t GetAdressSpaceData();

		private:
			void Init();
			void Deinit();
			bool OnBeforeDataChange( const frl::opc::address_space::Tag* const tagRawPtr );
			void OnDataChange( const frl::opc::address_space::Tag* const tagRawPtr );
			void SetAdressSpaceData(const string_t& content);

			boost::shared_ptr<frl::opc::DAServer> opcServer_;
			Json::Value data_;
			Channels::EntriesMgr entriesMgr;
			Core::EvtHandler<ItemAddedEvt> onItemAdded_;

			boost::signals2::connection cache_connection_;
			boost::signals2::connection channel_connection_;
		};
	}
}

typedef boost::shared_ptr<Servers::OPC::OPCServer> OPCServerSPtr;
typedef boost::weak_ptr<Servers::OPC::OPCServer> OPCServerWPtr;

#endif // OPCSERVERTHREAD_H