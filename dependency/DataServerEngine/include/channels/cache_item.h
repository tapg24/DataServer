#ifndef CACHE_ITEM_H_NEW
#define CACHE_ITEM_H_NEW

#include <string>
#include <map>
#include "utils/variant.h"
#include "utils/string.h"
#include "../../opc_foundation/opcda.h"
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

namespace Channels
{
	class CacheItem : boost::noncopyable
	{
		string_t name_;
		FILETIME ftTimeStamp_;
//		FILETIME ftCacheTimeStamp_;
		bool changed_;
		WORD wQuality_;
		ComVariant vDataValue_;
//		ComVariant vCacheDataValue_;
		HRESULT error;

	public:
		CacheItem(const string_t& name, const VARIANT& value);
		CacheItem(const string_t& name, const VARIANT& value, const WORD quality);
		CacheItem(const string_t& name, const FILETIME& timestamp, const WORD quality, const VARIANT& value);
		~CacheItem();

		void CopyFrom(const CacheItem& item);
		void CopyFrom(const WORD quality, const VARIANT& value);
		void CopyFrom(const FILETIME& timestamp, const WORD quality, const VARIANT& value);
		string_t GetName() const;
		ComVariant GetValue() const;
		string_t GetValueAsString() const;
		string_t GetTypeAsString() const;
		FILETIME GetTimeStamp() const;
		string_t GetTimestampAsString() const;
		WORD GetQuality() const;
		string_t GetQualityAsString() const;
		void SetName(const string_t& name);
		void SetValue(const VARIANT& variant);
		void SetTimeStamp(const FILETIME& timestamp);
		void SetQuality(const WORD quality);
		void Reset();

		bool isChanged();
	};


	typedef boost::shared_ptr<CacheItem> CacheItemPtr;
	typedef std::map<string_t, CacheItemPtr> CacheContainer;
}

#endif // CACHE_ITEM_H_NEW