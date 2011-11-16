#include "channels/channel_opc/active_opc_client.h"

ActiveOpcClient::ActiveOpcClient()
{
	worker_thread.reset( new boost::thread(boost::bind(&boost::asio::io_service::run, &scheduler)) );
}

ActiveOpcClient::~ActiveOpcClient()
{
	scheduler.stop();
	worker_thread->join();
}

void ActiveOpcClient::Connect()
{
	scheduler.post( boost::bind(&ActiveOpcClient::ConnectImpl, this) );

}

void ActiveOpcClient::Disconnect()
{
	scheduler.post( boost::bind(&ActiveOpcClient::DisconnectImpl, this) );
}

void ActiveOpcClient::ConnectImpl()
{
	std::cout << "ConnectImpl";
}

void ActiveOpcClient::DisconnectImpl()
{
	std::cout << "DisconnectImpl";
}
