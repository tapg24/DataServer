#include "channels/channel_modbus/devices/radius/sirius2old_device.h"
#include "core/project_mgr.h"
#include "channels/cache_mgr.h"
#include "utils/timer.hpp"
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/assign/std/vector.hpp>
#include <numeric>

namespace Channels
{
	namespace Modbus
	{

		Sirius2Old::Sirius2Old(ChannelWPtr parent, const string_t& deviceId, const int32_t deviceType, const int32_t serialId, const int32_t modbusId, const bool calc)
			: ModbusDeviceBase(parent, deviceId), deviceId_(deviceId), deviceType_(deviceType), serialId_(serialId), modbusId_(modbusId), calculateParam_(calc)
		{
			identRequested_ = false;
			deviceStatus_ = 0;
			checkCount_ = 0;

			circularBuf_P.set_capacity(30);
			circularBuf_Q.set_capacity(30);

			systemTags.request_time = SetTagPrefix("system.request_time");
			systemTags.device_time = SetTagPrefix("system.device_time");
			systemTags.deviceStatus = SetTagPrefix("system.device_status");
//			DeviceBase::AddTag(systemTags.request_time, 5);
//			DeviceBase::AddTag(systemTags.device_time, 5);
//			DeviceBase::AddTag(systemTags.deviceStatus, 17);
		}

		Sirius2Old::~Sirius2Old()
		{

		}

		void Sirius2Old::DoIdentityRequest(boost::shared_ptr<StreamRTU>& stream)
		{
			InitParamTags();

			DoRequest(identityRequest_, stream);

			string_t deviceFamilyType = deviceId_ + ".device_family_type";
			string_t deviceSerialCode = deviceId_ + ".serial_code";

			// если тип устройства несовпадает, то отключаем опрос устройства
			if ( deviceType_ == projectMgr::getInstance().GetCacheMgr()->GetItem(deviceFamilyType)->GetValue().AsUShort() )
			{
				deviceStatus_ &= ~1;
			}
			else
			{
				deviceStatus_ |=  1;
			}

			// если серийный код устройства не совпадает, то отключаем функцию управления сириусом
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

		void Sirius2Old::DoDataRequest(boost::shared_ptr<StreamRTU>& stream)
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

			CalcParamTags();

			projectMgr::getInstance().GetCacheMgr()->GetItem(systemTags.device_time)->CopyFrom(OPC_QUALITY_GOOD, ComVariant(devicePerformance.GetElapsedSec()));
		}

  		string_t Sirius2Old::SetTagPrefix( const string_t& tagName )
  		{
  			string_t newTagName = boost::lexical_cast<string_t>(deviceId_) + "." + tagName;
  			return newTagName;
  		}
  
 		void Sirius2Old::SetTagPrefix( std::vector<string_t>& tagNameArray )
 		{
 			BOOST_FOREACH(string_t& tagName, tagNameArray)
 			{
 				tagName = SetTagPrefix(tagName);
 			}
 		}
 
		void Sirius2Old::InitParamTags()
		{
			switch ( deviceType_ )
			{
			case 98:
				InitDeviceTypeC();
				break;
			case 107:
				InitDeviceTypeTN();
				break;
			default:
				InitDeviceTypeOthers();
				break;
			}
		}

		void Sirius2Old::InitDeviceTypeTN()
		{
			using namespace boost::assign;

			paramTags.U = SetTagPrefix("U");
			paramTags.Ua = SetTagPrefix("Ua");
			paramTags.Ub = SetTagPrefix("Ub");
			paramTags.Uc = SetTagPrefix("Uc");

			std::vector<string_t> params;
			params += paramTags.Ua, paramTags.Ub, paramTags.Uc;
			BOOST_FOREACH(const string_t& paramName, params)
			{

				if ( !TagExist(paramName) )
				{
					calculateParam_ = false;
					break;
				}
			}

			if ( calculateParam_ )
			{
				std::vector<string_t> calcTags;
				calcTags += paramTags.U;
				BOOST_FOREACH(const string_t& tagName, calcTags)
				{
//					AddTag(tagName, 4);
				}
			}
		}

		void Sirius2Old::InitDeviceTypeC()
		{
			using namespace boost::assign;

			paramTags.I = SetTagPrefix("I");
			paramTags.Ia = SetTagPrefix("Ia");
			paramTags.Ib = SetTagPrefix("Ib");
			paramTags.Ic = SetTagPrefix("Ic");

			std::vector<string_t> params;
			params += paramTags.Ia, paramTags.Ib, paramTags.Ic;
			BOOST_FOREACH(const string_t& paramName, params)
			{

				if ( !ModbusDeviceBase::TagExist(paramName) )
				{
					calculateParam_ = false;
					break;
				}
			}

			if ( calculateParam_ )
			{
				std::vector<string_t> calcTags;
				calcTags += paramTags.I;
				BOOST_FOREACH(const string_t& tagName, calcTags)
				{
//					AddTag(tagName, 4);
				}
			}
		}

