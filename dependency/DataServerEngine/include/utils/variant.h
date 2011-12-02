#ifndef VARIANT_H
#define VARIANT_H

#include "string.h"
//#include <string>
#include <wtypes.h>
#include <OaIdl.h>


class ComVariant
{
private:
	VARIANT value;

public:
	// Constructors
	ComVariant();
	ComVariant( const ComVariant &rVal );

	ComVariant( bool cValue );
	ComVariant( char cValue );
	ComVariant( unsigned char cValue );
	ComVariant( short cValue);
	ComVariant( unsigned short cValue );
	ComVariant( int cValue);
	ComVariant( unsigned int cValue );
	ComVariant( long cValue );
	ComVariant( unsigned long cValue );
	ComVariant( __int64 cValue );
	ComVariant( unsigned __int64 cValue );
	ComVariant( float cValue );
	ComVariant( double cValue );
	ComVariant( CY cValue );
	ComVariant( const FILETIME &cValue );
	ComVariant( const string_t& cValue );
	ComVariant( const VARIANT& cValue );

	// Destructor
	~ComVariant();

	// methods
	void init();
	void clear();
	const VARTYPE& getType() const;
	void setType( VARTYPE type );
	VARIANT& getRef();
	const VARIANT& getConstRef() const;
	VARIANT* getPtr();
	string_t valueAsString() const;
	string_t typeAsString() const;

	// assignment methods
	ComVariant& operator = ( const ComVariant &rVal );
	ComVariant& operator = ( bool rVal );
	ComVariant& operator = ( char rVal );
	ComVariant& operator = ( unsigned char rVal );
	ComVariant& operator = ( short rVal );
	ComVariant& operator = ( unsigned short rVal );
	ComVariant& operator = ( int rVal );
	ComVariant& operator = ( unsigned int rVal );
	ComVariant& operator = ( long rVal );
	ComVariant& operator = ( unsigned long rVal );
	ComVariant& operator = ( __int64 rVal );
	ComVariant& operator = ( unsigned __int64 rVal );
	ComVariant& operator = ( float rVal );
	ComVariant& operator = ( double rVal );
	ComVariant& operator = ( const CY &rVal );
	ComVariant& operator = ( const FILETIME &rVal );
	ComVariant& operator = ( const string_t &rVal );
	ComVariant& operator = ( const VARIANT &rVal );

	// implicit cast methods
	//operator bool() const;
	//operator char() const;
	//operator unsigned char() const;
	//operator short() const;
	//operator unsigned short() const;
	//operator int() const;
	//operator unsigned int() const;
	//operator long() const;
	//operator unsigned long() const;
	//operator __int64() const;
	//operator unsigned __int64() const;
	//operator float() const;
	//operator double() const;
	//operator CY() const;
	//operator FILETIME() const;
	//operator string_t() const;
	operator const VARIANT&() const;

	// explicit cast methods
	bool AsBool() const;
	char AsChar() const;
	unsigned char AsUChar() const;
	short AsShort() const;
	unsigned short AsUShort() const;
	int AsInt() const;
	unsigned int AsUInt() const;
	long AsLong() const;
	unsigned long AsULong() const;
	long long AsLongLong() const;
	unsigned long long AsULongLong() const;
	float AsFloat() const;
	double AsDouble() const;
	CY AsCY() const;
	FILETIME AsFILETIME() const;
	string_t AsString() const;

	// other methods
	HRESULT copyTo( VARIANT &dst ) const;
	HRESULT copyTo( VARIANT *dst ) const;

	// static methods
	static void variantClear( VARIANT *ptr );
	static HRESULT variantCopy( VARIANT *dst, const VARIANT *src );
	static HRESULT variantCopy( VARIANT &dst, const VARIANT &src );
	static bool isEqual( const VARIANT &val1, const VARIANT &val2 );
};


#endif // VARIANT_H