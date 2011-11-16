#include "channels/channel_modbus/modbus_rtu_nport_impl.h"
#include "channels/channel_modbus/crc.h"
#include "channels/channel_modbus/modbus_except.hpp"
#include <wtypes.h>
#include "../../moxa_ipserial/include/IPSerial.h"

int ModbusRTU_NPORT_Impl::refCounter_ = 0;

ModbusRTU_NPORT_Impl::ModbusRTU_NPORT_Impl()
{
	receiveBuffer_ = new unsigned char[MAX_BUFFER_SIZE];
	memset(receiveBuffer_, 0, MAX_BUFFER_SIZE);

	writeBuffer_ = new unsigned char[MAX_BUFFER_SIZE];
	memset(writeBuffer_, 0, MAX_BUFFER_SIZE);

	answerByteCount_ = 0;

	if ( !refCounter_ )
	{
		nsio_init();
	}
	++refCounter_;
}

ModbusRTU_NPORT_Impl::~ModbusRTU_NPORT_Impl()
{
	delete receiveBuffer_;
	delete writeBuffer_;

	if ( --refCounter_ == 0 )
	{
		nsio_end();
	}
}

void ModbusRTU_NPORT_Impl::Open(const string_t& ip, int port, int baudRate)
{
	ip_.assign(ip.begin(), ip.end());
	ip_.push_back('\0');
	baudRate_ = baudRate;

	int alive_value = nsio_checkalive(&ip_[0], 100);
	if ( alive_value != NSIO_OK )
	{
		throw modbus_error_notalive();
	}

	int error_or_port = nsio_open(&ip_[0], port, 3000);
	if ( error_or_port >= 0 )
	{
		port_id_ = error_or_port;
		nsio_ioctl (port_id_, baudRate, (BIT_8 | STOP_1 | P_IP_SERIAL_NONE) );
		isOpen_ = true;
	}
	else
	{
		throw serial_port_not_open();
	}
}

void ModbusRTU_NPORT_Impl::Close()
{
	nsio_close(port_id_);
	isOpen_ = false;
}

void ModbusRTU_NPORT_Impl::ResetPort()
{
	nsio_resetport(port_id_, "");
	Sleep(1000);
}

void ModbusRTU_NPORT_Impl::ResetServer()
{
	nsio_resetserver(&ip_[0], "");
	Sleep(3000);
}

void ResetServer(const string_t& ip)
{
	std::vector<char> ipVec(ip.begin(), ip.end());
	ipVec.push_back('\0');

	nsio_resetserver(&ipVec[0], "");
	Sleep(3000);
}

bool ModbusRTU_NPORT_Impl::IsOpen() const
{
	return isOpen_;
}

void ModbusRTU_NPORT_Impl::SetTimeout(const DWORD t)
{
	nsio_SetReadTimeouts(port_id_, t);
	nsio_SetWriteTimeouts(port_id_, t);
}

size_t ModbusRTU_NPORT_Impl::GetRetSize() const
{
	return answerByteCount_;
}

void ModbusRTU_NPORT_Impl::SetDiscreteBuffer(const BYTE* buffer, const size_t discreteCount)
{
	size_t byteCount = discreteCount / 8;
	if (discreteCount % 8) byteCount++;

	memcpy(writeBuffer_, buffer, byteCount);
}

void ModbusRTU_NPORT_Impl::GetDiscreteBuffer(BYTE* buffer, const size_t offset, const size_t discreteCount) const
{
	size_t byteCount = discreteCount / 8;
	if (discreteCount % 8) byteCount++;

	if ( byteCount > MAX_BUFFER_SIZE  )
	{
		throw modbus_error_request();
	}

	memcpy(buffer, receiveBuffer_ + offset, byteCount);
}

void ModbusRTU_NPORT_Impl::SetRegisterBuffer(const BYTE* buffer, const size_t registerCount)
{
	memcpy(writeBuffer_, buffer, registerCount * REGISTER_SIZE);
}

void ModbusRTU_NPORT_Impl::GetRegisterBuffer(BYTE* buffer, const size_t offset, const size_t registerCount) const
{
	if ( (offset + registerCount * REGISTER_SIZE) > MAX_BUFFER_SIZE  )
	{
		throw modbus_error_request();
	}

	memcpy(buffer, receiveBuffer_ + offset, registerCount * REGISTER_SIZE);
}

