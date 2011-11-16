#ifndef MODBUS_RTU_H
#define MODBUS_RTU_H

#include "utils/types.h"

#include "timeout_serial.h"

#define READ_BUFFER_SIZE 8
#define MAX_BUFFER_SIZE 2048
#define REGISTER_SIZE 2

class ModbusRTU_Impl
{
	TimeoutSerial serial;

public:
	ModbusRTU_Impl();
	~ModbusRTU_Impl();

	void Open(const std::string& devname, unsigned int baud_rate);
	void Close();
	bool IsOpen() const;
	void SetTimeout(const boost::posix_time::time_duration& t);

	size_t GetRetSize() const;
	HRESULT GetRegisterValue(BYTE* buffer, const size_t offset, const size_t registerCount) const;
	HRESULT SetRegisterValue(const BYTE* buffer, const size_t registerCount);
	void ReadRegister(const int station, const int startAddr, size_t registerCount);
	void WriteRegister(const int station, const int startAddr, size_t registerCount);

	static HRESULT FourByteToFloat(unsigned char byteOneHi, unsigned char byteOneLo, unsigned char byteTwoHi, unsigned char byteTwoLo, float *pfVal);

//protected:
//	ModbusRTU_Impl();
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