#ifndef default_h__
#define default_h__

#include "entries/entry.h"
#include "utils/string.h"
#include "utils/variant.h"

#include "../../jsoncpp-0.5.0/include/json/value.h"

namespace Channels
{
	class DefaultEntry : public Channels::Entry
	{
	public:
		DefaultEntry(const string_t& name, const Json::Value& convert);
		~DefaultEntry();

		// Entry interface
		void Update(const string_t& tag_name);
		bool IsChanged() const;
		std::list<string_t> GetSrcList() const;		

	private:
		string_t src_name_;
		// DefaultEntry interface
		void SetSrc(const string_t& src_name);

	};

	typedef boost::shared_ptr<DefaultEntry> DefaultEntryPtr;

}

#endif // default_h__