void ModbusRTU_NPORT_Impl::ReadRegister(int station, int startAddr, size_t registerCount)
{
	size_t byteCount = registerCount * REGISTER_SIZE;

	if ( byteCount > MAX_BUFFER_SIZE )
	{
		throw modbus_error_request();
	}

	size_t requestByteCount = byteCount + 5;
	BYTE offset = 0;
	bool tryRequest = true;

	BYTE readCmd[READ_BUFFER_SIZE];
	memset(readCmd, 0, READ_BUFFER_SIZE);

	BYTE reciveTmp[MAX_BUFFER_SIZE];
	memset(reciveTmp, 0, MAX_BUFFER_SIZE);

	readCmd[0]  = station;
	readCmd[1]  = 3;
	readCmd[2]  = startAddr / 256;
	readCmd[3]  = startAddr % 256;
	readCmd[4]  = byteCount / 256;
	readCmd[5]  = byteCount % 256;

	Crc crc;
	crc.crc16_ibm(readCmd, 6, readCmd[6], readCmd[7]);

	int write_len = nsio_write(port_id_, (char*)readCmd, sizeof(readCmd));
	if ( !write_len )
	{
		throw modbus_error_timeout();
	}

	answerByteCount_ = 0;

	int readLen = 0;
	while( tryRequest )
	{
		tryRequest = false;

		readLen = nsio_read(port_id_, (char*)reciveTmp + offset, sizeof(reciveTmp) - offset);
		if ( readLen == 0)
		{
			if( nsio_checkalive(&ip_[0], 10) != NSIO_OK )
			{
				throw modbus_error_notalive();
			}

			throw modbus_error_timeout();
		}
		else if ( readLen < 0 )
		{
			if( nsio_checkalive(&ip_[0], 10) != NSIO_OK )
			{
				throw modbus_error_notalive();
			}
		}

		answerByteCount_ += readLen;

		if ( !answerByteCount_ )
		{
			if( nsio_checkalive(&ip_[0], 10) != NSIO_OK )
			{
				throw modbus_error_notalive();
			}

			throw modbus_error_timeout();
		}

		if ( answerByteCount_ < static_cast<size_t>((reciveTmp[2] + 5)) )
		{
			offset += answerByteCount_;
			tryRequest = true;
			continue;
		}
		else if ( answerByteCount_ == (reciveTmp[2] + 5) )
		{
			offset = 0;
			answerByteCount_ = 0;
		}
		else
		{
			throw modbus_error_unk();
		}

		if ( reciveTmp[1] == 0x83 ) // логическая ошибка запроса
		{
			switch(reciveTmp[3])
			{
			case 0x01 : throw modbus_error_01();
			case 0x02 : throw modbus_error_02();
			case 0x03 : throw modbus_error_03();
			case 0x04 : throw modbus_error_04();
			case 0x05 : tryRequest = true; continue;
			case 0x06 : throw modbus_error_06();
			case 0x07 : throw modbus_error_07();
			case 0x08 : throw modbus_error_08();
			default : throw modbus_error_unk();
			}
		}
		else
		{
			memset(receiveBuffer_, 0, MAX_BUFFER_SIZE); // очистим буфер
			memcpy(receiveBuffer_, reciveTmp + 3, byteCount); // в буффер заносим значения регистров
			return;
		}
	}
}

void ModbusRTU_NPORT_Impl::WriteRegister(int station, int startAddr, size_t registerCount)
{
	size_t headerSize = 7;
	size_t byteCount = registerCount * REGISTER_SIZE;

	BYTE writeCmd[MAX_BUFFER_SIZE];
	memset(writeCmd, 0, MAX_BUFFER_SIZE);

	BYTE reciveTmp[MAX_BUFFER_SIZE];
	memset(reciveTmp, 0, MAX_BUFFER_SIZE);

	writeCmd[0]  = station;
	writeCmd[1]  = 16;
	writeCmd[2]  = startAddr / 256;
	writeCmd[3]  = startAddr % 256;
	writeCmd[4]  = 0;
	writeCmd[5]  = registerCount;
	writeCmd[6]  = byteCount;

	for(size_t i = 0; i < byteCount; i++)
	{
		writeCmd[i + 7] = writeBuffer_[i];
	}

	Crc crc;
	crc.crc16_ibm(writeCmd, headerSize + byteCount, writeCmd[headerSize + byteCount], writeCmd[headerSize + byteCount + 1]);

	DWORD totalLen =  7 + byteCount + 2;

	int writ_err = nsio_write(port_id_, (char*)writeCmd, totalLen);
	answerByteCount_ = nsio_read(port_id_, (char*)reciveTmp, MAX_BUFFER_SIZE);
	
	if ( answerByteCount_ < 0 )
	{
		if( nsio_checkalive(&ip_[0], 10) != NSIO_OK )
		{
			throw modbus_error_notalive();
		}

		throw modbus_error_timeout();
	}

	USHORT answerCrc = crc.crc16_ibm(reciveTmp, answerByteCount_ - 2);
	USHORT correctCrc = (reciveTmp[answerByteCount_ - 2] << 8 | reciveTmp[answerByteCount_ - 1]);

	if ( answerCrc != correctCrc )
	{
		throw modbus_error_crc();
	}

	if( 8 == answerByteCount_ ) // запрос был корректно обработан
	{
		if ( reciveTmp[1] == 0x90 )
		{
			throw modbus_error_03();
		}

		for ( size_t idx = 0; idx < 6; ++idx )
		{
			if ( writeCmd[idx] != reciveTmp[idx] )
			{
				throw modbus_error_unk();
			}
		}
	}
}

