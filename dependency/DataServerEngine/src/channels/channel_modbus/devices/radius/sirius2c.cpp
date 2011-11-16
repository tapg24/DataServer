#include "channels/channel_modbus/devices/radius/sirius2c.h"

#include "core/project_mgr.h"
#include "channels/cache_mgr.h"
#include "utils/timer.hpp"

#include <boost/foreach.hpp>

namespace Channels
{
	namespace Modbus
	{
		Sirius2C::Sirius2C( boost::shared_ptr<Channel>& parent	 // родительский канал
							, const string_t& deviceId			// идентификатор устройства в системе
							, const int32_t deviceType			// тип устройства
							, const int32_t serialId			// серийный номер устройства / пароль
							, const int32_t modbusId			// номер устройства в сети modbus
							, const int32_t primeCurrent		// первичный ток
							, const int32_t secondCurrent		// вторичный ток
							)
			: DeviceBase(parent, deviceId), deviceType_(deviceType), serialId_(serialId)
		{
			identRequested_ = false;
			deviceStatus_ = 0;
			checkCount_ = 0;

			systemTags.request_time = DeviceBase::CatTagPrefix("system.request_time");
			systemTags.device_time = DeviceBase::CatTagPrefix("system.device_time");
			systemTags.deviceStatus = DeviceBase::CatTagPrefix("system.device_status");
			DeviceBase::AddTag(systemTags.request_time, 5);
			DeviceBase::AddTag(systemTags.device_time, 5);
			DeviceBase::AddTag(systemTags.deviceStatus, 17);

			paramTags.I = DeviceBase::CatTagPrefix("I");
			paramTags.Ia = DeviceBase::CatTagPrefix("Ia");
			paramTags.Ib = DeviceBase::CatTagPrefix("Ib");
			paramTags.Ic = DeviceBase::CatTagPrefix("Ic");
			DeviceBase::AddTag(paramTags.I, 4);
			//DeviceBase::AddTag(paramTags.I, _STR("к¬"));
			DeviceBase::AddTag(paramTags.Ia, 4);
			DeviceBase::AddTag(paramTags.Ib, 4);
			DeviceBase::AddTag(paramTags.Ic, 4);

			// identity request
			identityRequest_.station = modbusId;
			identityRequest_.func = 3;
			identityRequest_.startAdress = 4;
			identityRequest_.byteCount = 4;
			identityRequest_.tags.push_back( TagInfo("type", 18) );
			identityRequest_.tags.push_back( TagInfo("serial_code", 18) );

			// data request
			// discrete
			{
				RequestRTU request;
				request.startAdress = modbusId;
				request.func = 2;
				request.startAdress = 72;
				request.byteCount = 2;
				request.tags.push_back( TagInfo("DI16", 18) );
				requestContainer_.push_back(request);
			}
			// analog
			{
				RequestRTU request;
				request.startAdress = modbusId;
				request.func = 2;
				request.startAdress = 1280;
				request.byteCount = 6;
				float factor = static_cast<float>(primeCurrent / secondCurrent / 1000);
				request.tags.push_back( TagInfo("Ia", 4, factor /* к¬ */) );
				request.tags.push_back( TagInfo("Ib", 4, factor /* к¬ */) );
				request.tags.push_back( TagInfo("Ic", 4, factor /* к¬ */) );
				requestContainer_.push_back(request);
			}
		}

		Sirius2C::~Sirius2C()
		{

		}

		void Sirius2C::Calculate()
		{
			float Ia = projectMgr::getInstance().GetCacheMgr()->GetItem(paramTags.Ia)->GetValue().AsFloat();
			float Ib = projectMgr::getInstance().GetCacheMgr()->GetItem(paramTags.Ib)->GetValue().AsFloat();
			float Ic = projectMgr::getInstance().GetCacheMgr()->GetItem(paramTags.Ic)->GetValue().AsFloat();

			float I = (Ia + Ib + Ic) / 3;
			if ( !_finite(I) ) I = 0;

			projectMgr::getInstance().GetCacheMgr()->GetItem(paramTags.I)->CopyFrom(OPC_QUALITY_GOOD, ComVariant(I));
		}

		void Sirius2C::DoIdentityRequest( boost::shared_ptr<StreamRTU>& stream )
		{
			DoRequest(identityRequest_, stream);

			string_t deviceFamilyType = DeviceBase::CatTagPrefix("device_family_type");
			string_t deviceSerialCode = DeviceBase::CatTagPrefix("serial_code");

			// если тип устройства несовпадает, то отключаем опрос устройства
			if ( deviceType_ == projectMgr::getInstance().GetCacheMgr()->GetItem(deviceFamilyType)->GetValue().AsUShort() )
			{
				deviceStatus_ &= ~1;
			}
			else
			{
				deviceStatus_ |=  1;
			}

			// если серийный код устройства не совпадает, то отключаем функцию управлени€ сириусом
			if ( serialId_ == projectMgr::getInstance().GetCacheMgr()->GetItem(deviceSerialCode)->GetValue().AsUShort() )
			{
				deviceStatus_ &= ~2;
			}
			else
			{
				deviceStatus_ |=  2;
			}

			projectMgr::getInstance().GetCacheMgr()->GetItem(systemTags.deviceStatus)->CopyFrom(OPC_QUALITY_GOOD, ComVariant(deviceStatus_));
		}

		void Sirius2C::DoDataRequest( boost::shared_ptr<StreamRTU>& stream )
		{
			CXTimer devicePerformance;

			if ( !identRequested_ )
			{
				DoIdentityRequest(stream);
				identRequested_= true;
			}

			if ( (deviceStatus_ & 1) == 1 )
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

				DoRequest(request, stream);

				projectMgr::getInstance().GetCacheMgr()->GetItem(systemTags.request_time)->CopyFrom(OPC_QUALITY_GOOD, ComVariant(requestPerformance.GetElapsedSec()));
			}

			Calculate();

			projectMgr::getInstance().GetCacheMgr()->GetItem(systemTags.device_time)->CopyFrom(OPC_QUALITY_GOOD, ComVariant(devicePerformance.GetElapsedSec()));
		}

		void Sirius2C::SetIdentityRequest( const RequestRTU& request )
		{
			identityRequest_ = request;
		}

		void Sirius2C::AddRequest( const RequestRTU& request )
		{
			requestContainer_.push_back(request);
		}

	}
}