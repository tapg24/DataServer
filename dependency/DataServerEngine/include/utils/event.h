#ifndef EVENT_H
#define EVENT_H

#include <Windows.h>
#include <boost/assert.hpp>
#include <boost/noncopyable.hpp>

namespace Channels
{
	class Event : private boost::noncopyable
	{
	private:
		HANDLE handle;

	public:
		Event();
		~Event();

		void signal();
		void reset();
		void wait( void );
		bool timedWait( unsigned long time_ );
	};
}

#endif // EVENT_H