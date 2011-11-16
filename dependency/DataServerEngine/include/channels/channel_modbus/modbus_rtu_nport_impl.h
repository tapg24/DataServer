#ifndef MODBUS_RTU_IP_H
#define MODBUS_RTU_IP_H

#include "utils/types.h"
#include "utils/string.h"
#include <vector>

#define READ_BUFFER_SIZE 8
#define MAX_BUFFER_SIZE 2048
#define REGISTER_SIZE 2

class ModbusRTU_NPORT_Impl
{
	static int refCounter_;

public:
	ModbusRTU_NPORT_Impl();
	~ModbusRTU_NPORT_Impl();

	void Open(const string_t& ip, int port, int baudRate);
	void Close();
	void ResetPort();
	void ResetServer();
	void ResetServer(const string_t& ip);
	bool IsOpen() const;
	void SetTimeout(const DWORD t);

	size_t GetRetSize() const;
	void SetDiscreteBuffer(const BYTE* buffer, const size_t discreteCount);
	void GetDiscreteBuffer(BYTE* buffer, const size_t offset, const size_t discreteCount) const;
	void SetRegisterBuffer(const BYTE* buffer, const size_t registerCount);
	void GetRegisterBuffer(BYTE* buffer, const size_t offset, const size_t registerCount) const;
	void ReadRegister(const int station, const int startAddr, size_t registerCount);
	void WriteRegister(const int station, const int startAddr, size_t registerCount);
	//void SetCoilValue();
	void ReadDiscretes(const int station, const int startAddr, const size_t discreteCount);

	static void FourByteToFloat(BYTE byteOneHi, BYTE byteOneLo, BYTE byteTwoHi, BYTE byteTwoLo, float *pfVal);

private:
	std::vector<char> ip_;
	unsigned int baudRate_;
	BYTE* writeBuffer_;
	BYTE* receiveBuffer_;
	size_t answerByteCount_;
	size_t bytesToWrite_;

	int port_id_;
	bool isOpen_;
};

#endif // MODBUS_RTU_IP_H