#ifndef MODBUS_RTU_SERIAL_H
#define MODBUS_RTU_SERIAL_H

#include "utils/types.h"
#include <vector>
#include <string>

#include "channels/channel_modbus/modbus_except.hpp"
#include "modbus_rtu_stream.h"
#include "timeout_serial.h"
#include "crc.h"

class ModbusRTU_Serial : public Channels::Modbus::StreamRTU
{
	TimeoutSerial serial;

public:
	ModbusRTU_Serial();
	~ModbusRTU_Serial();

	// override
	void Open(const std::vector<std::string>& connectionStringArray);
	void Close();
	bool IsOpen() const;
	void SetTimeout(const DWORD timeout);

	const size_t GetRetSize() const;
	void SetBuffer(const BYTE* buffer, const size_t byteCount);
	void GetBuffer(BYTE* buffer, const size_t offset, const size_t byteCount) const;
	void ReadRegister(const int32_t station, const int32_t startAddr, size_t byteCount);
//	void WriteRegister(const int station, const int startAddr, size_t registerCount);
	void ReadDiscretes(const int32_t station, const int32_t startAddr, const size_t byteCount);
	void ReadCoils(const int32_t station, const int32_t startAddr, const size_t byteCount);

private:
	std::string devname_;
	unsigned int baud_rate_;

	BYTE* writeBuffer_;
	BYTE* receiveBuffer_;
	size_t answerByteCount_;
	size_t bytesToWrite_;

	bool do_reOpen;

};

#endif // MODBUS_RTU_H