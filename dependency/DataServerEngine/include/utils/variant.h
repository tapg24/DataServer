#ifndef VARIANT_H
#define VARIANT_H

#include <string>
#include <wtypes.h>
#include <OaIdl.h>


namespace Channels
{

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
		ComVariant( const std::string& cValue );
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
		std::string valueAsString() const;
		std::string typeAsString() const;

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
		ComVariant& operator = ( const std::string &rVal );
		ComVariant& operator = ( const VARIANT &rVal );

		// cast methods
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
		//operator std::string() const;
		operator const VARIANT&() const;

		// get value methods
		bool AsBool();
		char AsChar();
		unsigned char AsUChar();
		short AsShort();
		unsigned short AsUShort();
		int AsInt();
		unsigned int AsUInt();
		long AsLong();
		unsigned long AsULong();
		long long AsLongLong();
		unsigned long long AsULongLong();
		float AsFloat();
		double AsDouble();
		CY AsCY();
		FILETIME AsFILETIME();
		std::string AsString();

		// other methods
		HRESULT copyTo( VARIANT &dst ) const;
		HRESULT copyTo( VARIANT *dst ) const;

		// static methods
		static void variantClear( VARIANT *ptr );
		static HRESULT variantCopy( VARIANT *dst, const VARIANT *src );
		static HRESULT variantCopy( VARIANT &dst, const VARIANT &src );
		static bool isEqual( const VARIANT &val1, const VARIANT &val2 );
	};

}

#endif // VARIANT_H