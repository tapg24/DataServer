#ifndef ACTIVE_OBJECT_ASIO_H
#define ACTIVE_OBJECT_ASIO_H

#define WIN32_LEAN_AND_MEAN

#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>

struct active_object_asio : boost::noncopyable
{

	active_object_asio() : timer(scheduler, boost::posix_time::seconds(15))
	{
		timer.async_wait( boost::bind(&active_object_asio::timer_expired, this, boost::asio::placeholders::error) );
		execution_thread.reset( new boost::thread(boost::bind(&boost::asio::io_service::run, &scheduler)) );
	};

	virtual ~active_object_asio()
	{
		scheduler.stop();
		execution_thread->join();
	};

	void some_operation()
	{
		scheduler.post( boost::bind(&active_object_asio::some_operation_impl, this) );
	};

protected:
	boost::asio::io_service scheduler;

private:
	boost::asio::deadline_timer timer;
	boost::shared_ptr<boost::thread> execution_thread;

	void some_operation_impl ()
	{
		// do something here...
		std::cout << "Do something." << std::endl;
	};

	void timer_expired(const boost::system::error_code& error)
	{
		timer.expires_at (timer.expires_at() + boost::posix_time::seconds(15));
		if (!error)
			timer.async_wait( boost::bind(&active_object_asio::timer_expired, this, boost::asio::placeholders::error) );
	};
};

#endif ACTIVE_OBJECT_ASIO_H