#ifndef event_handler_h__
#define event_handler_h__

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/signals2.hpp>
#include <map>

namespace Core
{
	class EvtType
	{
	protected:
		int id_;
	public:
		EvtType(int id) : id_(id) {}
		~EvtType() {}
	};

	template <typename EventType>
	class EvtHandler
	{
	public:

		typedef boost::signals2::signal< void (EventType) > Signal;
		typedef typename Signal::slot_type Slot;

		EvtHandler() {}
		~EvtHandler() { }

		boost::signals2::connection Bind(const Slot& slot)
		{
			return signal.connect(slot);
		}
		//void UnBind(boost::signals2::connection& connection)
		//{
		//	connection.disconnect();
		//	//signal.disconnect(connection);
		//}
		void Invoke(const EventType& evt)
		{
			signal(evt);
		}

	private:
		Signal signal;
	};

	// usage example:

	//struct FirstEvt : public EvtType
	//{
	//	FirstEvt(int id) : EvtType(id) {}
	//	~FirstEvt() {}
	//	int Get() const { return EvtType::id_; }
	//};
	//typedef EvtHandler<Core::FirstEvt>::Slot FirstEvent;

}

#endif // event_handler_h__
