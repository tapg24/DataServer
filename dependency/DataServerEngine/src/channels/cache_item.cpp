#include "channels/cache_item.h"
#include <boost/log/trivial.hpp>
#include <boost/format.hpp>
#include "utils/string_algorithm.h"

namespace Channels
{

	CacheItem::CacheItem(const string_t& name, const VARIANT& value)
	{
		name_ = name;

		HRESULT result = ComVariant::variantCopy( vDataValue_.getRef(), value );
		if ( FAILED(result) )
		{
			BOOST_LOG_TRIVIAL(trace) << "CacheItem::CacheItem error for " << name;
		}

		FILETIME timeStamp;
		::GetSystemTimeAsFileTime( &timeStamp );
		ftTimeStamp_ = timeStamp;

		wQuality_ = OPC_QUALITY_BAD;
	}

	CacheItem::CacheItem( const string_t& name, const FILETIME& timestamp, const WORD quality, const VARIANT& value )
	{
		name_ = name;

		HRESULT result = ComVariant::variantCopy( vDataValue_.getRef(), value );
		if ( FAILED(result) )
		{
			BOOST_LOG_TRIVIAL(trace) << "CacheItem::CacheItem Variant::variantCopy error for " << name;
		}

		//if ( value.vt != VT_EMPTY )
		//{
		//	HRESULT result = ComVariant::variantCopy( vCacheDataValue_.getRef(), value );
		//	if ( FAILED(result) )
		//	{
		//		BOOST_LOG_TRIVIAL(trace) << "CacheItem::CacheItem Variant::variantCopy error for " << name;
		//	}
		//	ftCacheTimeStamp_ = timestamp;
		//}

		ftTimeStamp_ = timestamp;
		wQuality_ = quality;
		//error = result;

		changed_ = true;
	}

	CacheItem::CacheItem( const string_t& name, const VARIANT& value, const WORD quality )
	{
		name_ = name;

		HRESULT result = ComVariant::variantCopy( vDataValue_.getRef(), value );
		if ( FAILED(result) )
		{
			BOOST_LOG_TRIVIAL(trace) << "CacheItem::CacheItem error for " << name;
		}

		FILETIME timeStamp;
		::GetSystemTimeAsFileTime( &timeStamp );
		ftTimeStamp_ = timeStamp;

		wQuality_ = quality;
	}

	CacheItem::~CacheItem()
	{

	}

	void CacheItem::CopyFrom(const CacheItem& item)
	{
		if (  ComVariant::isEqual(vDataValue_.getRef(), item.GetValue()) )
		{
			if( wQuality_ != item.GetQuality() )
			{
				ftTimeStamp_ = item.GetTimeStamp();
				wQuality_ = item.GetQuality();

				changed_ = true;
			}
			return;
		}

		HRESULT result = ComVariant::variantCopy( vDataValue_.getRef(), item.GetValue().getConstRef() );
		if ( FAILED(result) )
		{
			BOOST_LOG_TRIVIAL(trace) << "CacheItem::CopyFrom Variant::variantCopy error for " << name_;
		}

		//if ( item.GetValue().getType() != VT_EMPTY )
		//{
		//	HRESULT result = ComVariant::variantCopy( vCacheDataValue_.getRef(), item.GetValue().getConstRef() );
		//	if ( FAILED(result) )
		//	{
		//		BOOST_LOG_TRIVIAL(trace) << "CacheItem::CopyFrom Variant::variantCopy error for " << name_;
		//	}
		//	ftCacheTimeStamp_ = item.GetTimeStamp();
		//}

		ftTimeStamp_ = item.GetTimeStamp();
		wQuality_ = item.GetQuality();

		changed_ = true;
	}

