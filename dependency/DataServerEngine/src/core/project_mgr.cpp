#include "core/project_mgr.h"
#include "servers/opc da/opc_server.h"
#include "channels/cache_mgr.h"
#include "channels/channel_mgr.h"
#include "channels/channel_base.h"
#include "channels/channel_opc/channel_opc.h"
#include "channels/channel_modbus/channel_modbus_rtu.h"
#include "channels/channel_modbus/devices/modbus_device_base.h"
#include "channels/channel_modbus/devices/modbus_device_factory.h"
#include "utils/wxzip.h"
#include "utils/path.h"

#include <fstream>
#include <algorithm>

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/log/trivial.hpp>
#include <boost/format.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

namespace Core
{
	ProjectMgr::ProjectMgr()
	{
	}

	ProjectMgr::~ProjectMgr()
	{
		UninitOpcServer();
	}

	bool ProjectMgr::IsOpen()
	{
		return isOpen_;
	}

	string_t ProjectMgr::GetProjectFilename()
	{
		FRL_EXCEPT_GUARD();

		boost::filesystem::path current_dir = Util::GetModulePath();
		boost::filesystem::path conf = current_dir / "main_conf";

		if( !boost::filesystem::exists( conf ) )
		{
			FRL_THROW_CLASS( InvalidMainConf, "Error while opening the main configuration file." );
		}

		boost::filesystem::ifstream ifs(conf);

		Json::Value root;
		Json::Reader reader;

		if ( !reader.parse(ifs, root) )
		{
			FRL_THROW_CLASS( InvalidMainConf, "Error while parsing the main configuration file." );
		}

		if ( root["version"].asInt() != 2 )
		{
			FRL_THROW_CLASS( InvalidMainConf, "Not supported version." );
		}

		boost::filesystem::path project = current_dir /root["project"].asString(); 
		return project.string();
	}

	void ProjectMgr::InitOpcServer()
	{
		txtAdressspace_ = Util::Zip::GetEntry(projectFilename_, "adressspace");
		serverThread_.reset( new Servers::OPC::OPCServer(txtAdressspace_) );
	}

	void ProjectMgr::UninitOpcServer()
	{
		if ( serverThread_ )
		{
			string_t adressspace = serverThread_->GetAdressSpaceData();
			Util::Zip::UpdateEntry(projectFilename_, "adressspace", adressspace);
			serverThread_.reset();
		}
	}

	OPCServerWPtr ProjectMgr::WeakOpcServer()
	{
		return OPCServerWPtr(serverThread_);
	}

	OPCServerSPtr ProjectMgr::SharedOpcServer()
	{
		return serverThread_;
	}

	void ProjectMgr::InitChannels()
	{
		FRL_EXCEPT_GUARD();

		txtChannels_ = Util::Zip::GetEntry(projectFilename_, "channels");

		Json::Value root;
		Json::Reader reader;

		if ( !reader.parse(txtChannels_, root) )
		{
			FRL_THROW_CLASS( NotInitialize, "Error while parsing the channels configuration file." );
		}

		Json::Value channels = root["modbus rtu nport"];
		for ( size_t idx = 0; idx < channels.size(); ++idx )
		{
			Channels::Channel* channel = channelMgr_->Create(Channels::MODBUS_RTU_NPORT, channels[idx]);
			Channels::Modbus::ModbusRTU* modbus_rtu_nport = static_cast<Channels::Modbus::ModbusRTU*>(channel);

			string_t path = str( boost::format("channel_info/modbus_rtu/%1%") % channel->GetName() );
			string_t txtDevices;

			try
			{
				txtDevices = Util::Zip::GetEntry(projectFilename_, path);
				Json::Value jsonDevices;

				if (reader.parse(txtDevices, jsonDevices))
				{
					jsonDevices = jsonDevices["devices"];
					for ( size_t idx = 0; idx < jsonDevices.size(); ++idx )
					{
						boost::shared_ptr<Channels::Channel> ch(channel);
						Channels::Modbus::Device device( Channels::Modbus::CreateDevice(ch, jsonDevices[idx]) );
						modbus_rtu_nport->AddDevice(device);
					}
				}
				else
				{
					BOOST_LOG_TRIVIAL(warning) << string_t(path) + " not parsed";
				}
			}
			catch(const Util::Zip::FileNotExist& exception)
			{
				BOOST_LOG_TRIVIAL(warning) << exception.GetDescription();
				continue;
			}

			channel->Start();
		}

		//channels = root["modbus rtu serial"];
		//for ( size_t idx = 0; idx < channels.size(); ++idx )
		//{
		//	string_t connection = channels[idx]["connection"].asString();
		//	string_t name = channels[idx]["name"].asString();

		//	Channels::Channel* channel = channelMgr::getInstance().Create(Channels::MODBUS_RTU_SERIAL, name);
		//	StringArray splitArray;
		//	boost::split( splitArray, connection, boost::is_any_of("=") );
		//	channel->SetInitParam( splitArray );
		//	//channel->Start();
		//}

		channels = root["opc da"];
		for ( size_t idx = 0; idx < channels.size(); ++idx )
		{
			Channels::Channel* channel = channelMgr_->Create(Channels::OPC_DA2, channels[idx]);
			Channels::OPC::ChannelOPC* opc = static_cast<Channels::OPC::ChannelOPC*>(channel);

			string_t path = str( boost::format("channel_info/opc/%1%") % channel->GetName() );
			string_t txtTags = Util::Zip::GetEntry(projectFilename_, path);

			Json::Value jsonTags;
			std::set<string_t> items;
			if (reader.parse(txtTags, jsonTags))
			{
				jsonTags = jsonTags["tags"];
				for ( size_t idx = 0; idx < jsonTags.size(); ++idx )
				{
					items.insert(jsonTags[idx].asString());
				}
			}
			channel->Start();
			opc->AddItems(items);
		}
	}

