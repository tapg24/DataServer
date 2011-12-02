#ifndef PROJECT_MGR_H
#define PROJECT_MGR_H

#include "utils/string.h"
#include "utils/singleton.h"
#include "servers/opc da/opc_server_fwd.h"
#include "channels/cache_mgr_fwd.h"
#include "channels/channel_mgr_fwd.h"

#include "../../frl/include/frl_exception.h"

#include <memory>
#include <boost/shared_ptr.hpp>
#include <boost/signals2.hpp>

namespace Core
{
	class ProjectMgr
	{
		typedef boost::signals2::signal< void () > Signal;
		typedef Signal::slot_type Slot;

		bool isOpen_;
		string_t projectFilename_;
		string_t txtChannels_;
		string_t txtConvertInfo_;
		string_t txtAdressspace_;
		string_t txtTags_;

		Signal updateBinding_;

		CacheMgrPtr cacheMgr_;
		ChannelMgrPtr channelMgr_;
		OPCServerSPtr serverThread_;

	public:
		FRL_EXCEPTION_CLASS( NotInitialize );
		FRL_EXCEPTION_CLASS( InvalidMainConf );
		FRL_EXCEPTION_CLASS( InvalidProject );

		ProjectMgr();
		~ProjectMgr();

		// файл проекта открыт
		bool IsOpen();
		// получить экземпл€р opc сервера
		OPCServerWPtr WeakOpcServer();
		OPCServerSPtr SharedOpcServer();
		// получить кеш
		Channels::CacheMgr* GetCacheMgr();
		Channels::ChannelMgr* GetChannelMgr();
		// получить каналы
		void RegOpcServer();
		void UnregOpcServer();
		void LoadProject(const string_t& filename = "");
		void CloseProject();
		boost::signals2::connection Bind(const Slot& slot);
		string_t ProjectName() const;

	private:
		string_t GetProjectFilename();
		bool ValidProjectFile(const string_t& filename);
		// настройка opc сервера
		void InitOpcServer();
		// удаление opc сервера
		void UninitOpcServer();
		// настройка каналов
		void InitChannels();
	};
}

typedef Singleton< Core::ProjectMgr > projectMgr;

#endif // PROJECT_MGR_H