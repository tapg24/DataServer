#include "utils/event.h"

namespace Channels
{
	Event::Event() : handle( CreateEvent(NULL, FALSE, FALSE, NULL) )
	{
		BOOST_ASSERT( handle );
	}

	Event::~Event()
	{
		CloseHandle( handle );
	}

	void Event::signal()
	{
		SetEvent( handle );
	}

	void Event::wait( void )
	{
		WaitForSingleObject( handle, INFINITE );
	}

	bool Event::timedWait( unsigned long time_ )
	{
		DWORD result = WaitForSingleObject( handle, time_ );
		if( result == WAIT_TIMEOUT )
		{
			return false;
		}

		return true; // WAIT_OBJECT_0
	}

	void Event::reset()
	{
		ResetEvent( handle );
	}

}