		void Sirius2Old::InitDeviceTypeOthers()
		{
			using namespace boost::assign;

			paramTags.I = SetTagPrefix("I");
			paramTags.Ia = SetTagPrefix("Ia");
			paramTags.Iax = SetTagPrefix("Iax");
			paramTags.Iay = SetTagPrefix("Iay");
			paramTags.Ib = SetTagPrefix("Ib");
			paramTags.Ibx = SetTagPrefix("Ibx");
			paramTags.Iby = SetTagPrefix("Iby");
			paramTags.Ic = SetTagPrefix("Ic");
			paramTags.Icx = SetTagPrefix("Icx");
			paramTags.Icy = SetTagPrefix("Icy");

			paramTags.U = SetTagPrefix("U");
			paramTags.Ua = SetTagPrefix("Ua");
			paramTags.Uax = SetTagPrefix("Uax");
			paramTags.Uay = SetTagPrefix("Uay");
			paramTags.Ub = SetTagPrefix("Ub");
			paramTags.Ubx = SetTagPrefix("Ubx");
			paramTags.Uby = SetTagPrefix("Uby");
			paramTags.Uc = SetTagPrefix("Uc");
			paramTags.Ucx = SetTagPrefix("Ucx");
			paramTags.Ucy = SetTagPrefix("Ucy");

			paramTags.Q = SetTagPrefix("Q");
			paramTags.P = SetTagPrefix("P");
			

			std::vector<string_t> params;
			params += paramTags.Iax, paramTags.Iay, paramTags.Ibx, paramTags.Iby, paramTags.Icx, paramTags.Icy;
			BOOST_FOREACH(const string_t& paramName, params)
			{

				if ( !TagExist(paramName) )
				{
					calculateParam_ = false;
					break;
				}
			}

			if ( calculateParam_ )
			{
				std::vector<string_t> calcTags;
				calcTags += paramTags.Ia, paramTags.Ib, paramTags.Ic, paramTags.Ua, paramTags.Ub, paramTags.Uc, paramTags.I, paramTags.U, paramTags.P, paramTags.Q;
				BOOST_FOREACH(const string_t& tagName, calcTags)
				{
//					AddTag(tagName, 4);
				}
			}
		}

		void Sirius2Old::CalcParamTags()
		{
			if ( calculateParam_ )
			{
				switch ( deviceType_ )
				{
				case 98:
					CalcDeviceTypeC();
					break;
				case 107:
					CalcDeviceTypeTN();
					break;
				default:
					CalcDeviceTypeOthers();
					break;
				}
			}
		}

