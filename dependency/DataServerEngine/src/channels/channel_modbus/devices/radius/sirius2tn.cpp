#include "channels/channel_modbus/devices/radius/sirius2tn.h"

#include "core/project_mgr.h"
#include "channels/cache_mgr.h"
#include "channels/channel_modbus/devices/modbus_request.h"
#include "utils/timer.hpp"

namespace Channels
{
	namespace Modbus
	{
		Sirius2TN::Sirius2TN( ChannelWPtr parent
			, const string_t& deviceId
			, const int32_t deviceType
			, const int32_t serialId
			, const int32_t modbusId
			, const int32_t primeVoltage
			, const int32_t secondVoltage )
			: Sirius2Base(parent, deviceId, deviceType, "Ñèðèóñ-2-ÒÍ", serialId, modbusId)
		{
			paramTags.f = Sirius2Base::WithPrefix("f");
			paramTags.U = Sirius2Base::WithPrefix("U");
			paramTags.Ua = Sirius2Base::WithPrefix("Ua");
			paramTags.Ub = Sirius2Base::WithPrefix("Ub");
			paramTags.Uc = Sirius2Base::WithPrefix("Uc");

			Sirius2Base::AddTag(paramTags.f, ComVariant(float(0)), OPC_QUALITY_BAD);
			Sirius2Base::AddTag(Sirius2Base::WithPrefix("f_unit"), ComVariant(string_t("Ãö")), OPC_QUALITY_GOOD);
			Sirius2Base::AddTag(paramTags.U, ComVariant(float(0)), OPC_QUALITY_BAD);
			Sirius2Base::AddTag(paramTags.Ua, ComVariant(float(0)), OPC_QUALITY_BAD);
			Sirius2Base::AddTag(paramTags.Ub, ComVariant(float(0)), OPC_QUALITY_BAD);
			Sirius2Base::AddTag(paramTags.Uc, ComVariant(float(0)), OPC_QUALITY_BAD);
			Sirius2Base::AddTag(Sirius2Base::WithPrefix("U_unit"), ComVariant(string_t("êÂ")), OPC_QUALITY_GOOD);

			// data request
			// analog
			{
				RequestRTU request;
				request.startAdress = modbusId;
				request.func = 3;
				request.startAdress = 1280;
				request.byteCount = 8;
				request.tags.push_back( TagInfo(paramTags.f, 4, 0.01 /* Ãö */) );
				float factor = static_cast<float>(primeVoltage / secondVoltage / 1000);
				request.tags.push_back( TagInfo(paramTags.Ua, 4, factor /* êÂ */) );
				request.tags.push_back( TagInfo(paramTags.Ub, 4, factor /* êÂ */) );
				request.tags.push_back( TagInfo(paramTags.Uc, 4, factor /* êÂ */) );
				Sirius2Base::AddRequest(request);
			}
		}

		Sirius2TN::~Sirius2TN()
		{

		}

		void Sirius2TN::Calculate()
		{
			float Ua = projectMgr::getInstance().GetCacheMgr()->GetItem(paramTags.Ua)->GetValue().AsFloat();
			float Ub = projectMgr::getInstance().GetCacheMgr()->GetItem(paramTags.Ub)->GetValue().AsFloat();
			float Uc = projectMgr::getInstance().GetCacheMgr()->GetItem(paramTags.Uc)->GetValue().AsFloat();

			float U = (Ua + Ub + Uc) / 3;
			if ( !_finite(U) ) U = 0;

			projectMgr::getInstance().GetCacheMgr()->GetItem(paramTags.U)->CopyFrom(OPC_QUALITY_GOOD, ComVariant(U));
		}

	}
}