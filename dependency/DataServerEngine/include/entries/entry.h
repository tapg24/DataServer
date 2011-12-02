#ifndef entry_h__
#define entry_h__

#include "channels/cache_item.h"
#include "utils/string.h"
#include "utils/variant.h"
#include <list>
#include <boost/shared_ptr.hpp>

namespace Channels
{
	enum EntryType
	{
		NullEntry_Type = 0,
		DirectEntry_Type,
		TS_Type
	};

	class Entry
	{
	protected:
		string_t name_;
		EntryType type_;

	public:
		Entry(const string_t& name, EntryType type);
		virtual ~Entry();

		string_t GetName() const;
		EntryType GetType() const;
		ComVariant GetValue();
		WORD GetQuality() const;

		// virtual interface
		virtual void Update(const string_t& tag) = 0;
		virtual bool IsChanged() const = 0;
		virtual std::list<string_t> GetSrcList() const = 0;		
	};

	typedef boost::shared_ptr<Entry> EntryPtr;
}

#endif // entry_h__