	void ProjectMgr::RegOpcServer()
	{
		serverThread_->RegisterServer();
	}

	void ProjectMgr::UnregOpcServer()
	{
		serverThread_->UnregisterServer();
	}

	void ProjectMgr::LoadProject( const string_t& filename /* = "" */)
	{
		FRL_EXCEPT_GUARD();

		CloseProject();

		string_t tmp;

		if ( filename.empty() )
		{
			try
			{
				tmp = GetProjectFilename();
			}
			catch(const InvalidMainConf& ex)
			{
				BOOST_LOG_TRIVIAL(error) << ex.getFullDescription();
				return;
			}
		}
		else tmp = filename;

		if ( tmp.empty() )
		{
			BOOST_LOG_TRIVIAL(error) << "Не задан файл проекта.";
			return;
		}
		else if ( !boost::filesystem::exists(tmp) )
		{
			BOOST_LOG_TRIVIAL(error) << tmp << " - Файл проекта отсутствует.";
			return;
		}
		if ( !ValidProjectFile(tmp) )
		{
			BOOST_LOG_TRIVIAL(error) << tmp << " - Файл проекта повреждён.";
			return;
		}

		projectFilename_ = tmp;

		InitOpcServer();
		updateBinding_();
		InitChannels();
	}

	Channels::CacheMgr* ProjectMgr::GetCacheMgr()
	{
		BOOST_ASSERT( cacheMgr_.get() != NULL );
		return cacheMgr_.get();
	}

	Channels::ChannelMgr* ProjectMgr::GetChannelMgr()
	{
		BOOST_ASSERT( channelMgr_.get() != NULL );
		return channelMgr_.get();
	}

	boost::signals2::connection ProjectMgr::Bind( const Slot& slot )
	{
		return updateBinding_.connect(slot);
	}

	string_t ProjectMgr::ProjectName() const
	{
		boost::filesystem::path project(projectFilename_);
		return project.filename();
	}

	bool ProjectMgr::ValidProjectFile( const string_t& filename )
	{
		std::list<string_t> expected;
		expected.push_back("adressspace");
		expected.push_back("channels");
		expected.push_back("project_info");
		
		std::list<string_t> entrys;
		if (!Util::Zip::IsValidArchive(filename))
		{
			return false;
		}
		else
		{
			Util::Zip::GetListEntry(filename, entrys);
			BOOST_FOREACH(const string_t& exp, expected)
			{
				if ( std::count(entrys.begin(), entrys.end(), exp) == 0 )
				{
					return false;
				}
			}
		}
		return true;
	}

	void ProjectMgr::CloseProject()
	{
		channelMgr_.reset( new Channels::ChannelMgr );
		cacheMgr_.reset( new Channels::CacheMgr );
		UninitOpcServer();
	}

}