#include "frl_platform.h"
#if( FRL_PLATFORM == FRL_PLATFORM_WIN32 )
#include <vector>
#include "opc/da/server/frl_opc_da_server.h"
#include "../../opc_foundation/opcda.h"
#include "opc/da/server/frl_opc_server_factory.h"

namespace frl{ namespace opc{

DAServer::DAServer( const ServerType& serverType )
	:	ServerKind( serverType )
{
	isInit = False;
}

DAServer::~DAServer()
{
	uninit();
}

Bool DAServer::isInited()
{
	return isInit;
}

void DAServer::registrerServer()
{
	std::vector< IID > categories;
	categories.push_back( ::CATID_OPCDAServer20 );
	ServerKind::registrerServer( categories );
}

void DAServer::registrerServer3()
{
	std::vector< IID > categories;
	categories.push_back( ::CATID_OPCDAServer20 );
	categories.push_back( ::CATID_OPCDAServer30 );
	ServerKind::registrerServer( categories );
}

frl::Bool DAServer::init()
{
	HRESULT hResult = S_OK;
	hResult = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	//hResult = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	if (FAILED(hResult))
	{
		MessageBoxW( NULL, L"Error on CoInitializeEx",  L"Error!", MB_OK | MB_ICONSTOP );
		isInit = False;
		return isInit;
	}

	factory.isOutProc( True );

	hResult = ::CoRegisterClassObject( lexicalCast<frl::String,CLSID>( getCLSID() ), &factory, CLSCTX_LOCAL_SERVER |
		CLSCTX_REMOTE_SERVER, REGCLS_MULTIPLEUSE, &objectId );


	if( FAILED(hResult) )
	{
		MessageBoxW( NULL, L"Error on CoRegistrerClassObject",  L"Error!", MB_OK |MB_ICONSTOP );
		CoUninitialize();
		isInit = False;
		return isInit;
	}			
	isInit = True;
	return isInit;
}

void DAServer::uninit()
{
	if ( isInit )
	{
		//CoRevokeClassObject( objectId );
		CoUninitialize();
		isInit = False;
	}
}

void DAServer::unregisterServer()
{
	ServerKind::unregisterServer();
}

void DAServer::SendShutdownRequest()
{
	factory.SendShutdownRequestToClient();
}

void DAServer::ShutdownServer()
{
	factory.ShutdownServer();
}

bool DAServer::hasConnectedClient()
{
	return factory.hasConnectedClient();
}

bool DAServer::ServerIsLive()
{
	return factory.serverIsLive();
}

} // namespace opc
} // FatRat Library

#endif // FRL_PLATFORM_WIN32
