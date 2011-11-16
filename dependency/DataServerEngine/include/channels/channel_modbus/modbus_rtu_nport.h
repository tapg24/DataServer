#ifndef MODBUS_RTU_NPORT_H
#define MODBUS_RTU_NPORT_H

#include "utils/types.h"
#include "utils/string.h"
#include <vector>
#include "modbus_rtu_stream.h"

class ModbusRTU_NPort : public Channels::Modbus::StreamRTU
{
	static int32_t refCounter_;

public:
	ModbusRTU_NPort(const string_t& ip, const int port, const int speed, const int timeout);
	~ModbusRTU_NPort();

	void ResetPort();
	void ResetServer();
// 
	// override
	void Open();
	void Close();
	bool IsOpen() const;
	void SetTimeout(const DWORD t);

	const size_t GetRetSize() const;
	void SetBuffer(const BYTE* buffer, const size_t byteCount);
	void GetBuffer(BYTE* buffer, const size_t offset, const size_t byteCount) const;
	void ReadRegister(const int32_t station, const int32_t startAddr, size_t byteCount);
	//void WriteRegister(const int32_t station, const int32_t startAddr, size_t registerCount);
	void ReadDiscretes(const int32_t station, const int32_t startAddr, const size_t byteCount);
	void ReadCoils(const int32_t station, const int32_t startAddr, const size_t byteCount);

private:
	std::vector<char> ip_;
	int32_t port_id_;
	long baudRate_;
	DWORD timeout_;

	BYTE* writeBuffer_;
	BYTE* receiveBuffer_;
	size_t answerByteCount_;
	size_t bytesToWrite_;

	bool isOpen_;
};

#endif // MODBUS_RTU_NPORT_H