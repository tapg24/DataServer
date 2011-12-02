#include "channels/channel_modbus/devices/radius/sirius2_base.h"

#include "core/project_mgr.h"
#include "channels/cache_mgr.h"
#include "utils/timer.hpp"

#include <boost/foreach.hpp>

namespace Channels
{
	namespace Modbus
	{
		Sirius2Base::Sirius2Base( ChannelWPtr parent
			, const string_t& deviceId
			, const int32_t deviceType
			, const string_t deviceTypeString
			, const int32_t serialId
			, const int32_t modbusId
			)
			: ModbusDeviceBase(parent, deviceId), deviceType_(deviceType), type_string_(deviceTypeString), serialId_(serialId), modbusId_(modbusId)
		{
			identRequested_ = false;
			deviceStatus_ = 0;
			checkCount_ = 0;

			systemTags.type = ModbusDeviceBase::WithPrefix("device_family_type");
			systemTags.serial_code = ModbusDeviceBase::WithPrefix("serial_code");
			systemTags.request_time = ModbusDeviceBase::WithPrefix("system.request_time");
			systemTags.device_time = ModbusDeviceBase::WithPrefix("system.device_time");
			systemTags.deviceStatus = ModbusDeviceBase::WithPrefix("system.device_status");

			AddTag(ModbusDeviceBase::WithPrefix("modbus_address"), ComVariant(modbusId_), OPC_QUALITY_GOOD);
			AddTag(ModbusDeviceBase::WithPrefix("type_dec"), ComVariant(deviceType_), OPC_QUALITY_GOOD);
			AddTag(ModbusDeviceBase::WithPrefix("type_string"), ComVariant(type_string_), OPC_QUALITY_GOOD);
			AddTag(ModbusDeviceBase::WithPrefix("device_status"), ComVariant(deviceStatus_), OPC_QUALITY_GOOD);
			AddTag(systemTags.request_time, ComVariant(double(0)), OPC_QUALITY_BAD);
			AddTag(systemTags.device_time, ComVariant(double(0)), OPC_QUALITY_BAD);
			AddTag(systemTags.deviceStatus, ComVariant(uint8_t(0)), OPC_QUALITY_BAD);

			// identity request
			identityRequest_.station = modbusId;
			identityRequest_.func = 3;
			identityRequest_.startAdress = 4;
			identityRequest_.byteCount = 4;
			identityRequest_.tags.push_back( TagInfo("type", 18) );
			identityRequest_.tags.push_back( TagInfo("serial_code", 18) );
		}

		void Sirius2Base::DoIdentityRequest( boost::shared_ptr<StreamRTU>& stream )
		{
			ModbusRequest::DoRequest(identityRequest_, stream);

			// если тип устройства несовпадает, то отключаем опрос устройства
			if ( deviceType_ == projectMgr::getInstance().GetCacheMgr()->GetItem(systemTags.type)->GetValue().AsUShort() )
			{
				deviceStatus_ &= ~TypeNotValid;
			}
			else
			{
				deviceStatus_ |=  TypeNotValid;
			}

			// если серийный код устройства не совпадает, то отключаем функцию управления сириусом
			if ( serialId_ == projectMgr::getInstance().GetCacheMgr()->GetItem(systemTags.serial_code)->GetValue().AsUShort() )
			{
				deviceStatus_ &= ~SerialNotValid;
			}
			else
			{
				deviceStatus_ |=  SerialNotValid;
			}

			projectMgr::getInstance().GetCacheMgr()->GetItem(systemTags.deviceStatus)->CopyFrom(OPC_QUALITY_GOOD, ComVariant(deviceStatus_));
		}

		void Sirius2Base::DoDataRequest( boost::shared_ptr<StreamRTU>& stream )
		{
			CXTimer devicePerformance;

			if ( !identRequested_ )
			{
				DoIdentityRequest(stream);
				identRequested_= true;
			}

			if ( (deviceStatus_ & TypeNotValid) == TypeNotValid )
			{
				if  ( ++checkCount_ == 2000 )
				{
					checkCount_ = 0;
					identRequested_ = false;
				}
			}

			BOOST_FOREACH ( const RequestRTU& request, requestContainer_)
			{
				CXTimer requestPerformance;

				ModbusRequest::DoRequest(request, stream);

				projectMgr::getInstance().GetCacheMgr()->GetItem(systemTags.request_time)->CopyFrom(OPC_QUALITY_GOOD, ComVariant(requestPerformance.GetElapsedSec()));
			}

			Calculate();

			projectMgr::getInstance().GetCacheMgr()->GetItem(systemTags.device_time)->CopyFrom(OPC_QUALITY_GOOD, ComVariant(devicePerformance.GetElapsedSec()));
		}

		void Sirius2Base::SetIdentityRequest( const RequestRTU& request )
		{
			identityRequest_ = request;
		}

		void Sirius2Base::AddRequest( const RequestRTU& request )
		{
			requestContainer_.push_back(request);
		}

	}
}