#include "utils/variant.h"
#include "utils/string_algorithm.h"
#include <boost/assert.hpp>
#include <../../opc_foundation/opcda.h>


	string_t TypeAsString(VARTYPE vt)
	{
		switch (vt)
		{
		case    VT_EMPTY:
			return "VT_EMPTY";

		case    VT_NULL:
			return "VT_NULL";

		case    VT_I2:
			return "VT_I2";

		case    VT_I4:
			return "VT_I4";

		case    VT_R4:
			return "VT_R4";

		case    VT_R8:
			return "VT_R8";

		case    VT_CY:
			return "VT_CY";

		case    VT_DATE:
			return "VT_DATE";

		case    VT_BSTR:
			return "VT_BSTR";

		case    VT_DISPATCH:
			return "VT_DISPATCH";

		case    VT_ERROR:
			return "VT_ERROR";

		case    VT_BOOL:
			return "VT_BOOL";

		case    VT_VARIANT:
			return "VT_VARIANT";

		case    VT_DECIMAL:
			return "VT_DECIMAL";

		case    VT_RECORD:
			return "VT_RECORD";

		case    VT_UNKNOWN:
			return "VT_UNKNOWN";

		case    VT_I1:
			return "VT_I1";

		case    VT_UI1:
			return "VT_UI1";

		case    VT_UI2:
			return "VT_UI2";

		case    VT_UI4:
			return "VT_UI4";

		case    VT_INT:
			return "VT_INT";

		case    VT_UINT:
			return "VT_UINT";

		case    VT_VOID:
			return "VT_VOID";

		case    VT_HRESULT:
			return "VT_HRESULT";

		case    VT_PTR:
			return "VT_PTR";

		case    VT_SAFEARRAY:
			return "VT_SAFEARRAY";

		case    VT_CARRAY:
			return "VT_CARRAY";

		case    VT_USERDEFINED:
			return "VT_USERDEFINED";

		case    VT_LPSTR:
			return "VT_LPSTR";

		case    VT_LPWSTR:
			return "VT_LPWSTR";

		case    VT_BLOB:
			return "VT_BLOB";

		case    VT_STREAM:
			return "VT_STREAM";

		case    VT_STORAGE:
			return "VT_STORAGE";

		case    VT_STREAMED_OBJECT:
			return "VT_STREAMED_OBJECT";

		case    VT_STORED_OBJECT:
			return "VT_STORED_OBJECT";

		case    VT_BLOB_OBJECT:
			return "VT_BLOB_OBJECT";

		case    VT_CF:  // Clipboard Format
			return "VT_CF";

		case    VT_CLSID:
			return "VT_CLSID";

		case    VT_VECTOR:
			return "VT_VECTOR";

		case    VT_ARRAY:
			return "VT_ARRAY";

			//case    VT_BYREF:
			//return "VT_BYREF";

		case    VT_BYREF|VT_DECIMAL:
			return "VT_BYREF|VT_DECIMAL";


			//case    VT_ARRAY|*:
			//return "VT_ARRAY|*";

		case    VT_BYREF|VT_UI1:
			return "VT_BYREF|VT_UI1";

		case    VT_BYREF|VT_I2:
			return "VT_BYREF|VT_I2";

		case    VT_BYREF|VT_I4:
			return "VT_BYREF|VT_I4";

		case    VT_BYREF|VT_R4:
			return "VT_BYREF|VT_R4";

		case    VT_BYREF|VT_R8:
			return "VT_BYREF|VT_R8";

		case    VT_BYREF|VT_BOOL:
			return "VT_BYREF|VT_BOOL";

		case    VT_BYREF|VT_ERROR:
			return "VT_BYREF|VT_ERROR";

		case    VT_BYREF|VT_CY:
			return "VT_BYREF|VT_CY";

		case    VT_BYREF|VT_DATE:
			return "VT_BYREF|VT_DATE";

		case    VT_BYREF|VT_BSTR:
			return "VT_BYREF|VT_BSTR";

		case    VT_BYREF|VT_UNKNOWN:
			return "VT_BYREF|VT_UNKNOWN";

		case    VT_BYREF|VT_DISPATCH:
			return "VT_BYREF|VT_DISPATCH";

			//case    VT_ARRAY|*:
			//return "VT_ARRAY|*";

		case    VT_BYREF|VT_VARIANT:
			return "VT_BYREF|VT_VARIANT";

			//Generic case ByRef:
			//return "ByRef";


		case    VT_BYREF|VT_I1:
			return "VT_BYREF|VT_I1";

		case    VT_BYREF|VT_UI2:
			return "VT_BYREF|VT_UI2";

		case    VT_BYREF|VT_UI4:
			return "VT_BYREF|VT_UI4";

		case    VT_BYREF|VT_INT:
			return "VT_BYREF|VT_INT";

		case    VT_BYREF|VT_UINT:
			return "VT_BYREF|VT_UINT";

		default:
			return "Unknown type";
		}
	}

	ComVariant::ComVariant()
	{
		init();
		value.vt = VT_EMPTY;
	}

	ComVariant::ComVariant( const ComVariant &rVal )
	{
		init();
		*this = rVal;
	}

	ComVariant::ComVariant( bool cValue )
	{
		init();
		*this = cValue;
	}

	ComVariant::ComVariant( char cValue )
	{
		init();
		*this = cValue;
	}

	ComVariant::ComVariant( unsigned char cValue )
	{
		init();
		*this = cValue;
	}

	ComVariant::ComVariant( short cValue )
	{
		init();
		*this = cValue;
	}

	ComVariant::ComVariant( unsigned short cValue )
	{
		init();
		*this = cValue;
	}

	ComVariant::ComVariant( int cValue )
	{
		init();
		*this = cValue;
	}

	ComVariant::ComVariant( unsigned int cValue )
	{
		init();
		*this = cValue;
	}

	ComVariant::ComVariant( long cValue )
	{
		init();
		*this = cValue;
	}

	ComVariant::ComVariant( unsigned long cValue )
	{
		init();
		*this = cValue;
	}

	ComVariant::ComVariant( __int64 cValue )
	{
		init();
		*this = cValue;
	}

	ComVariant::ComVariant( unsigned __int64 cValue )
	{
		init();
		*this = cValue;
	}

	ComVariant::ComVariant( float cValue )
	{
		init();
		*this = cValue;
	}

	ComVariant::ComVariant( double cValue )
	{
		init();
		*this = cValue;
	}

	ComVariant::ComVariant( CY cValue )
	{
		init();
		*this = cValue;
	}

	ComVariant::ComVariant( const FILETIME &cValue )
	{
		init();
		*this = cValue;
	}

	ComVariant::ComVariant( const string_t& cValue )
	{
		init();
		*this = cValue;
	}

	ComVariant::ComVariant( const VARIANT& cValue )
	{
		init();
		*this = cValue;
	}

	ComVariant::~ComVariant()
	{
		clear();
	}

	void ComVariant::init()
	{
		::ZeroMemory( &value, sizeof(VARIANT) );
	}

	void ComVariant::clear()
	{
		ComVariant::variantClear( &value );
	}

	void ComVariant::variantClear( VARIANT *ptr )
	{
		if( ptr == NULL )
			return;
		::VariantClear( ptr );
		::ZeroMemory( ptr, sizeof(VARIANT) );
	}

	const VARTYPE& ComVariant::getType() const
	{
		return value.vt;
	}

	VARIANT& ComVariant::getRef() 
	{
		return value;
	}

	const VARIANT& ComVariant::getConstRef() const
	{
		return value;
	}

	VARIANT* ComVariant::getPtr()
	{
		return &value;
	}

	HRESULT ComVariant::variantCopy( VARIANT *dst, const VARIANT *src )
	{
		if( dst == NULL || src == NULL )
			return E_FAIL;
		return ::VariantCopy( dst, (VARIANT*)src );
	}

	HRESULT ComVariant::variantCopy( VARIANT &dst, const VARIANT &src )
	{
		return ::VariantCopy( &dst, (VARIANT*)&src );
	}

	ComVariant& ComVariant::operator=( const ComVariant &rVal )
	{
		if( this == &rVal )
			return *this;
		clear();
		ComVariant::variantCopy( &value, &rVal.value );
		return *this;
	}

	ComVariant& ComVariant::operator=( bool rVal )
	{
		clear();
		value.vt = VT_BOOL;
		value.boolVal = ( rVal ) ? VARIANT_TRUE : VARIANT_FALSE;
		return *this;
	}		

	ComVariant& ComVariant::operator=( char rVal )
	{
		clear();
		value.vt = VT_I1;
		value.cVal = rVal;
		return *this;
	}

	ComVariant& ComVariant::operator=( unsigned char rVal )
	{
		clear();
		value.vt = VT_UI1;
		value.bVal = rVal;
		return *this;
	}

	ComVariant& ComVariant::operator=( short rVal )
	{
		clear();
		value.vt = VT_I2;
		value.iVal = rVal;
		return *this;
	}

	ComVariant& ComVariant::operator=( unsigned short rVal )
	{
		clear();
		value.vt = VT_UI2;
		value.uiVal = rVal;
		return *this;
	}

	ComVariant& ComVariant::operator=( int rVal )
	{
		clear();
		value.vt = VT_I4;
		value.lVal = rVal;
		return *this;
	}

	ComVariant& ComVariant::operator=( unsigned int rVal )
	{
		clear();
		value.vt = VT_UI4;
		value.ulVal = rVal;
		return *this;
	}

	ComVariant& ComVariant::operator=( long rVal )
	{
		clear();
		value.vt = VT_I4;
		value.lVal = rVal;
		return *this;
	}

	ComVariant& ComVariant::operator=( unsigned long rVal )
	{
		clear();
		value.vt = VT_UI4;
		value.ulVal = rVal;
		return *this;
	}

	ComVariant& ComVariant::operator=( __int64 rVal )
	{
		clear();
		value.vt = VT_I8;
		value.llVal = rVal;
		return *this;
	}

	ComVariant& ComVariant::operator=( unsigned __int64 rVal )
	{
		clear();
		value.vt = VT_UI8;
		value.llVal = rVal;
		return *this;
	}

	ComVariant& ComVariant::operator=( float rVal )
	{
		clear();
		value.vt = VT_R4;
		value.fltVal = rVal;
		return *this;
	}

	ComVariant& ComVariant::operator=( double rVal )
	{
		clear();
		value.vt = VT_R8;
		value.dblVal = rVal;
		return *this;
	}

	ComVariant& ComVariant::operator=( const CY &rVal )
	{
		clear();
		value.vt = VT_CY;
		value.cyVal = rVal;
		return *this;
	}

	ComVariant& ComVariant::operator=( const FILETIME &rVal )
	{
		clear();
		value.vt = VT_DATE;
		SYSTEMTIME st;
		if( ! :: FileTimeToSystemTime( &rVal, &st ) )
		{
			//FRL_THROW_SYSAPI_S();
		}
		if( ! ::SystemTimeToVariantTime( &st, &value.date ) )
		{
			//FRL_THROW_S();
		}
		return *this;
	}

	ComVariant& ComVariant::operator=( const string_t& rVal )
	{
		clear();
		value.vt = VT_BSTR;
#if( UNICODE )
		value.bstrVal = SysAllocString( rVal.c_str() );
#else
		value.bstrVal = SysAllocString( widen( rVal ).c_str() );
#endif
		return *this;
	}

	ComVariant& ComVariant::operator=( const VARIANT& rVal )
	{
		clear();
		variantCopy( &value, &rVal );
		return *this;
	}

	//Variant::operator bool() const
	//{
	//	BOOST_ASSERT( value.vt == VT_BOOL );
	//	return ( value.boolVal ) ? true : false;
	//}

	//Variant::operator char() const
	//{
	//	BOOST_ASSERT( value.vt == VT_I1 );
	//	return value.cVal;
	//}

	//Variant::operator unsigned char() const
	//{
	//	BOOST_ASSERT( value.vt == VT_UI1 );
	//	return value.bVal;
	//}

	//Variant::operator short() const
	//{
	//	BOOST_ASSERT( value.vt == VT_I2 );
	//	return value.iVal;
	//}

	//Variant::operator unsigned short() const
	//{
	//	BOOST_ASSERT( value.vt == VT_UI2 );
	//	return value.uiVal;
	//}

	//Variant::operator int() const
	//{
	//	BOOST_ASSERT( value.vt == VT_I4 );
	//	return value.lVal;
	//}

	//Variant::operator unsigned int() const
	//{
	//	BOOST_ASSERT( value.vt == VT_UI4 );
	//	return ( unsigned int )value.ullVal;
	//}

	//Variant::operator long() const
	//{
	//	BOOST_ASSERT( value.vt == VT_I4 );
	//	return value.lVal;
	//}

	//Variant::operator unsigned long() const
	//{
	//	BOOST_ASSERT( value.vt == VT_UI4 );
	//	return value.ulVal;
	//}

	//Variant::operator __int64() const
	//{
	//	BOOST_ASSERT( value.vt == VT_I8 );
	//	return value.llVal;
	//}

	//Variant::operator unsigned __int64() const
	//{
	//	BOOST_ASSERT( value.vt == VT_UI8 );
	//	return value.ullVal;
	//}

	//Variant::operator float() const
	//{
	//	BOOST_ASSERT( value.vt == VT_R4 );
	//	return value.fltVal;
	//}

	//Variant::operator double() const
	//{
	//	BOOST_ASSERT( value.vt == VT_R8 );
	//	return value.dblVal;
	//}

	//Variant::operator CY() const
	//{
	//	BOOST_ASSERT( value.vt == VT_CY );
	//	return value.cyVal;
	//}

	//Variant::operator FILETIME() const
	//{
	//	BOOST_ASSERT( value.vt == VT_DATE );

	//	SYSTEMTIME st;
	//	BOOST_ASSERT( ::VariantTimeToSystemTime( value.date, &st ) ); 

	//	FILETIME ft;
	//	BOOST_ASSERT( ::SystemTimeToFileTime( &st, &ft ) );

	//	return ft;
	//}

	//Variant::operator string_t() const
	//{
	//	BOOST_ASSERT( value.vt == VT_BSTR );

	//	#if( UNICODE )
	//		return value.bstrVal;
	//	#else
	//		return wstring2string( value.bstrVal );
	//	#endif
	//}

	ComVariant::operator const VARIANT&() const
	{
		return value;
	}

	bool ComVariant::isEqual( const VARIANT &val1, const VARIANT &val2 )
	{
		if ( val1.vt != val2.vt )
			return false;

		switch (val1.vt)
		{
		case VT_EMPTY:
			return val1.vt == val2.vt;
		case VT_I1:
			return val1.cVal == val2.cVal;
		case VT_UI1:
			return val1.bVal == val2.bVal;
		case VT_I2:
			return val1.iVal == val2.iVal;
		case VT_UI2:
			return val1.uiVal == val2.uiVal;
		case VT_I4:
			return val1.lVal == val2.lVal;
		case VT_UI4:
			return val1.ulVal == val2.ulVal;
		case VT_I8:
			return val1.llVal == val2.llVal;
		case VT_UI8:
			return val1.ullVal == val2.ullVal;
		case VT_R4:
			return val1.fltVal == val2.fltVal;
		case VT_R8:
			return val1.dblVal == val2.dblVal;
		case VT_CY:
			return (val1.cyVal.int64 == val2.cyVal.int64);
		case VT_DATE:
			return val1.date == val2.date;
		case VT_BOOL:
			return val1.boolVal == val2.boolVal;
		case VT_BSTR:
			if( val1.bstrVal != NULL && val2.bstrVal != NULL )
				return wcscmp(val1.bstrVal, val2.bstrVal) == 0;
			return ( val1.bstrVal == val2.bstrVal );
		}
		return false;
	}

	void ComVariant::setType( VARTYPE type )
	{
		//::VariantChangeType( &value, &value, 0, type );
		::VariantChangeType( &value, &value, VARIANT_ALPHABOOL, type );
	}

	HRESULT ComVariant::copyTo( VARIANT &dst ) const
	{
		return ComVariant::variantCopy( dst, value );
	}

	HRESULT ComVariant::copyTo( VARIANT *dst ) const
	{
		return ComVariant::variantCopy( dst, &value );
	}

	string_t ComVariant::valueAsString() const
	{
		if (value.vt != VT_BSTR)
		{
			ComVariant tmp = *this;

			tmp.setType(VT_BSTR);
			return tmp.valueAsString();
		}

		return narrow(value.bstrVal);
	}

	string_t ComVariant::typeAsString() const
	{
		return TypeAsString(value.vt);
	}

	bool ComVariant::AsBool() const
	{
		BOOST_ASSERT( value.vt == VT_BOOL );
		return ( value.boolVal ) ? true : false;
	}

	char ComVariant::AsChar() const
	{
		BOOST_ASSERT( value.vt == VT_I1 );
		return value.cVal;
	}

	unsigned char ComVariant::AsUChar() const
	{
		BOOST_ASSERT( value.vt == VT_UI1 );
		return value.bVal;
	}

	short ComVariant::AsShort() const
	{
		BOOST_ASSERT( value.vt == VT_I2 );
		return value.iVal;
	}

	unsigned short ComVariant::AsUShort() const
	{
		BOOST_ASSERT( value.vt == VT_UI2 );
		return value.uiVal;
	}

	int ComVariant::AsInt() const
	{
		BOOST_ASSERT( value.vt == VT_I4 );
		return value.lVal;
	}

	unsigned int ComVariant::AsUInt() const
	{
		BOOST_ASSERT( value.vt == VT_UI4 );
		return ( unsigned int )value.ullVal;
	}

	long ComVariant::AsLong() const
	{
		BOOST_ASSERT( value.vt == VT_I4 );
		return value.lVal;
	}

	unsigned long ComVariant::AsULong() const
	{
		BOOST_ASSERT( value.vt == VT_UI4 );
		return value.ulVal;
	}

	long long ComVariant::AsLongLong() const
	{
		BOOST_ASSERT( value.vt == VT_I8 );
		return value.llVal;
	}

	unsigned long long ComVariant::AsULongLong() const
	{
		BOOST_ASSERT( value.vt == VT_UI8 );
		return value.ullVal;
	}

	float ComVariant::AsFloat() const
	{
		BOOST_ASSERT( value.vt == VT_R4 );
		return value.fltVal;
	}

	double ComVariant::AsDouble() const
	{
		BOOST_ASSERT( value.vt == VT_R8 );
		return value.dblVal;
	}

	CY ComVariant::AsCY() const
	{
		BOOST_ASSERT( value.vt == VT_CY );
		return value.cyVal;
	}

	FILETIME ComVariant::AsFILETIME() const
	{
		BOOST_ASSERT( value.vt == VT_DATE );

		SYSTEMTIME st;
		BOOST_ASSERT( ::VariantTimeToSystemTime( value.date, &st ) ); 

		FILETIME ft;
		BOOST_ASSERT( ::SystemTimeToFileTime( &st, &ft ) );

		return ft;
	}

	string_t ComVariant::AsString() const
	{
		BOOST_ASSERT( value.vt == VT_BSTR );

		#if( UNICODE )
			return value.bstrVal;
		#else
			return narrow( value.bstrVal );
		#endif
	}