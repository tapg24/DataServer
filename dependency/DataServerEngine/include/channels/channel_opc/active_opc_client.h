#ifndef active_opc_client_h__
#define active_opc_client_h__

#define WIN32_LEAN_AND_MEAN

#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>

//// OPC Client Library
//#include <iostream>
//#include "opc_foundation/opcda.h"
//#include "opc_client/include/OPCClient.h"
//#include "opc_client/include/OPCHost.h"
//#include "opc_client/include/OPCServer.h"
//#include "opc_client/include/OPCGroup.h"
//#include "opc_client/include/OPCItem.h"
//#include <sys\timeb.h>

class ActiveOpcClient
{
public:
	ActiveOpcClient();
	~ActiveOpcClient();

	void Connect();
	void Disconnect();
private:
	boost::asio::io_service scheduler;
	boost::shared_ptr<boost::thread> worker_thread;

	void ConnectImpl();
	void DisconnectImpl();

};

#endif // active_opc_client_h__
