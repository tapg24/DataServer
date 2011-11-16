#ifndef CRC_H
#define CRC_H

#include "../../utils/types.h"

class Crc
{
public:
	Crc();
	virtual ~Crc();

	USHORT crc16_ibm(BYTE *puchMsg, USHORT sLen, BYTE& byteCRCHi, BYTE& byteCRCLo);
	USHORT crc16_ibm(BYTE *puchMsg, USHORT sLen);
};

#endif // CRC_H