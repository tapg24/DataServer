#ifndef active_object_h__
#define active_object_h__

#include <memory>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/foreach.hpp>

class ActiveObject
{
public:
	ActiveObject() : terminated(false)
	{
		worker_thread.reset(new boost::thread(&ActiveObject::threadMethod, this));
	}
	~ActiveObject()
	{
		stop();
	}

	void post(boost::function<void()> f)
	{
		boost::function<void()> fn(f);
		{
			boost::lock_guard<boost::mutex> guard(mut);
			messages.push_back(fn);
		}
		hasMessages.notify_one();
	}

	template <typename R>
	boost::unique_future<R> post_task(boost::packaged_task<R> task)
	{
		boost::unique_future<R> future = task.get_future();
		boost::function<void()> fn = MoveTaskIntoFunction(task);
		{
			boost::lock_guard<boost::mutex> guard(mut);
			messages.push_back(fn);
		}
		hasMessages.notify_one();
		return boost::move(future);
	}

protected:
	void stop()
	{
		if(!terminated)
		{
			post(boost::bind(&ActiveObject::terminate, this));
			worker_thread->join();
		}
	}

	virtual void on_exception(std::exception*)
	{
		BOOST_ASSERT(!"Unhandled exception in ActiveObject");
	}

private:
	ActiveObject(const ActiveObject&);
	void operator=(const ActiveObject&);

	template <typename R>
	inline boost::function<void()> MoveTaskIntoFunction(boost::packaged_task<R>& task)
	{
		struct MoveTask
		{
			MoveTask(boost::packaged_task<R>& task) : task_(boost::move(task)) {}
			MoveTask(const MoveTask& that) : task_(boost::move(that.task_)) {}

			void operator ()() { return task_(); }

		private:
			mutable boost::packaged_task<R> task_;
		};
		return MoveTask(task);
	}

	void terminate()
	{
		terminated = true;
	}

	void invoke(const boost::function<void()>& f)
	{
		try
		{
			f();
		}
		catch(std::exception& e)
		{
			on_exception(&e);
		}
		catch(...)
		{
			on_exception(NULL);
		}
	}

	void threadMethod()
	{
		while(!terminated)
		{
			std::list<boost::function<void()>> workingCopy;

			{
				boost::unique_lock<boost::mutex> guard(mut);

				if(messages.empty())
				{
					// give a chance to other threads to add messages
					mut.unlock();
					boost::thread::yield();
					mut.lock();

					while(messages.empty())
						hasMessages.wait(guard);
				}

				workingCopy.swap(messages);
			}

			BOOST_FOREACH(boost::function<void()> msg, workingCopy)
			{
				invoke(msg);
			}
		}
	}

	std::auto_ptr<boost::thread> worker_thread;
	boost::mutex mut;
	boost::condition_variable hasMessages;
	std::list<boost::function<void()>> messages;
	bool terminated;
};

#endif // active_object_h__
