#ifndef MODBUS_EXCEPT_HPP
#define MODBUS_EXCEPT_HPP

#include "../../utils/types.h"

class serial_port_not_open : public std::runtime_error
{
public:
	explicit serial_port_not_open () : runtime_error("Невозможно открыть порт.") {}
	virtual ~serial_port_not_open() {}
};

class modbus_error : public std::logic_error
{
public:
	explicit modbus_error (const std::string& what_arg) : logic_error(what_arg) {}
	virtual ~modbus_error() {}
};

class modbus_error_notalive : public modbus_error
{
public:
	explicit modbus_error_notalive () : modbus_error("Ошибка CRC.") {};
	virtual ~modbus_error_notalive() {}
};

class modbus_error_crc : public modbus_error
{
public:
	explicit modbus_error_crc () : modbus_error("Ошибка CRC.") {};
	virtual ~modbus_error_crc() {}
};

class modbus_error_request : public modbus_error
{
public:
	explicit modbus_error_request () : modbus_error("Ошибка конфигурации запроса.") {};
	virtual ~modbus_error_request() {}
};

class modbus_error_unk : public modbus_error
{
public:
	explicit modbus_error_unk () : modbus_error("Неизвестная ошибка.") {};
	virtual ~modbus_error_unk() {}
};

class modbus_error_timeout : public modbus_error
{
public:
	explicit modbus_error_timeout () : modbus_error("Неизвестная ошибка.") {};
	virtual ~modbus_error_timeout() {}
};

class modbus_error_01 : public modbus_error
{
public:
	explicit modbus_error_01 () : modbus_error("Принятый код функции не может быть обработан на подчиненном.") {};
	virtual ~modbus_error_01() {}
};

class modbus_error_02 : public modbus_error
{
public:
	explicit modbus_error_02() : modbus_error("Адрес данных, указанный в запросе, не доступен данному подчиненному.") {};
	virtual ~modbus_error_02() {}
};

class modbus_error_03 : public modbus_error
{
public:
	explicit modbus_error_03() : modbus_error("Величина, содержащаяся в поле данных запроса, является недопустимой величиной для подчиненного.") {};
	virtual ~modbus_error_03() {}
};

class modbus_error_04 : public modbus_error
{
public:
	explicit modbus_error_04() : modbus_error("Невосстанавливаемая ошибка имела место, пока подчиненный пытался выполнить затребованное действие.") {};
	virtual ~modbus_error_04() {}
};

class modbus_error_05 : public modbus_error
{
public:
	explicit modbus_error_05() : modbus_error("Подчиненный принял запрос и обрабатывает его, но это требует много времени. Этот ответ предохраняет главного от генерации ошибки тайм-аута.") {};
	virtual ~modbus_error_05() {}
};

class modbus_error_06 : public modbus_error
{
public:
	explicit modbus_error_06() : modbus_error("Подчиненный занят обработкой команды. Главный должен повторить сообщение позже, когда подчиненный освободится.") {};
	virtual ~modbus_error_06() {}
};

class modbus_error_07 : public modbus_error
{
public:
	explicit modbus_error_07() : modbus_error("Подчиненный не может выполнить программную функцию, принятую в запросе. Этот код возвращается для неудачного программного запроса, использующего функции с номерами 13 или 14. Главный должен запросить диагностическую информацию или информацию об ошибках с подчиненного.") {};
	virtual ~modbus_error_07() {}
};

class modbus_error_08 : public modbus_error
{
public:
	explicit modbus_error_08() : modbus_error("Подчиненный пытается читать расширенную память, но обнаружил ошибку паритета. Главный может повторить запрос, но обычно в таких случаях требуется ремонт.") {};
	virtual ~modbus_error_08() {}
};

#endif // MODBUS_EXCEPT_HPP