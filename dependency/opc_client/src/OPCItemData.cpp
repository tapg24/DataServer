#include <atlstr.h>
#include <atlcoll.h>
#include "OPCItemData.h"
#include "OPCClient.h"

OPCItemData::OPCItemData(HRESULT err):error(err)
{
	vDataValue.vt = VT_EMPTY;
}

OPCItemData::OPCItemData(FILETIME time, WORD qual, VARIANT & val, HRESULT err)
{
	vDataValue.vt = VT_EMPTY;
	HRESULT result = VariantCopy( &vDataValue, &val);
	if (FAILED(result))
	{
		throw OPCException("VariantCopy failed");
	}

	ftTimeStamp = time;
	wQuality = qual;
	error = err;
}


OPCItemData::OPCItemData()
{
	vDataValue.vt = VT_EMPTY;
}

/*
OPCItemData::OPCItemData( OPCItemData const& source )
{
	VariantInit(&vDataValue);
	HRESULT result = VariantCopy( &vDataValue, &(source.vDataValue));
	if (FAILED(result))
	{
		throw OPCException("VariantCopy failed");
	}

	ftTimeStamp = source.ftTimeStamp;
	wQuality = source.wQuality;
	error = source.error;
}
*/


OPCItemData::~OPCItemData()
{
	if (vDataValue.vt == 8 && vDataValue.bstrVal != 0)
	{
		HRESULT result = VariantClear(&vDataValue);
		if (FAILED(result))
		{
			throw OPCException("VariantClear failed");
		}
	}
}


void OPCItemData::set(OPCITEMSTATE &itemState)
{
	HRESULT result = VariantCopy( &vDataValue, &itemState.vDataValue);
	if (FAILED(result))
	{
		throw OPCException("VariantCopy failed");
	}

	ftTimeStamp = itemState.ftTimeStamp;
	wQuality = itemState.wQuality;
}


void OPCItemData::set(FILETIME time, WORD qual, VARIANT & val)
{
	HRESULT result = VariantCopy( &vDataValue, &val);
	if (FAILED(result))
	{
		throw OPCException("VarCopy failed");
	}

	ftTimeStamp = time;
	wQuality = qual;
}

/*
OPCItemData & OPCItemData::operator=(OPCItemData &source)
{
	HRESULT result = VariantCopy( &vDataValue, &(source.vDataValue));
	if (FAILED(result))
	{
		throw OPCException("VarCopy failed");
	}

	ftTimeStamp = source.ftTimeStamp;
	wQuality = source.wQuality;

	return *this;
}
*/

void OPCItemData::deepCopy( OPCItemData& dest ) const
{
	HRESULT result = VariantCopy( &(dest.vDataValue), &vDataValue);
	if (FAILED(result))
	{
		throw OPCException("VariantCopy failed");
	}

	dest.ftTimeStamp = ftTimeStamp;
	dest.wQuality = wQuality;
	dest.error = error;
}

OPCItemData* OPCItemData::createPtr()
{
	return new OPCItemData( ftTimeStamp, wQuality, vDataValue, error );
}

void OPCItemData::copyFrom( const OPCItemData& src )
{
	HRESULT result = VariantCopy( &vDataValue, &(src.vDataValue));
	if (FAILED(result))
	{
		throw OPCException("VariantCopy failed");
	}

	ftTimeStamp = src.ftTimeStamp;
	wQuality = src.wQuality;
	error = src.error;
}

void OPCItemData::copyFrom( const OPCItemData* src )
{
	//assert(src);

	HRESULT result = VariantCopy( &vDataValue, &(src->vDataValue));
	if (FAILED(result))
	{
		throw OPCException("VariantCopy failed");
	}

	ftTimeStamp = src->ftTimeStamp;
	wQuality = src->wQuality;
	error = src->error;
}


COPCItem_DataMap::~COPCItem_DataMap()
{
	POSITION pos = GetStartPosition();
	while (pos != NULL)
	{
		OPCItemData * data = GetNextValue(pos);
		if (data){
			delete data;
		}
	}
	RemoveAll();
}





