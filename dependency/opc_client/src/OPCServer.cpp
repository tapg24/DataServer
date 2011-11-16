/*
OPCClientToolKit
Copyright (C) 2005 Mark C. Beharrell

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public
License as published by the Free Software Foundation; either
version 2 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public
License along with this library; if not, write to the
Free Software Foundation, Inc., 59 Temple Place - Suite 330,
Boston, MA  02111-1307, USA.
*/

#include "OPCServer.h"

// implement IOPCShutdown
//////////////////////////////////////////////////////////////////////////
// TODO: move to ConnectionPoint module
class COPCServer::COPCShutdownRequest : public IOPCShutdown
{
private:
	DWORD refCount;
	DWORD callbackHandle;
	IShutdownRequestCallback* callback_;

public:

	COPCShutdownRequest() : refCount(0), callback_(NULL)
	{} 

	~COPCShutdownRequest()
	{
		callback_ = NULL;
	} 

	STDMETHODIMP QueryInterface( REFIID iid, LPVOID* ppInterface)
	{
		if ( ppInterface == NULL) return E_INVALIDARG;

		if ( iid == IID_IUnknown ) *ppInterface = (IUnknown*) this;
		else if ( iid == IID_IOPCShutdown) *ppInterface = (IOPCDataCallback*) this;
		else
		{
			*ppInterface = NULL;
			return E_NOINTERFACE;
		}

		AddRef();
		return S_OK;
	}

	STDMETHODIMP_(ULONG) AddRef()
	{
		return ++refCount;
	}


	STDMETHODIMP_(ULONG) Release()
	{
		--refCount; 

		if ( refCount == 0) delete this;
		return refCount;
	}



	// The OPC Callback function 
	void SetShutdownRequestCallback(IShutdownRequestCallback* callback)
	{
		if ( !callback )
		{
			throw OPCException("callback is NULL", S_FALSE);
		}

		callback_ = callback;
	}

	STDMETHODIMP ShutdownRequest(LPCWSTR szReason)
	{
		if ( !callback_ ) return S_FALSE;

		CString reason(szReason);
		callback_->ShutdownRequest(reason);
		return S_OK;
	}
};

//////////////////////////////////////////////////////////////////////////

COPCServer::COPCServer(ATL::CComPtr<IOPCServer> &opcServerInterface)
{
	iOpcServer = opcServerInterface;

	HRESULT res = opcServerInterface->QueryInterface(IID_IOPCBrowseServerAddressSpace, (void**)&iOpcNamespace);
	if (FAILED(res)){
		throw OPCException("Failed to obtain IID_IOPCBrowseServerAddressSpace interface",res);
	}

	res = opcServerInterface->QueryInterface(IID_IOPCItemProperties, (void**)&iOpcProperties);
	if (FAILED(res)){
		throw OPCException("Failed to obtain IID_IOPCItemProperties interface",res);
	}

	res = opcServerInterface->QueryInterface(IID_IConnectionPointContainer, (void**)&iOpcConnections);
	if (FAILED(res)){
		throw OPCException("Failed to obtain IID_IConnectionPointContainer interface",res);
	}
}



COPCServer::~COPCServer()
{
	CComPtr<IConnectionPoint> connectionPoint = NULL;
	HRESULT res = iOpcConnections->FindConnectionPoint(IID_IOPCShutdown, &connectionPoint);
	if ( FAILED(res) )
	{
	}
	else
	{
		connectionPoint->Unadvise(shutdownCookie);
	}
}

COPCGroup *COPCServer::makeGroup(const CString & groupName, bool active, unsigned long reqUpdateRate_ms, unsigned long &revisedUpdateRate_ms, float deadBand, const DWORD keepAlive, DWORD& revisedKeepAlive)
{
	return new COPCGroup(groupName, active, reqUpdateRate_ms, revisedUpdateRate_ms, deadBand, keepAlive, revisedKeepAlive, *this);
}

void COPCServer::getAllItemNames(CAtlArray<CString> & opcItemNames)
{
	if (!iOpcNamespace) return;

	OPCNAMESPACETYPE nameSpaceType;
	HRESULT result = iOpcNamespace->QueryOrganization(&nameSpaceType);

	USES_CONVERSION;
	int v = 0;
	WCHAR emptyString[] = {0};
	//result = iOpcNamespace->ChangeBrowsePosition(OPC_BROWSE_TO, emptyString);

	ATL::CComPtr<IEnumString> iEnum;
	result = iOpcNamespace->BrowseOPCItemIDs(OPC_FLAT,emptyString,VT_EMPTY,0,(&iEnum));
	if ( FAILED(result) )
	{
		return;
	}
	 
    WCHAR * str;
    ULONG strSize;
	while( (result = iEnum->Next(1, &str, &strSize)) == S_OK )
	{
		WCHAR * fullName;
		result = iOpcNamespace->GetItemID(str, &fullName);
		if ( SUCCEEDED(result) )
		{
			USES_CONVERSION;
			char * cStr = OLE2T(fullName);
			//char * cStr = OLE2T(str);
			//printf("Adding %s\n", cStr);
			opcItemNames.Add(cStr);
			COPCClient::comFree(fullName);
		}
		COPCClient::comFree(str);
	}
}

