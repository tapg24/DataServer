#include "channels/channel_modbus/devices/radius/sirius2yv.h"

#include "core/project_mgr.h"
#include "channels/cache_mgr.h"
#include "channels/channel_modbus/devices/modbus_request.h"
#include "utils/timer.hpp"

#include <numeric>

namespace Channels
{
	namespace Modbus
	{
		Sirius2YV::Sirius2YV( ChannelWPtr parent
			, const string_t& deviceId
			, const int32_t deviceType
			, const int32_t serialId
			, const int32_t modbusId
			, const int32_t primeCurrent
			, const int32_t secondCurrent
			, const int32_t primeVoltage
			, const int32_t secondVoltage )
			: Sirius2Base(parent, deviceId, deviceType, "Ñèðèóñ-2-ÓÂ", serialId, modbusId)
		{
			paramTags.DI16 = Sirius2Base::WithPrefix("DI16");
			paramTags.U = Sirius2Base::WithPrefix("U");
			paramTags.Ua = Sirius2Base::WithPrefix("Ua");
			paramTags.Uax = Sirius2Base::WithPrefix("Uax");
			paramTags.Uay = Sirius2Base::WithPrefix("Uay");
			paramTags.Ub = Sirius2Base::WithPrefix("Ub");
			paramTags.Ubx = Sirius2Base::WithPrefix("Ubx");
			paramTags.Uby = Sirius2Base::WithPrefix("Uby");
			paramTags.Uc = Sirius2Base::WithPrefix("Uc");
			paramTags.Ucx = Sirius2Base::WithPrefix("Ucx");
			paramTags.Ucy = Sirius2Base::WithPrefix("Ucy");
			paramTags.I = Sirius2Base::WithPrefix("I");
			paramTags.Ia = Sirius2Base::WithPrefix("Ia");
			paramTags.Iax = Sirius2Base::WithPrefix("Iax");
			paramTags.Iay = Sirius2Base::WithPrefix("Iay");
			paramTags.Ib = Sirius2Base::WithPrefix("Ib");
			paramTags.Ibx = Sirius2Base::WithPrefix("Ibx");
			paramTags.Iby = Sirius2Base::WithPrefix("Iby");
			paramTags.Ic = Sirius2Base::WithPrefix("Ic");
			paramTags.Icx = Sirius2Base::WithPrefix("Icx");
			paramTags.Icy = Sirius2Base::WithPrefix("Icy");
			paramTags.I2x = Sirius2Base::WithPrefix("I2x");
			paramTags.I2y = Sirius2Base::WithPrefix("I2y");
			paramTags.P = Sirius2Base::WithPrefix("P");
			paramTags.Q = Sirius2Base::WithPrefix("Q");

			Sirius2Base::AddTag(paramTags.DI16, ComVariant(uint16_t(0)), OPC_QUALITY_BAD);
			Sirius2Base::AddTag(paramTags.U, ComVariant(float(0)), OPC_QUALITY_BAD);
			Sirius2Base::AddTag(paramTags.Ua, ComVariant(float(0)), OPC_QUALITY_BAD);
			Sirius2Base::AddTag(paramTags.Uax, ComVariant(float(0)), OPC_QUALITY_BAD);
			Sirius2Base::AddTag(paramTags.Uay, ComVariant(float(0)), OPC_QUALITY_BAD);
			Sirius2Base::AddTag(paramTags.Ub, ComVariant(float(0)), OPC_QUALITY_BAD);
			Sirius2Base::AddTag(paramTags.Ubx, ComVariant(float(0)), OPC_QUALITY_BAD);
			Sirius2Base::AddTag(paramTags.Uby, ComVariant(float(0)), OPC_QUALITY_BAD);
			Sirius2Base::AddTag(paramTags.Uc, ComVariant(float(0)), OPC_QUALITY_BAD);
			Sirius2Base::AddTag(paramTags.Ucx, ComVariant(float(0)), OPC_QUALITY_BAD);
			Sirius2Base::AddTag(paramTags.Ucy, ComVariant(float(0)), OPC_QUALITY_BAD);
			Sirius2Base::AddTag(Sirius2Base::WithPrefix("U_unit"), ComVariant(string_t("êÂ")), OPC_QUALITY_GOOD);
			Sirius2Base::AddTag(paramTags.I, ComVariant(float(0)), OPC_QUALITY_BAD);
			Sirius2Base::AddTag(paramTags.Ia, ComVariant(float(0)), OPC_QUALITY_BAD);
			Sirius2Base::AddTag(paramTags.Iax, ComVariant(float(0)), OPC_QUALITY_BAD);
			Sirius2Base::AddTag(paramTags.Iay, ComVariant(float(0)), OPC_QUALITY_BAD);
			Sirius2Base::AddTag(paramTags.Ib, ComVariant(float(0)), OPC_QUALITY_BAD);
			Sirius2Base::AddTag(paramTags.Ibx, ComVariant(float(0)), OPC_QUALITY_BAD);
			Sirius2Base::AddTag(paramTags.Iby, ComVariant(float(0)), OPC_QUALITY_BAD);
			Sirius2Base::AddTag(paramTags.Ic, ComVariant(float(0)), OPC_QUALITY_BAD);
			Sirius2Base::AddTag(paramTags.Icx, ComVariant(float(0)), OPC_QUALITY_BAD);
			Sirius2Base::AddTag(paramTags.Icy, ComVariant(float(0)), OPC_QUALITY_BAD);
			Sirius2Base::AddTag(paramTags.I2x, ComVariant(float(0)), OPC_QUALITY_BAD);
			Sirius2Base::AddTag(paramTags.I2y, ComVariant(float(0)), OPC_QUALITY_BAD);
			Sirius2Base::AddTag(Sirius2Base::WithPrefix("I_unit"), ComVariant(string_t("À")), OPC_QUALITY_GOOD);
			Sirius2Base::AddTag(paramTags.P, ComVariant(float(0)), OPC_QUALITY_BAD);
			Sirius2Base::AddTag(Sirius2Base::WithPrefix("P_unit"), ComVariant(string_t("ÌÂò")), OPC_QUALITY_GOOD);
			Sirius2Base::AddTag(paramTags.Q, ComVariant(float(0)), OPC_QUALITY_BAD);
			Sirius2Base::AddTag(Sirius2Base::WithPrefix("Q_unit"), ComVariant(string_t("ÌÂð")), OPC_QUALITY_GOOD);

			// data request
			// discrete
			{
				RequestRTU request;
				request.startAdress = modbusId;
				request.func = 2;
				request.startAdress = 72;
				request.byteCount = 2;
				request.tags.push_back( TagInfo(paramTags.DI16, 18) );
				Sirius2Base::AddRequest(request);
			}
			// analog
			{
				RequestRTU request;
				request.startAdress = modbusId;
				request.func = 3;
				request.startAdress = 1294;
				request.byteCount = 28;
				float factorCurrent = static_cast<float>(primeCurrent / secondCurrent);
				request.tags.push_back( TagInfo(paramTags.Iax, 4, factorCurrent /* À */) );
				request.tags.push_back( TagInfo(paramTags.Iay, 4, factorCurrent /* À */) );
				request.tags.push_back( TagInfo(paramTags.Ibx, 4, factorCurrent /* À */) );
				request.tags.push_back( TagInfo(paramTags.Iby, 4, factorCurrent /* À */) );
				request.tags.push_back( TagInfo(paramTags.Icx, 4, factorCurrent /* À */) );
				request.tags.push_back( TagInfo(paramTags.Icy, 4, factorCurrent /* À */) );
				request.tags.push_back( TagInfo(paramTags.I2x, 4, factorCurrent /* À */) );
				request.tags.push_back( TagInfo(paramTags.I2y, 4, factorCurrent /* À */) );
				float factorVoltage = static_cast<float>(primeVoltage / secondVoltage / 1000);
				request.tags.push_back( TagInfo(paramTags.Uax, 4, factorVoltage /* êÂ */) );
				request.tags.push_back( TagInfo(paramTags.Uay, 4, factorVoltage /* êÂ */) );
				request.tags.push_back( TagInfo(paramTags.Ubx, 4, factorVoltage /* êÂ */) );
				request.tags.push_back( TagInfo(paramTags.Uby, 4, factorVoltage /* êÂ */) );
				request.tags.push_back( TagInfo(paramTags.Ucx, 4, factorVoltage /* êÂ */) );
				request.tags.push_back( TagInfo(paramTags.Ucy, 4, factorVoltage /* êÂ */) );
				Sirius2Base::AddRequest(request);
			}
		}

		void Sirius2YV::Calculate()
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

			// Ôè ïîäñ÷èòàåì ïî òîêó
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
}