	void CacheItem::CopyFrom(const WORD quality, const VARIANT& value)
	{
		FILETIME timestamp;
		::GetSystemTimeAsFileTime( &timestamp );

		if (  ComVariant::isEqual(vDataValue_.getRef(), value) )
		{
			if( wQuality_ != quality )
			{
				ftTimeStamp_ = timestamp;
				wQuality_ = quality;
				
				changed_ = true;
			}

			return;
		}

		HRESULT result = ComVariant::variantCopy( vDataValue_.getRef(), value);
		if ( FAILED(result) )
		{
			BOOST_LOG_TRIVIAL(trace) << "CacheItem::CopyFrom Variant::variantCopy error for " << name_;
		}

		//if ( value.vt != VT_EMPTY )
		//{
		//	HRESULT result = ComVariant::variantCopy( vCacheDataValue_.getRef(), value );
		//	if ( FAILED(result) )
		//	{
		//		BOOST_LOG_TRIVIAL(trace) << "CacheItem::CopyFrom Variant::variantCopy error for " << name_;
		//	}
		//	ftCacheTimeStamp_ = timestamp;
		//}

		ftTimeStamp_ = timestamp;
		wQuality_ = quality;

		changed_ = true;
	}

	void CacheItem::CopyFrom( const FILETIME& timestamp, const WORD quality, const VARIANT& value )
	{
		if (  ComVariant::isEqual(vDataValue_.getRef(), value) )
		{
			if( wQuality_ != quality )
			{
				ftTimeStamp_ = timestamp;
				wQuality_ = quality;

				changed_ = true;
			}
			return;
		}

		HRESULT result = ComVariant::variantCopy( vDataValue_.getRef(), value);
		if ( FAILED(result) )
		{
			BOOST_LOG_TRIVIAL(trace) << "CacheItem::CopyFrom Variant::variantCopy error for " << name_;
		}

		//if ( value.vt != VT_EMPTY )
		//{
		//	HRESULT result = ComVariant::variantCopy( vCacheDataValue_.getRef(), value );
		//	if ( FAILED(result) )
		//	{
		//		BOOST_LOG_TRIVIAL(trace) << "CacheItem::CopyFrom Variant::variantCopy error for " << name_;
		//	}
		//	ftCacheTimeStamp_ = timestamp;
		//}

		ftTimeStamp_ = timestamp;
		wQuality_ = quality;

		changed_ = true;
	}


	string_t CacheItem::GetName() const
	{
		return name_;
	}

	ComVariant CacheItem::GetValue() const
	{
//		if( vDataValue_.getType() != VT_EMPTY )
//		{
			return vDataValue_;
//		}
		//else
		//{
		//	return vCacheDataValue_;
		//}
	}

	FILETIME CacheItem::GetTimeStamp() const
	{
//		if( vDataValue_.getType() != VT_EMPTY )
//		{
			return ftTimeStamp_;
//		}
//		else
//		{
//			return ftCacheTimeStamp_;
//		}
	}

	WORD CacheItem::GetQuality() const
	{
		return wQuality_;
	}

	void CacheItem::SetName( const string_t& name )
	{
		name_ = name;
	}

	void CacheItem::SetValue( const VARIANT& variant )
	{
		HRESULT result = ComVariant::variantCopy( vDataValue_.getRef(), variant);
		if (FAILED(result))
		{
			BOOST_LOG_TRIVIAL(trace) << "CacheItem::CopyFrom Variant::SetValue error for " << name_;
		}

		changed_ = true;
	}

	void CacheItem::SetTimeStamp( const FILETIME& timestamp )
	{
		ftTimeStamp_ = timestamp;

		changed_ = true;
	}

	void CacheItem::SetQuality( const WORD quality )
	{
		wQuality_ = quality;

		changed_ = true;
	}

	void CacheItem::Reset()
	{
		if( wQuality_ == OPC_QUALITY_BAD )
		{
			return;
		}

		FILETIME timeStamp;
		::GetSystemTimeAsFileTime( &timeStamp );
		ftTimeStamp_ = timeStamp;

		wQuality_ = OPC_QUALITY_BAD;

		changed_ = true;
	}

	bool CacheItem::isChanged()
	{
		if ( changed_ )
		{
			changed_ = false;
			return true;
		}
		else
		{
			return false;
		}
	}

	string_t CacheItem::GetValueAsString() const
	{
		return vDataValue_.valueAsString();
	}

	string_t CacheItem::GetTypeAsString() const
	{
		return vDataValue_.typeAsString();
	}

	string_t CacheItem::GetQualityAsString() const
	{
		return OPCQualityAsString( wQuality_ );
	}

	string_t CacheItem::GetTimestampAsString() const
	{
		return FiletimeAsString(ftTimeStamp_);
	}

}