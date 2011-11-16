#ifndef active_object_tpl_h__
#define active_object_tpl_h__

#include <queue>

#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/xtime.hpp>

#include <boost/function.hpp>
#include <boost/bind.hpp >

////////////////////////////////////////////////////////////////////////////////////////////////

const int MILLISECONDS_PER_SECOND = 1000;
const int NANOSECONDS_PER_SECOND = 1000000000;
const int NANOSECONDS_PER_MILLISECOND = 1000000;

////////////////////////////////////////////////////////////////////////////////////////////////
inline void to_time(int milliseconds, boost::xtime & xt)
{
	int res = 0;
	res = boost::xtime_get(&xt, boost::TIME_UTC);
	assert(res == boost::TIME_UTC);

	xt.sec += (milliseconds / MILLISECONDS_PER_SECOND);
	xt.nsec += ((milliseconds % MILLISECONDS_PER_SECOND) *
		NANOSECONDS_PER_MILLISECOND);

	if (xt.nsec >= NANOSECONDS_PER_SECOND)
	{
		++xt.sec;
		xt.nsec -= NANOSECONDS_PER_SECOND;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
void sleep(int ms)
{
	boost::xtime delay;
	to_time(ms, delay);
	boost::thread().sleep(delay);
}

////////////////////////////////////////////////////////////////////////////////////////////////

class worker_thread : private boost::noncopyable
{
public:
	typedef boost::function<void()> job;

public:
	worker_thread()
		: stop_(false), thr_(boost::bind(&worker_thread::thread_fun, this))
	{}

	~worker_thread()
	{
		{
			boost::mutex::scoped_lock l(mtx_);
			stop_ = true;
		}
		cnd_.notify_one();
		thr_.join();
	}


	void queue(job const& j)
	{
		{
			boost::mutex::scoped_lock l(mtx_);
			jobs_.push(j);
		}
		cnd_.notify_one();
	}


private:
	void thread_fun()
	{
		job j;
		while(true)
		{
			{
				boost::mutex::scoped_lock l(mtx_);

				while(!stop_ && jobs_.empty())
					cnd_.wait(l);
				if(stop_)
					break;

				jobs_.front().swap(j);
				jobs_.pop();
			}
			j();
		}
	}


private:
	boost::mutex mtx_;
	boost::condition cnd_;
	std::queue<job> jobs_;

	bool stop_;
	boost::thread thr_;
};

////////////////////////////////////////////////////////////////////////////////////////////////

template<class T>
class async_result : private boost::noncopyable
{
public:
	async_result(T const& t = T())
		: result_(t)
		, ready_(false)
	{}

	~async_result()
	{
		static_cast<void>(this->result()); // wait till it has been used by an active object
	}

	void set(T const& t)
	{
		{
			boost::mutex::scoped_lock l(mtx_);
			result_ = t;
			ready_ = true;
		}
		cnd_.notify_all();
	}

	bool ready()
	{
		boost::mutex::scoped_lock l(mtx_);
		return ready_;
	}

	T result()
	{
		boost::mutex::scoped_lock l(mtx_);
		while(!ready_)
			cnd_.wait(l);
		return result_;
	}

private:
	boost::mutex mtx_;
	boost::condition cnd_;
	T result_;
	bool ready_;
};

////////////////////////////////////////////////////////////////////////////////////////////////

template<class T>
struct active_object : private boost::noncopyable
{
	active_object(T const& t = T())
		: object(t)
	{}

	T object;
	worker_thread thread;
};

////////////////////////////////////////////////////////////////////////////////////////////////

#endif // active_object_tpl_h__
