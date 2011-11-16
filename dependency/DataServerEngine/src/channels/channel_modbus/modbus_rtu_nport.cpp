#include "channels/channel_modbus/modbus_rtu_nport.h"
#include "channels/channel_modbus/crc.h"
#include "channels/channel_modbus/modbus_except.hpp"
#include <wtypes.h>
#include "../../moxa_ipserial/include/IPSerial.h"

int32_t ModbusRTU_NPort::refCounter_ = 0;

ModbusRTU_NPort::ModbusRTU_NPort( const string_t& ip, const int port, const int speed, const int timeout )
: port_id_(port), baudRate_(speed), timeout_(timeout)
{
	ip_.assign(ip.begin(), ip.end());

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

ModbusRTU_NPort::~ModbusRTU_NPort()
{
	delete receiveBuffer_;
	delete writeBuffer_;

	if ( --refCounter_ == 0 )
	{
		nsio_end();
	}
}

void ModbusRTU_NPort::Open()
{
	int alive_value = nsio_checkalive(&ip_[0], 100);
	if ( alive_value != NSIO_OK )
	{
		throw modbus_error_notalive();
	}

	int error_or_port = nsio_open(&ip_[0], port_id_, 3000);
	if ( error_or_port >= 0 )
	{
		//port_id_ = error_or_port;
		nsio_ioctl (port_id_, 0, (BIT_8 | STOP_1 | P_IP_SERIAL_NONE) );
		nsio_baud(port_id_, baudRate_);
		SetTimeout(timeout_);
		isOpen_ = true;
	}
	else
	{
		throw serial_port_not_open();
	}
}

void ModbusRTU_NPort::Close()
{
	nsio_close(port_id_);
	isOpen_ = false;
}

void ModbusRTU_NPort::ResetPort()
{
	nsio_resetport(port_id_, "");
	Sleep(1000);
}

void ModbusRTU_NPort::ResetServer()
{
	nsio_resetserver(&ip_[0], "");
	Sleep(3000);
}

bool ModbusRTU_NPort::IsOpen() const
{
	return isOpen_;
}

void ModbusRTU_NPort::SetTimeout(const DWORD t)
{
	nsio_SetReadTimeouts(port_id_, t);
	nsio_SetWriteTimeouts(port_id_, t);
}

const size_t ModbusRTU_NPort::GetRetSize() const
{
	return answerByteCount_;
}

void ModbusRTU_NPort::SetBuffer(const BYTE* buffer, const size_t byteCount)
{
	if ( byteCount > MAX_BUFFER_SIZE  )
	{
		throw modbus_error_request();
	}

	memcpy(writeBuffer_, buffer, byteCount);
}

void ModbusRTU_NPort::GetBuffer(BYTE* buffer, const size_t offset, const size_t byteCount) const
{
	if ( byteCount > MAX_BUFFER_SIZE  )
	{
		throw modbus_error_request();
	}

	memcpy(buffer, receiveBuffer_ + offset, byteCount);
}

void ModbusRTU_NPort::ReadRegister(int32_t station, int32_t startAddr, size_t byteCount)
{
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

	readCmd[0]  = static_cast<BYTE>(station);
	readCmd[1]  = 3;
	readCmd[2]  = static_cast<BYTE>(startAddr / 256);
	readCmd[3]  = static_cast<BYTE>(startAddr % 256);
	readCmd[4]  = static_cast<BYTE>(byteCount / 256);
	readCmd[5]  = static_cast<BYTE>(byteCount % 256);

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

/*
void ModbusRTU_NPort::WriteRegister(int32_t station, int32_t startAddr, size_t byteCount)
{
	size_t headerSize = 7;

	BYTE writeCmd[MAX_BUFFER_SIZE];
	memset(writeCmd, 0, MAX_BUFFER_SIZE);

	BYTE reciveTmp[MAX_BUFFER_SIZE];
	memset(reciveTmp, 0, MAX_BUFFER_SIZE);

	writeCmd[0]  = static_cast<BYTE>(station);
	writeCmd[1]  = 16;
	writeCmd[2]  = static_cast<BYTE>(startAddr / 256);
	writeCmd[3]  = static_cast<BYTE>(startAddr % 256);
	writeCmd[4]  = 0;
	writeCmd[5]  = static_cast<BYTE>(byteCount);
	writeCmd[6]  = static_cast<BYTE>(byteCount * REGISTER_SIZE);

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
*/

void ModbusRTU_NPort::ReadDiscretes(const int32_t station, const int32_t startAddr, size_t byteCount)
{
	int32_t offset = 0;
	bool tryRequest = true;
	BYTE readCmd[READ_BUFFER_SIZE] = {0};
	BYTE reciveTmp[MAX_BUFFER_SIZE] = {0};

	size_t bitCount = byteCount * 8;

	readCmd[0]  = static_cast<BYTE>(station);
	readCmd[1]  = 2;
	readCmd[2]  = static_cast<BYTE>(startAddr / 256);
	readCmd[3]  = static_cast<BYTE>(startAddr % 256);
	readCmd[4]  = static_cast<BYTE>(bitCount / 256);
	readCmd[5]  = static_cast<BYTE>(bitCount % 256);

	Crc crc;
	crc.crc16_ibm(readCmd, 6, readCmd[6], readCmd[7]);

	int writeLen = nsio_write(port_id_, (char*)readCmd, sizeof(readCmd));
	if ( !writeLen )
	{
		throw modbus_error_timeout();
	}

	answerByteCount_ = 0;

	int32_t readLen = 0;

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

void ModbusRTU_NPort::ReadCoils(const int32_t station, const int32_t startAddr, size_t byteCount)
{
	int32_t offset = 0;
	bool tryRequest = true;
	BYTE readCmd[READ_BUFFER_SIZE] = {0};
	BYTE reciveTmp[MAX_BUFFER_SIZE] = {0};

	size_t bitCount = byteCount * 8;

	readCmd[0]  = static_cast<BYTE>(station);
	readCmd[1]  = 1;
	readCmd[2]  = static_cast<BYTE>(startAddr / 256);
	readCmd[3]  = static_cast<BYTE>(startAddr % 256);
	readCmd[4]  = static_cast<BYTE>(bitCount / 256);
	readCmd[5]  = static_cast<BYTE>(bitCount % 256);

	Crc crc;
	crc.crc16_ibm(readCmd, 6, readCmd[6], readCmd[7]);

	int writeLen = nsio_write(port_id_, (char*)readCmd, sizeof(readCmd));
	if ( !writeLen )
	{
		throw modbus_error_timeout();
	}

	answerByteCount_ = 0;

	int32_t readLen = 0;

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