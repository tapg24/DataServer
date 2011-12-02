#include "channels/channel_modbus/devices/modbus_request.h"
#include "channels/channel_modbus/modbus_except.hpp"
#include "channels/channel_modbus/modbus_rtu_stream.h"
#include "core/project_mgr.h"
#include "channels/cache_mgr.h"
#include "utils/bitwise.hpp"
#include "utils/variant.h"
#include <boost/foreach.hpp>

namespace Channels
{
	namespace Modbus
	{
		void ModbusRequest::ProcessData( const RequestRTU& request, boost::shared_ptr<StreamRTU>& stream, const bool swapEndian /* = false */ )
		{
			int32_t offset = 0;
			BYTE buffer[MAX_BUFFER_SIZE] = {0};
			ComVariant variant;

			stream->GetBuffer(buffer, 0, request.byteCount);

			BOOST_FOREACH(const TagInfo& tag, request.tags)
			{
				switch ( tag.type_ )
				{
				case VT_EMPTY:
					{
						offset += static_cast<int32_t>(tag.factor_);
						break;;
					}
				case VT_UI1:
					{
						BYTE tmpValue = 0;
						size_t size = sizeof(tmpValue);
						memcpy(&tmpValue, buffer + offset, size);
						variant = tmpValue;
						offset += size;
						break;
					}
				case VT_UI2:
					{
						WORD tmpValue = 0;
						size_t size = sizeof(tmpValue);
						memcpy(&tmpValue, buffer + offset, size);

						if ( swapEndian ) variant = swap_endian<WORD>(tmpValue);
						else variant = tmpValue;

						offset += size;
						break;
					}
				case VT_UI4:
					{
						DWORD tmpValue = 0;
						size_t size = sizeof(tmpValue);
						memcpy(&tmpValue, buffer + offset, size);

						if ( swapEndian ) variant = swap_endian<DWORD>(tmpValue);
						else variant = tmpValue;

						offset += size;
						break;
					}
				case VT_I4:
					{
						short tmpValue = 0;
						size_t size = sizeof(tmpValue);
						memcpy(&tmpValue, buffer + offset, size);

						if ( swapEndian ) variant = swap_endian<short>(tmpValue);
						else variant = tmpValue;

						if ( tag.factor_ != 1 )
						{
							variant = static_cast<float>(tmpValue) * tag.factor_;
						}
						else
						{
							variant = static_cast<float>(tmpValue);
						}
						offset += size;
						break;
					}
				case VT_R4:
					{
						short tmpValue = 0;
						size_t size = sizeof(tmpValue);
						memcpy(&tmpValue, buffer + offset, size);

						if ( swapEndian ) tmpValue = swap_endian<short>(tmpValue);
						else tmpValue = tmpValue;

						if ( tag.factor_ != 1 )
						{
							double tmpDouble = static_cast<float>(tmpValue) * tag.factor_;
							variant = static_cast<float>(tmpDouble);
						}
						else
						{
							variant = static_cast<float>(tmpValue);
						}
						offset += size;
						break;
					}
				}

				if (  !tag.name_.empty() && projectMgr::getInstance().GetCacheMgr()->ItemExist(tag.name_) )
				{
					projectMgr::getInstance().GetCacheMgr()->GetItem(tag.name_)->CopyFrom(OPC_QUALITY_GOOD, variant);
				}
			}
		}

		void ModbusRequest::DoRequest(const RequestRTU& request, boost::shared_ptr<StreamRTU>& stream)
		{
			try
			{
				switch ( request.func )
				{
				case 1:
					{
						stream->ReadCoils(request.station, request.startAdress, request.byteCount);
						ProcessData(request, stream, false);
						break;
					}
				case 2:
					{
						stream->ReadDiscretes(request.station, request.startAdress, request.byteCount);
						ProcessData(request, stream, false);
						break;
					}
				case 3:
					{
						stream->ReadRegister(request.station, request.startAdress, request.byteCount);
						ProcessData(request, stream, true);
						break;
					}
				}
			}
			catch(modbus_error_notalive&)
			{
				throw;
			}
			catch(modbus_error&)
			{
				BOOST_FOREACH(const TagInfo& tag, request.tags)
				{
					if ( projectMgr::getInstance().GetCacheMgr()->ItemExist(tag.name_) )
					{
						projectMgr::getInstance().GetCacheMgr()->GetItem(tag.name_)->Reset();
					}
				}
			}
		}
	}
}