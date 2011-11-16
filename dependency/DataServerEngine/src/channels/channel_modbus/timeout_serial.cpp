#include "channels/channel_modbus/timeout_serial.h"
#include <boost/bind.hpp>

TimeoutSerial::TimeoutSerial(): io(), port(io), timer(io), timeout(boost::posix_time::seconds(0))
{}

TimeoutSerial::TimeoutSerial(const std::string& devname,
							 unsigned int baud_rate,
							 boost::asio::serial_port_base::parity opt_parity,
							 boost::asio::serial_port_base::character_size opt_csize,
							 boost::asio::serial_port_base::flow_control opt_flow,
							 boost::asio::serial_port_base::stop_bits opt_stop
							 )
							 : io(), port(io), timer(io), timeout(boost::posix_time::seconds(0))
{
    open(devname, baud_rate, opt_parity, opt_csize, opt_flow, opt_stop);
}

void TimeoutSerial::open(const std::string& devname,
						 unsigned int baud_rate,
						 boost::asio::serial_port_base::parity opt_parity,
						 boost::asio::serial_port_base::character_size opt_csize,
						 boost::asio::serial_port_base::flow_control opt_flow,
						 boost::asio::serial_port_base::stop_bits opt_stop
						 )
{
    if(isOpen()) close();
    port.open(devname);
    port.set_option(boost::asio::serial_port_base::baud_rate(baud_rate));
    port.set_option(opt_parity);
    port.set_option(opt_csize);
    port.set_option(opt_flow);
    port.set_option(opt_stop);
	//port.set_option(RTSControl(false));
}

bool TimeoutSerial::isOpen() const
{
    return port.is_open();
}

void TimeoutSerial::close()
{
    if( isOpen() == false ) return;
    port.close();
}

void TimeoutSerial::setTimeout(const boost::posix_time::time_duration& t)
{
    timeout = t;
}

void TimeoutSerial::write(const BYTE* data, size_t size)
{
	writeCondition_ = inProgress;
	boost::asio::async_write( port, boost::asio::buffer(data, size), boost::bind(&TimeoutSerial::writeHandler, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred) );
	
	if(timeout != boost::posix_time::seconds(0))
	{
		timer.expires_from_now(timeout);
	}
	else
	{
		timer.expires_from_now(boost::posix_time::hours(100000));
	}

	timer.async_wait(boost::bind(&TimeoutSerial::writeTimeoutHandler, this, boost::asio::placeholders::error));

	while(true)
	{
		io.run_one();
		if ( writeCondition_ == withoutError )
		{
			return;
		}
		else if ( writeCondition_ == withError )
		{
			throw boost::system::system_error(boost::system::error_code(), "Timeout while write.");
		}
		else
		{
			// NOP
		}		
	}
}

void TimeoutSerial::write(const std::vector<BYTE>& data)
{
    boost::asio::write(port, boost::asio::buffer(&data[0], data.size()));
}

size_t TimeoutSerial::read(BYTE* data, size_t size)
{
	port.async_read_some( boost::asio::buffer(data, size), boost::bind(&TimeoutSerial::readHandler, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred) );

    if(timeout != boost::posix_time::seconds(0))
	{
		timer.expires_from_now(timeout);
	}
    else
	{
		timer.expires_from_now(boost::posix_time::hours(100000));
	}
    
    timer.async_wait(boost::bind(&TimeoutSerial::readTimeoutHandler, this, boost::asio::placeholders::error));
    
    readCondition_ = inProgress;
    bytesTransferred = 0;
    while(true)
    {
        io.run_one();
		if ( readCondition_ == withoutError )
		{
			return bytesTransferred;
		}
		else if ( readCondition_ == withError )
		{
			throw boost::system::system_error(boost::system::error_code(), "Timeout while read.");
		}
		else
		{
			// NOP
		}
	}
}

std::vector<BYTE> TimeoutSerial::read(size_t size)
{
	std::vector<BYTE> result(size, '\0');//Allocate a vector with the desired size
    read(&result[0], size);//Fill it with values
    return result;
}


void TimeoutSerial::flush()
{
	::PurgeComm(port.native(), PURGE_RXABORT || PURGE_RXCLEAR || PURGE_TXABORT || PURGE_TXCLEAR);
}

bool TimeoutSerial::CheckPort()
{
	//DWORD dwModemStatus;
	//GetCommModemStatus(port.native(), &dwModemStatus);

	#define DSR_TIMEOUT 1000L
	DWORD dwModemStatus;
	DWORD dwStartTime, dwNowTime;
	dwStartTime = dwNowTime = GetTickCount();
	do
	{
		GetCommModemStatus(port.native(), &dwModemStatus);
		if (!(dwModemStatus & MS_DSR_ON))
		{
			Sleep(50);
		}

		dwNowTime = GetTickCount();
	}
	while ((!(dwModemStatus & MS_DSR_ON)) && (labs(dwStartTime - dwNowTime) < DSR_TIMEOUT));

	if (!(dwModemStatus & MS_DSR_ON))
	{
		int i = 1;
	}
	else
	{
		int i = 2;
	}

	return false;
}


TimeoutSerial::~TimeoutSerial()
{}

void TimeoutSerial::readTimeoutHandler(const boost::system::error_code& error)
{
	if(error == boost::asio::error::operation_aborted) return;

	readCondition_ = withError;
}

void TimeoutSerial::readHandler(const boost::system::error_code& error, const size_t bytesTransferred)
{
    if( !error )
    {
        readCondition_ = withoutError;
        this->bytesTransferred = bytesTransferred;
    }
}

void TimeoutSerial::writeTimeoutHandler(const boost::system::error_code& error)
{
	if(error == boost::asio::error::operation_aborted) return;

	writeCondition_ = withError;
}

void TimeoutSerial::writeHandler(const boost::system::error_code& error, const size_t bytesTransferred)
{
	if ( !error )
	{
		writeCondition_ = withoutError;
	}
}