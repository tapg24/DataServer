#ifndef DEVICE_ENCODE_H
#define DEVICE_ENCODE_H

#include <string>

namespace Sirius
{
	static std::string GetName(const WORD deviceId)
	{
		switch ( deviceId )
		{
		case 0x30: return "Сириус-РНМ-1";
		case 0x67: return "Сириус-ТЗ";
		case 0x62: return "Сириус-2-С";
		case 0x6B: return "Сириус-ТН";
		case 0x64: return "Сириус-2-МЛ";
		case 0x5D: return "Сириус-УВ";
		case 0x65: return "Сириус-Т";
		case 0x31: return "Спринт-М";
		case 0x69: return "Сириус-СП";
		case 0x42: return "Орион-РТЗ";
		case 0x61: return "Сириус-ДЗ-35";
		case 0x6F: return "";
		case 0x66: return "Сириус-Д";
		case 0x32: return "Сириус-ЦС";
		case 0x46: return "Сириус-АЧР";
		case 0x63: return "Сириус-2-В";
		default: return "Неизв. устр.";
		}
	}
} // Sirius

#endif // DEVICE_ENCODE_H