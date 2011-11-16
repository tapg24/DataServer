#ifndef opc_server_fwd_h__
#define opc_server_fwd_h__

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

namespace Servers 
{
	namespace OPC
	{
		class OPCServer;
	}
}

typedef boost::shared_ptr<Servers::OPC::OPCServer> OPCServerSPtr;
typedef boost::weak_ptr<Servers::OPC::OPCServer> OPCServerWPtr;

#endif // opc_server_fwd_h__