void COPCServer::getItemNames(const WCHAR* branchName, CAtlArray<CString> & opcItemNames)
{
	if (!iOpcNamespace) return;

	OPCNAMESPACETYPE nameSpaceType;
	WCHAR emptyString[] = {0};
	ATL::CComPtr<IEnumString> iEnum;
	ATL::CComPtr<IEnumString> iEnumLeaf;
	ATL::CComPtr<IEnumString> iEnumBranch;
	WCHAR* str;
	ULONG strSize;

	HRESULT result = iOpcNamespace->QueryOrganization(&nameSpaceType);

	if (nameSpaceType == OPC_NS_HIERARCHIAL)
	{
		if ( wcscmp(branchName, emptyString) != NULL )
		{
			result = iOpcNamespace->ChangeBrowsePosition(OPC_BROWSE_TO, branchName);
		} 
		else
		{
			iOpcNamespace->ChangeBrowsePosition(OPC_BROWSE_TO, emptyString);
		}

		result = iOpcNamespace->BrowseOPCItemIDs(OPC_LEAF, emptyString, VT_EMPTY, 0, &iEnumBranch);
		if (FAILED(result))
		{
			return;
		}

		while( result = iEnumBranch->Next(1, &str, &strSize) == S_OK )
		{
			WCHAR * fullName;
			result = iOpcNamespace->GetItemID(str, &fullName);
			if ( SUCCEEDED(result) )
			{
				USES_CONVERSION;
				char * cStr = OLE2T(fullName);
				opcItemNames.Add(cStr);
				COPCClient::comFree(fullName);
			}
			COPCClient::comFree(str);
		}
	}
}

void COPCServer::getBrancheNames( const WCHAR* branchName, CAtlArray<CString> & opcBrancheNames )
{
	if (!iOpcNamespace) return;

	OPCNAMESPACETYPE nameSpaceType;
	WCHAR emptyString[] = {0};
	ATL::CComPtr<IEnumString> iEnum;
	ATL::CComPtr<IEnumString> iEnumLeaf;
	ATL::CComPtr<IEnumString> iEnumBranch;
	WCHAR* str;
	ULONG strSize;

	HRESULT result = iOpcNamespace->QueryOrganization(&nameSpaceType);

	if (nameSpaceType == OPC_NS_HIERARCHIAL)
	{
		if ( wcscmp(branchName, emptyString) != NULL )
		{
			result = iOpcNamespace->ChangeBrowsePosition(OPC_BROWSE_TO, branchName);
		} 
		else
		{
			iOpcNamespace->ChangeBrowsePosition(OPC_BROWSE_TO, emptyString);
		}

		result = iOpcNamespace->BrowseOPCItemIDs(OPC_BRANCH, emptyString, VT_EMPTY, 0, &iEnumBranch);
		if (FAILED(result))
		{
			return;
		}

		while( result = iEnumBranch->Next(1, &str, &strSize) == S_OK )
		{
			WCHAR * fullName;
			result = iOpcNamespace->GetItemID(str, &fullName);
			if ( SUCCEEDED(result) )
			{
				USES_CONVERSION;
				char * cStr = OLE2T(fullName);
				opcBrancheNames.Add(cStr);
				COPCClient::comFree(fullName);
			}
			COPCClient::comFree(str);
		}
	}
}



void COPCServer::getStatus(ServerStatus &status)
{
	OPCSERVERSTATUS *serverStatus;
	HRESULT result = iOpcServer->GetStatus(&serverStatus);
	if ( FAILED(result) )
	{
		throw OPCException("Failed to get status");
	}

	status.ftStartTime = serverStatus->ftStartTime;
    status.ftCurrentTime = serverStatus->ftCurrentTime;
    status.ftLastUpdateTime = serverStatus->ftLastUpdateTime;
    status.dwServerState = serverStatus->dwServerState;
    status.dwGroupCount = serverStatus->dwGroupCount;
    status.dwBandWidth = serverStatus->dwBandWidth;
    status.wMajorVersion = serverStatus->wMajorVersion;
    status.wMinorVersion = serverStatus->wMinorVersion;
    status.wBuildNumber = serverStatus->wBuildNumber;
	if (serverStatus->szVendorInfo != NULL)
	{
		USES_CONVERSION;
		status.vendorInfo = OLE2T(serverStatus->szVendorInfo);
		COPCClient::comFree(serverStatus->szVendorInfo);
	}
	COPCClient::comFree(serverStatus);
}

void COPCServer::setShutdownRequestCallback( IShutdownRequestCallback* callback )
{
	if ( iOpcConnections )
	{
		CComPtr<IConnectionPoint> connectionPoint = NULL;
		HRESULT res = iOpcConnections->FindConnectionPoint(IID_IOPCShutdown, &connectionPoint);
		if ( FAILED(res) )
		{
			throw OPCException("Failed to obtain IID_IOPCShutdown interface", res);
		}
		else
		{
			shutdownRequestHandler = new COPCShutdownRequest();
			connectionPoint->Advise(shutdownRequestHandler, &shutdownCookie);
		}
	}

	shutdownRequestHandler->SetShutdownRequestCallback(callback);
}
