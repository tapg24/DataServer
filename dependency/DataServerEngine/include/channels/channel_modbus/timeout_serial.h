#ifndef TIMEOUTSERIAL_H
#define	TIMEOUTSERIAL_H

#define WIN32_LEAN_AND_MEAN
#include <boost/asio.hpp>

/**
 * Thrown if timeout occurs
 */
class timeout_exception: public std::runtime_error
{
public:
    timeout_exception(const std::string& arg): runtime_error(arg) {}
};

/**
 * Serial port class, with timeout on read operations.
 */
class TimeoutSerial: private boost::noncopyable
{
public:
    TimeoutSerial();

    /**
     * Opens a serial device. By default timeout is disabled.
     * \param devname serial device name, example "/dev/ttyS0" or "COM1"
     * \param baud_rate serial baud rate
     * \param opt_parity serial parity, default none
     * \param opt_csize serial character size, default 8bit
     * \param opt_flow serial flow control, default none
     * \param opt_stop serial stop bits, default 1
     * \throws boost::system::system_error if cannot open the
     * serial device
     */
    TimeoutSerial(const std::string& devname, unsigned int baud_rate,
        boost::asio::serial_port_base::parity opt_parity=
            boost::asio::serial_port_base::parity(
                boost::asio::serial_port_base::parity::none),
        boost::asio::serial_port_base::character_size opt_csize=
            boost::asio::serial_port_base::character_size(8),
        boost::asio::serial_port_base::flow_control opt_flow=
            boost::asio::serial_port_base::flow_control(
                boost::asio::serial_port_base::flow_control::none),
        boost::asio::serial_port_base::stop_bits opt_stop=
            boost::asio::serial_port_base::stop_bits(
                boost::asio::serial_port_base::stop_bits::one));

    /**
     * Opens a serial device.
     * \param devname serial device name, example "/dev/ttyS0" or "COM1"
     * \param baud_rate serial baud rate
     * \param opt_parity serial parity, default none
     * \param opt_csize serial character size, default 8bit
     * \param opt_flow serial flow control, default none
     * \param opt_stop serial stop bits, default 1
     * \throws boost::system::system_error if cannot open the
     * serial device
     */
    void open(const std::string& devname, unsigned int baud_rate,
        boost::asio::serial_port_base::parity opt_parity=
            boost::asio::serial_port_base::parity(
                boost::asio::serial_port_base::parity::none),
        boost::asio::serial_port_base::character_size opt_csize=
            boost::asio::serial_port_base::character_size(8),
        boost::asio::serial_port_base::flow_control opt_flow=
            boost::asio::serial_port_base::flow_control(
                boost::asio::serial_port_base::flow_control::none),
        boost::asio::serial_port_base::stop_bits opt_stop=
            boost::asio::serial_port_base::stop_bits(
                boost::asio::serial_port_base::stop_bits::one));

    /**
     * \return true if serial device is open
     */
    bool isOpen() const;

    /**
     * Close the serial device
     * \throws boost::system::system_error if any error
     */
    void close();

    /**
     * Set the timeout on read/write operations.
     * To disable the timeout, call setTimeout(boost::posix_time::seconds(0));
     */
    void setTimeout(const boost::posix_time::time_duration& t);

    /**
     * Write data
     * \param data array of char to be sent through the serial device
     * \param size array size
     * \throws boost::system::system_error if any error
     */
    void write(const BYTE* data, size_t size);

     /**
     * Write data
     * \param data to be sent through the serial device
     * \throws boost::system::system_error if any error
     */
    void write(const std::vector<BYTE>& data);

    /**
     * Read some data, blocking
     * \param data array of char to be read through the serial device
     * \param size array size
     * \return numbr of character actually read 0<=return<=size
     * \throws boost::system::system_error if any error
     * \throws timeout_exception in case of timeout
     */
    size_t read(BYTE* data, size_t size);

    /**
     * Read some data, blocking
     * \param size how much data to read
     * \return the receive buffer. It iempty if no data is available
     * \throws boost::system::system_error if any error
     * \throws timeout_exception in case of timeout
     */
    std::vector<BYTE> read(size_t size);

	bool CheckPort();
	void flush();

    ~TimeoutSerial();

private:

    void readTimeoutHandler(const boost::system::error_code& error);
    void readHandler(const boost::system::error_code& error, const size_t bytesTransferred);
	void writeTimeoutHandler(const boost::system::error_code& error);
	void writeHandler(const boost::system::error_code& error, const size_t bytesTransferred);

    enum ReadResult
    {
        resultInProgress,
        resultSuccess,
        resultError,
        resultTimeoutExpired
    };

	enum ProgressCondition
	{
		inProgress,
		withError,
		withoutError
	};

    boost::asio::io_service io; ///< Io service object
    boost::asio::serial_port port; ///< Serial port object
    boost::asio::deadline_timer timer; ///< Timer for timeout
    boost::posix_time::time_duration timeout; ///< Read/write timeout
    boost::asio::streambuf readData; ///< Holds eventual read but not consumed
    enum ReadResult result;  ///< Used by read with timeout
    size_t bytesTransferred; ///< Used by async read callback
	bool isTimeoutExpired;

	enum ProgressCondition writeCondition_;
	enum ProgressCondition readCondition_;
};

class RTSControl 
{ 
public: 
    explicit RTSControl(bool enable = false) : m_enable(enable) {}; 
	boost::system::error_code store(DCB& storage, boost::system::error_code& ec) const 
    { 
        if (m_enable) storage.fRtsControl = RTS_CONTROL_ENABLE; 
        else storage.fRtsControl = RTS_CONTROL_DISABLE; 
        //ec = boost::asio::error::operation_not_supported; 
        //ec = boost::system::error_code(); 
        return ec; 
    }; 

    boost::system::error_code load(const DCB& storage, boost::system::error_code& ec) 
    { 
        if (storage.fRtsControl == RTS_CONTROL_ENABLE) m_enable = true; 
        else m_enable = true; 
        return ec; 
    }; 
private: 
    bool m_enable; 
}; 


#endif  //TIMEOUTSERIAL_H