void ModbusRTU_NPORT_Impl::ReadDiscretes(const int station, const int startAddr, size_t discreteCount)
{
	int offset = 0;
	bool tryRequest = true;

	size_t byteCount = discreteCount / 8;
	if (discreteCount % 8)
		byteCount++;

	BYTE readCmd[READ_BUFFER_SIZE];
	memset(readCmd, 0, READ_BUFFER_SIZE);

	BYTE reciveTmp[MAX_BUFFER_SIZE];
	memset(reciveTmp, 0, MAX_BUFFER_SIZE);

	readCmd[0]  = station;
	readCmd[1]  = 2;
	readCmd[2]  = startAddr / 256;
	readCmd[3]  = startAddr % 256;
	readCmd[4]  = discreteCount / 256;
	readCmd[5]  = discreteCount % 256;

	Crc crc;
	crc.crc16_ibm(readCmd, 6, readCmd[6], readCmd[7]);

	int writeLen = nsio_write(port_id_, (char*)readCmd, sizeof(readCmd));
	if ( !writeLen )
	{
		throw modbus_error_timeout();
	}

	answerByteCount_ = 0;

	int readLen = 0;

	while( tryRequest )
	{
		tryRequest = false;

		readLen = nsio_read(port_id_, (char*)reciveTmp + offset, sizeof(reciveTmp) - offset);
		if ( readLen == 0)
		{
			if( nsio_checkalive(&ip_[0], 10) != NSIO_OK )
			{
				throw modbus_error_notalive();
			}

			throw modbus_error_timeout();
		}
		else if ( readLen < 0 )
		{
			if( nsio_checkalive(&ip_[0], 10) != NSIO_OK )
			{
				throw modbus_error_notalive();
			}
		}

		answerByteCount_ += readLen;

		if ( !answerByteCount_ )
		{
			if( nsio_checkalive(&ip_[0], 10) != NSIO_OK )
			{
				throw modbus_error_notalive();
			}

			throw modbus_error_timeout();
		}

		if ( answerByteCount_ < static_cast<size_t>(reciveTmp[2] + 5) )
		{
			offset += answerByteCount_;
			tryRequest = true;
			continue;
		}
		else if ( answerByteCount_ == (reciveTmp[2] + 5) )
		{
			offset = 0;
			answerByteCount_ = 0;
		}
		else
		{
			throw modbus_error_unk();
		}

		if ( reciveTmp[1] == 0x83 ) // логическая ошибка запроса
		{
			switch(reciveTmp[3])
			{
			case 0x01 : throw modbus_error_01();
			case 0x02 : throw modbus_error_02();
			case 0x03 : throw modbus_error_03();
			case 0x04 : throw modbus_error_04();
			case 0x05 : tryRequest = true; continue;
			case 0x06 : throw modbus_error_06();
			case 0x07 : throw modbus_error_07();
			case 0x08 : throw modbus_error_08();
			default : throw modbus_error_unk();
			}
		}
		else
		{
			memset(receiveBuffer_, 0, MAX_BUFFER_SIZE); // очистим буфер
			memcpy(receiveBuffer_, reciveTmp + 3, byteCount); // в буффер заносим значения регистров
			return;
		}
	}
}

void ModbusRTU_NPORT_Impl::FourByteToFloat(BYTE byteOneHi, BYTE byteOneLo, BYTE byteTwoHi, BYTE byteTwoLo, float *pfVal)
{
	BYTE byteVal[4];
	byteVal[0] = byteOneLo;
	byteVal[1] = byteOneHi;
	byteVal[2] = byteTwoLo;
	byteVal[3] = byteTwoHi;

	memcpy(pfVal, byteVal, 4);
}