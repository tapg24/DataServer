#ifndef MODBUS_STREAM_BASE_H
#define MODBUS_STREAM_BASE_H

namespace Channels
{
	namespace Modbus
	{
		class ModbusStream : boost::noncopyable
		{
		public:
			virtual ~ModbusStream() {}
		};
	}
}

#endif // MODBUS_STREAM_BASE_H