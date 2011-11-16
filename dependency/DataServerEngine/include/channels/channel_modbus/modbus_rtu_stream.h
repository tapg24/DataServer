#ifndef MODBUS_RTU_STREAM_H
#define MODBUS_RTU_STREAM_H

#include "utils/types.h"
#include <boost/noncopyable.hpp>

namespace Channels
{
	namespace Modbus
	{

		#define READ_BUFFER_SIZE 8
		#define MAX_BUFFER_SIZE 2048
		#define REGISTER_SIZE 2

		class StreamRTU : boost::noncopyable
		{
		public:
			// ctor/dtor
			virtual ~StreamRTU() {}

			// service
			virtual void Open() = 0;
			virtual void Close() = 0;
			virtual bool IsOpen() const = 0;
			virtual void SetTimeout(const DWORD timeout) = 0;

			// data transmis
			virtual void SetBuffer(const BYTE* buffer, const size_t byteCount) = 0;
			virtual void GetBuffer(BYTE* buffer, const size_t offset, const size_t byteCount) const = 0;
			virtual void ReadRegister(const int32_t station, const int32_t startAddr, size_t byteCount) = 0;
//			virtual void WriteRegister(const int32_t station, const int32_t startAddr, size_t registerCount) = 0;
			virtual void ReadDiscretes(const int32_t station, const int32_t startAddr, const size_t byteCount) = 0;
			virtual void ReadCoils(const int32_t station, const int32_t startAddr, const size_t byteCount) = 0;
		};
	}
}

#endif // MODBUS_RTU_STREAM_H