		void Sirius2Old::CalcDeviceTypeOthers()
		{
			if ( calculateParam_ )
			{
				float Iax = projectMgr::getInstance().GetCacheMgr()->GetItem(paramTags.Iax)->GetValue().AsFloat();
				float Iay = projectMgr::getInstance().GetCacheMgr()->GetItem(paramTags.Iay)->GetValue().AsFloat();
				float Ibx = projectMgr::getInstance().GetCacheMgr()->GetItem(paramTags.Ibx)->GetValue().AsFloat();
				float Iby = projectMgr::getInstance().GetCacheMgr()->GetItem(paramTags.Iby)->GetValue().AsFloat();
				float Icx = projectMgr::getInstance().GetCacheMgr()->GetItem(paramTags.Icx)->GetValue().AsFloat();
				float Icy = projectMgr::getInstance().GetCacheMgr()->GetItem(paramTags.Icy)->GetValue().AsFloat();
				float Uax = projectMgr::getInstance().GetCacheMgr()->GetItem(paramTags.Uax)->GetValue().AsFloat();
				float Uay = projectMgr::getInstance().GetCacheMgr()->GetItem(paramTags.Uay)->GetValue().AsFloat();
				float Ubx = projectMgr::getInstance().GetCacheMgr()->GetItem(paramTags.Ubx)->GetValue().AsFloat();
				float Uby = projectMgr::getInstance().GetCacheMgr()->GetItem(paramTags.Uby)->GetValue().AsFloat();
				float Ucx = projectMgr::getInstance().GetCacheMgr()->GetItem(paramTags.Ucx)->GetValue().AsFloat();
				float Ucy = projectMgr::getInstance().GetCacheMgr()->GetItem(paramTags.Ucy)->GetValue().AsFloat();

				// Фи подсчитаем по току
				float Fa = std::atan(Iay / Iax);
				if ( !_finite(Fa) ) Fa = 0;
				float Fb = std::atan(Iby / Ibx);
				if ( !_finite(Fb) ) Fb = 0;
				float Fc = std::atan(Icy / Icx);
				if ( !_finite(Fc) ) Fc = 0;
				float F = (Fa + Fb + Fc) / 3;
				if ( !_finite(F) ) F = 0;

				float Fai = std::atan(Iay / Iax);
				float Fau = std::atan(Uay / Uax);
				float cosF, sinF;
				if( Fai > Fau )
				{
					cosF = abs(std::cos(Fai - Fau));
					sinF = abs(std::sin(Fai - Fau));
				}
				else
				{
					cosF = abs(std::cos(Fau - Fai));
					sinF = abs(std::sin(Fau - Fai));
				}

				float Ia = std::pow((std::pow(Iax, 2.0f) + std::pow(Iay, 2.0f)), 0.5f);
				float Ib = std::pow((std::pow(Ibx, 2.0f) + std::pow(Iby, 2.0f)), 0.5f);
				float Ic = std::pow((std::pow(Icx, 2.0f) + std::pow(Icy, 2.0f)), 0.5f);
				float I = (Ia + Ib + Ic) / 3;
				float Ua = std::pow((std::pow(Uax, 2.0f) + std::pow(Uay, 2.0f)), 0.5f);
				float Ub = std::pow((std::pow(Ubx, 2.0f) + std::pow(Uby, 2.0f)), 0.5f);
				float Uc = std::pow((std::pow(Ucx, 2.0f) + std::pow(Ucy, 2.0f)), 0.5f);
				float U = (Ua + Ub + Uc) / 3;

				float S = 1.72f * U * I / 1000;

				float P = S * cosF;
				if ( !_finite(P) ) P = 0;
				float Q = S * sinF;
				if ( !_finite(Q) ) Q = 0;

				circularBuf_P.push_back(P);
				float mean_P = std::accumulate(circularBuf_P.begin(), circularBuf_P.end(), 0.0f) / circularBuf_P.size();

				circularBuf_Q.push_back(Q);
				float mean_Q = std::accumulate(circularBuf_Q.begin(), circularBuf_Q.end(), 0.0f) / circularBuf_Q.size();

				projectMgr::getInstance().GetCacheMgr()->GetItem(paramTags.I)->CopyFrom(OPC_QUALITY_GOOD, ComVariant(I));
				projectMgr::getInstance().GetCacheMgr()->GetItem(paramTags.Ia)->CopyFrom(OPC_QUALITY_GOOD, ComVariant(Ia));
				projectMgr::getInstance().GetCacheMgr()->GetItem(paramTags.Ib)->CopyFrom(OPC_QUALITY_GOOD, ComVariant(Ib));
				projectMgr::getInstance().GetCacheMgr()->GetItem(paramTags.Ic)->CopyFrom(OPC_QUALITY_GOOD, ComVariant(Ic));
				projectMgr::getInstance().GetCacheMgr()->GetItem(paramTags.U)->CopyFrom(OPC_QUALITY_GOOD, ComVariant(U));
				projectMgr::getInstance().GetCacheMgr()->GetItem(paramTags.Ua)->CopyFrom(OPC_QUALITY_GOOD, ComVariant(Ua));
				projectMgr::getInstance().GetCacheMgr()->GetItem(paramTags.Ub)->CopyFrom(OPC_QUALITY_GOOD, ComVariant(Ub));
				projectMgr::getInstance().GetCacheMgr()->GetItem(paramTags.Uc)->CopyFrom(OPC_QUALITY_GOOD, ComVariant(Uc));
				projectMgr::getInstance().GetCacheMgr()->GetItem(paramTags.P)->CopyFrom(OPC_QUALITY_GOOD, ComVariant(mean_P));
				projectMgr::getInstance().GetCacheMgr()->GetItem(paramTags.Q)->CopyFrom(OPC_QUALITY_GOOD, ComVariant(mean_Q));
			}
		}

		void Sirius2Old::CalcDeviceTypeTN()
		{
			if ( calculateParam_ )
			{
				float Ua = projectMgr::getInstance().GetCacheMgr()->GetItem(paramTags.Ua)->GetValue().AsFloat();
				float Ub = projectMgr::getInstance().GetCacheMgr()->GetItem(paramTags.Ub)->GetValue().AsFloat();
				float Uc = projectMgr::getInstance().GetCacheMgr()->GetItem(paramTags.Uc)->GetValue().AsFloat();

				float U = (Ua + Ub + Uc) / 3;
				if ( !_finite(U) ) U = 0;

				projectMgr::getInstance().GetCacheMgr()->GetItem(paramTags.U)->CopyFrom(OPC_QUALITY_GOOD, ComVariant(U));
			}
		}

		void Sirius2Old::CalcDeviceTypeC()
		{
			if ( calculateParam_ )
			{
				float Ia = projectMgr::getInstance().GetCacheMgr()->GetItem(paramTags.Ia)->GetValue().AsFloat();
				float Ib = projectMgr::getInstance().GetCacheMgr()->GetItem(paramTags.Ib)->GetValue().AsFloat();
				float Ic = projectMgr::getInstance().GetCacheMgr()->GetItem(paramTags.Ic)->GetValue().AsFloat();

				float I = (Ia + Ib + Ic) / 3;
				if ( !_finite(I) ) I = 0;

				projectMgr::getInstance().GetCacheMgr()->GetItem(paramTags.I)->CopyFrom(OPC_QUALITY_GOOD, ComVariant(I));
			}
		}

		void Sirius2Old::SetIdentityRequest( const RequestRTU& request )
		{
			identityRequest_ = request;
//			DeviceBase::AddTags(request.tags);
		}

		void Sirius2Old::AddRequest( const RequestRTU& request )
		{
			requestContainer_.push_back(request);
//			DeviceBase::AddTags(request.tags);
		}

	}
}