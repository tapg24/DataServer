#include "channels/channel_modbus/devices/radius/sirius2c.h"

#include "core/project_mgr.h"
#include "channels/cache_mgr.h"
#include "channels/channel_modbus/devices/modbus_request.h"
#include "utils/timer.hpp"

namespace Channels
{
	namespace Modbus
	{
		Sirius2C::Sirius2C( ChannelWPtr parent	 // родительский канал
							, const string_t& deviceId			// идентификатор устройства в системе
							, const int32_t deviceType			// тип устройства
							, const int32_t serialId			// серийный номер устройства / пароль
							, const int32_t modbusId			// номер устройства в сети modbus
							, const int32_t primeCurrent		// первичный ток
							, const int32_t secondCurrent		// вторичный ток
							)
			: Sirius2Base(parent, deviceId, deviceType, "Сириус-2-С", serialId, modbusId)
		{
			paramTags.DI16 = Sirius2Base::WithPrefix("DI16");
			paramTags.I = Sirius2Base::WithPrefix("I");
			paramTags.Ia = Sirius2Base::WithPrefix("Ia");
			paramTags.Ib = Sirius2Base::WithPrefix("Ib");
			paramTags.Ic = Sirius2Base::WithPrefix("Ic");

			Sirius2Base::AddTag(paramTags.DI16, ComVariant(uint16_t(0)), OPC_QUALITY_BAD);
			Sirius2Base::AddTag(paramTags.I, ComVariant(float(0)), OPC_QUALITY_BAD);
			Sirius2Base::AddTag(paramTags.Ia, ComVariant(float(0)), OPC_QUALITY_BAD);
			Sirius2Base::AddTag(paramTags.Ib, ComVariant(float(0)), OPC_QUALITY_BAD);
			Sirius2Base::AddTag(paramTags.Ic, ComVariant(float(0)), OPC_QUALITY_BAD);
			Sirius2Base::AddTag(Sirius2Base::WithPrefix("I_unit"), ComVariant(string_t("А")), OPC_QUALITY_GOOD);

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
				request.startAdress = 1280;
				request.byteCount = 6;
				float factor = static_cast<float>(primeCurrent / secondCurrent);
				request.tags.push_back( TagInfo(paramTags.Ia, 4, factor /* А */) );
				request.tags.push_back( TagInfo(paramTags.Ib, 4, factor /* А */) );
				request.tags.push_back( TagInfo(paramTags.Ic, 4, factor /* А */) );
				Sirius2Base::AddRequest(request);
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
	}
}