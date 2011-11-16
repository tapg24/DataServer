#pragma once

#include "OPCClientToolKitDLL.h"
#include "../../opc_foundation/opcda.h"

class COPCItem;

/**
*Wrapper for OPC data. Hides COM memory management.
*/
struct  OPCItemData
{
	FILETIME ftTimeStamp;
	WORD wQuality;
	VARIANT vDataValue;
	HRESULT error;


	OPCItemData();
	~OPCItemData();

	OPCItemData(HRESULT err);
	OPCItemData(FILETIME time, WORD qual, VARIANT & val, HRESULT err);

	void set(OPCITEMSTATE &itemState);
	void set(FILETIME time, WORD qual, VARIANT & val);
	void deepCopy(OPCItemData& dest) const;
	void copyFrom(const OPCItemData& src);
	void copyFrom(const OPCItemData* src);
	OPCItemData* createPtr();

//private:
	//OPCItemData(OPCItemData const& copy); // 08.11.2010 добавил конструктор копирования
	//OPCItemData & operator=(OPCItemData &source);

};



/**
* Can't find an ATL autoptr map - this class provides the functionality I want.
*/
class  COPCItem_DataMap : public CAtlMap<COPCItem *, OPCItemData *>
{
public:
	~COPCItem_DataMap();
};



