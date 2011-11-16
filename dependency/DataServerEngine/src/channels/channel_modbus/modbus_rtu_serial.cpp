#include "channels/channel_modbus/modbus_rtu_serial.h"
#include <boost/lexical_cast.hpp>
#include <boost/system/system_error.hpp>

ModbusRTU_Serial::ModbusRTU_Serial()
{
	receiveBuffer_ = new unsigned char[MAX_BUFFER_SIZE];
	memset(receiveBuffer_, 0, MAX_BUFFER_SIZE);

	writeBuffer_ = new unsigned char[MAX_BUFFER_SIZE];
	memset(writeBuffer_, 0, MAX_BUFFER_SIZE);

	answerByteCount_ = 0;
	do_reOpen = false;
}

ModbusRTU_Serial::~ModbusRTU_Serial()
{
	delete receiveBuffer_;
	delete writeBuffer_;
}

void ModbusRTU_Serial::Open(const std::vector<std::string>& connectionStringArray)
{
	devname_ = connectionStringArray[0];
	baud_rate_ = boost::lexical_cast<unsigned int>(connectionStringArray[1]);

	try
	{
		serial.open(devname_, baud_rate_);
	}
	catch (boost::system::system_error&)
	{
		throw serial_port_not_open();
	}
}

void ModbusRTU_Serial::Close()
{
	serial.close();
}

bool ModbusRTU_Serial::IsOpen() const
{
	return serial.isOpen();
}

void ModbusRTU_Serial::SetTimeout(const DWORD timeout)
{
	serial.setTimeout(boost::posix_time::milliseconds(timeout));
}

const size_t ModbusRTU_Serial::GetRetSize() const
{
	return answerByteCount_;
}

void ModbusRTU_Serial::SetBuffer(const BYTE* buffer, const size_t byteCount)
{
	if ( byteCount > MAX_BUFFER_SIZE  )
	{
		throw modbus_error_request();
	}

	memcpy(writeBuffer_, buffer, byteCount);
}

void ModbusRTU_Serial::GetBuffer(BYTE* buffer, const size_t offset, const size_t byteCount) const
{
	if ( byteCount > MAX_BUFFER_SIZE  )
	{
		throw modbus_error_request();
	}

	memcpy(buffer, receiveBuffer_ + offset, byteCount);
}

void ModbusRTU_Serial::ReadRegister(const int32_t station, const int32_t startAddr, const size_t byteCount)
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

	try
	{
		serial.write(readCmd, sizeof(readCmd));
		answerByteCount_ = 0;

		while( tryRequest )
		{
			tryRequest = false;

			answerByteCount_ += serial.read(reciveTmp + offset, sizeof(reciveTmp) - offset);

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
	catch (boost::system::system_error&)
	{
		throw modbus_error_timeout();
	}
}

/*
void ModbusRTU_Serial::WriteRegister(int station, int startAddr, size_t registerCount)
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

	try
	{
		serial.write(writeCmd, totalLen);
		answerByteCount_ = serial.read(reciveTmp, MAX_BUFFER_SIZE);
	}
	catch (boost::system::system_error&)
	{
		serial.CheckPort();
		throw modbus_error_unk();
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

void ModbusRTU_Serial::ReadDiscretes(const int32_t station, const int32_t startAddr, const size_t byteCount)
{
	if ( byteCount > MAX_BUFFER_SIZE )
	{
		throw modbus_error_request();
	}

	size_t bitCount = byteCount * 8;
	size_t requestByteCount = byteCount + 5;
	BYTE offset = 0;
	bool tryRequest = true;

	BYTE readCmd[READ_BUFFER_SIZE];
	memset(readCmd, 0, READ_BUFFER_SIZE);

	BYTE reciveTmp[MAX_BUFFER_SIZE];
	memset(reciveTmp, 0, MAX_BUFFER_SIZE);

	readCmd[0]  = static_cast<BYTE>(station);
	readCmd[1]  = 2;
	readCmd[2]  = static_cast<BYTE>(startAddr / 256);
	readCmd[3]  = static_cast<BYTE>(startAddr % 256);
	readCmd[4]  = static_cast<BYTE>(bitCount / 256);
	readCmd[5]  = static_cast<BYTE>(bitCount % 256);

	Crc crc;
	crc.crc16_ibm(readCmd, 6, readCmd[6], readCmd[7]);

	try
	{
		serial.write(readCmd, sizeof(readCmd));
		answerByteCount_ = 0;

		while( tryRequest )
		{
			tryRequest = false;

			answerByteCount_ += serial.read(reciveTmp + offset, sizeof(reciveTmp) - offset);

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
	catch (boost::system::system_error&)
	{
		throw modbus_error_timeout();
	}
}

void ModbusRTU_Serial::ReadCoils(const int32_t station, const int32_t startAddr, const size_t byteCount)
{
	if ( byteCount > MAX_BUFFER_SIZE )
	{
		throw modbus_error_request();
	}

	size_t bitCount = byteCount * 8;
	size_t requestByteCount = byteCount + 5;
	BYTE offset = 0;
	bool tryRequest = true;

	BYTE readCmd[READ_BUFFER_SIZE];
	memset(readCmd, 0, READ_BUFFER_SIZE);

	BYTE reciveTmp[MAX_BUFFER_SIZE];
	memset(reciveTmp, 0, MAX_BUFFER_SIZE);

	readCmd[0]  = static_cast<BYTE>(station);
	readCmd[1]  = 1;
	readCmd[2]  = static_cast<BYTE>(startAddr / 256);
	readCmd[3]  = static_cast<BYTE>(startAddr % 256);
	readCmd[4]  = static_cast<BYTE>(bitCount / 256);
	readCmd[5]  = static_cast<BYTE>(bitCount % 256);

	Crc crc;
	crc.crc16_ibm(readCmd, 6, readCmd[6], readCmd[7]);

	try
	{
		serial.write(readCmd, sizeof(readCmd));
		answerByteCount_ = 0;

		while( tryRequest )
		{
			tryRequest = false;

			answerByteCount_ += serial.read(reciveTmp + offset, sizeof(reciveTmp) - offset);

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
	catch (boost::system::system_error&)
	{
		throw modbus_error_timeout();
	}
}