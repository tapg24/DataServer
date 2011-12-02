#include "channels/channel_modbus/channel_modbus_rtu.h"
#include "channels/channel_modbus/modbus_except.hpp"
#include "core/project_mgr.h"
#include "channels/cache_mgr.h"
#include "utils/variant.h"

#include <boost/foreach.hpp>
#include <boost/log/trivial.hpp>

namespace Channels
{
	namespace Modbus
	{
		ModbusRTU::ModbusRTU(const boost::shared_ptr<StreamRTU> modbusStream, const std::string& name, const int pause)
			: Channel(MODBUS_RTU_NPORT, name), stream_(modbusStream), pauseDuration_ms(pause)
		{
			BOOST_LOG_TRIVIAL(trace) << "Канал Modbus RTU создан.";
			notifyAfterRequest = false;
			streamThreadStoped_ = true;

			systemTags.cycleTime = name + ".system.cycle_time";
			Channel::AddTag(systemTags.cycleTime, ComVariant(double(0)), OPC_QUALITY_GOOD);
			systemTags.channelAlive = name + ".alive";
			Channel::AddTag(systemTags.channelAlive, ComVariant(bool(false)), OPC_QUALITY_GOOD);

			StateChanged(Created);
		}
	
		ModbusRTU::~ModbusRTU()
		{
			Stop();
			streamThread_.join();

			StateChanged(Deleted);
			BOOST_LOG_TRIVIAL(trace) << "Канал Modbus RTU удалён.";
		}

		void ModbusRTU::Start()
		{
			connectToConverter_ = true;
			streamThread_ = boost::thread(&ModbusRTU::StreamThread, this);

			StateChanged(InProgress);
		}

		void ModbusRTU::Stop()
		{
			StopStreamThread();

			StateChanged(Disconnected);

			stream_->Close();
		}

		void ModbusRTU::StreamThread()
		{
			converterAlive_ = false;
			streamThreadStoped_ = false;
			firstStart = true;

			while( !stopStreamThread_.timedWait( 50 ) )
			{
				if ( !converterAlive_ && connectToConverter_ )
				{
					if  ( !firstStart )
					{
						boost::this_thread::sleep(boost::posix_time::seconds(2));
						firstStart = false;
					}

					try
					{
						stream_->Open();
						stream_->SetTimeout(100);
						StateChanged(Connected);
						converterAlive_ = true;
						notifyAfterRequest = false;
						projectMgr::getInstance().GetCacheMgr()->GetItem(systemTags.channelAlive)->CopyFrom(OPC_QUALITY_GOOD, ComVariant(true));
					}
					catch(std::runtime_error&)
					{
						stream_->Close();
						projectMgr::getInstance().GetCacheMgr()->GetItem(systemTags.channelAlive)->CopyFrom(OPC_QUALITY_GOOD, ComVariant(false));
						continue;
					}
					catch(modbus_error_notalive&)
					{
						stream_->Close();
						projectMgr::getInstance().GetCacheMgr()->GetItem(systemTags.channelAlive)->CopyFrom(OPC_QUALITY_GOOD, ComVariant(false));
						continue;
					}
				}

				if ( converterAlive_  )
				{
					projectMgr::getInstance().GetCacheMgr()->GetItem(systemTags.cycleTime)->CopyFrom(OPC_QUALITY_GOOD, ComVariant(cyclePerformance.GetElapsedSec()));
					cyclePerformance.Reset();

					BOOST_FOREACH(DevicePtr& device, devices_)
					{
						try
						{
							if( !converterAlive_ )
							{
								break;
							}

							boost::this_thread::sleep(boost::posix_time::milliseconds(pauseDuration_ms));
							device->DoDataRequest(stream_);
							projectMgr::getInstance().GetCacheMgr()->GetItem(systemTags.channelAlive)->CopyFrom(OPC_QUALITY_GOOD, ComVariant(true));
						}
						catch (modbus_error_notalive&)
						{
							StateChanged(Disconnected);
							//StateChanged(ChannelInProgress);
							converterAlive_ = false;
							projectMgr::getInstance().GetCacheMgr()->GetItem(systemTags.channelAlive)->CopyFrom(OPC_QUALITY_GOOD, ComVariant(false));
							ResetCache();

							break;
						}
					}

					if ( !notifyAfterRequest )
					{
						notifyAfterRequest = true;

						projectMgr::getInstance().GetCacheMgr()->NotifyAll();
					}
				}
			}

			//StateChanged(ChannelDisconnected);

			streamThreadStoped_ = true;
		}

		void ModbusRTU::StopStreamThread()
		{
			connectToConverter_ = false;
			stopStreamThread_.signal();
			while( !streamThreadStoped_ )
			{
				boost::this_thread::sleep(boost::posix_time::milliseconds(1));
			}

			ResetCache();
			StateChanged(Disconnected);
		}


		void ModbusRTU::AddDevice( const DevicePtr& device )
		{
			devices_.push_back(device);
		}
	}

}