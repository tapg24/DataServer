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

#if !defined(AFX_OPCSERVER_H__AD6316C0_37B3_4DEC_8378_EE03CC3AEED8__INCLUDED_)
#define AFX_OPCSERVER_H__AD6316C0_37B3_4DEC_8378_EE03CC3AEED8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OPCClient.h"
#include "OPCGroup.h"
#include "../../opc_foundation/opccomn.h"
#include <atlcom.h> 

struct IShutdownRequestCallback
{
	virtual void ShutdownRequest(const CString& reason) = 0;
};

/**
* Holds status information about the server
*/
typedef struct
{ 
	FILETIME ftStartTime; 
	FILETIME ftCurrentTime; 
	FILETIME ftLastUpdateTime; 
	OPCSERVERSTATE dwServerState; 
	DWORD dwGroupCount; 
	DWORD dwBandWidth; 
	WORD wMajorVersion;
	WORD wMinorVersion;
	WORD wBuildNumber;
	CString vendorInfo;
} ServerStatus;


/**
* Local representation of a local or remote OPC server. Wrapper for the COM interfaces to the server.
*/
class COPCServer  
{
private:
	/**
	* IUnknown interface to the OPC server
	*/
	ATL::CComPtr<IOPCServer> iOpcServer;

	/**
	* Interface to the OPC server namespace
	*/
	ATL::CComPtr<IOPCBrowseServerAddressSpace> iOpcNamespace;

	/**
	* interface to the properties maintained for each item in the server namespace
	*/
	ATL::CComPtr<IOPCItemProperties> iOpcProperties;

	/**
	* interface to the properties maintained for each item in the server namespace
	*/
	ATL::CComPtr<IConnectionPointContainer> iOpcConnections;

	friend class COPCGroup;
	/**
	* Used by group object.
	*/
	ATL::CComPtr<IOPCServer> &getServerInterface()
	{
		return iOpcServer;
	}

	friend class COPCItem;
	ATL::CComPtr<IOPCItemProperties> &getPropertiesInterface()
	{
		return iOpcProperties;
	}

	class COPCShutdownRequest;
	ATL::CComPtr<COPCShutdownRequest> shutdownRequestHandler;
	DWORD shutdownCookie;

public:

	/**
	* Make OPC server. 
	* @param opcServerInterface passed form the OPCHost
	*/
	COPCServer(ATL::CComPtr<IOPCServer> &opcServerInterface);


	virtual ~COPCServer();

	
	/**
	* Browse the OPC servers namespace.
	* This is currently done FLAT mode
	* TODO implement browsing of structured namespace
	*/
	void getAllItemNames(CAtlArray<CString> & opcItemNames);

	void getItemNames(const WCHAR* branchName, CAtlArray<CString> & opcItemNames);

//	void getItemProperties(const CAtlArray<CPropertyDescription> &propsToRead, ATL::CAutoPtrArray<SPropertyValue> &propsReaded);

	/**
	* Browse the OPC servers namespace in hierarchial mode.
	* 
	*/
	void getBrancheNames(const WCHAR* branchName, CAtlArray<CString> & opcBrancheNames);


	/**
	* Get an OPC group. Caller owns
	*/
	COPCGroup *makeGroup(const CString & groupName, bool active, unsigned long reqUpdateRate_ms, unsigned long &revisedUpdateRate_ms, float deadBand, const DWORD keepAlive, DWORD& revisedKeepAlive);


	/**
	* get the current status of the server.
	*/
	void getStatus(ServerStatus &status);

	void setShutdownRequestCallback(IShutdownRequestCallback* callback);
};

#endif // !defined(AFX_OPCSERVER_H__AD6316C0_37B3_4DEC_8378_EE03CC3AEED8__INCLUDED_)
