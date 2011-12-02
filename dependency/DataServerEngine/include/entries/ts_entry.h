#ifndef TS_h__
#define TS_h__

#include "entries/entry.h"
#include "core/state_checker.h"
#include "utils/types.h"
#include "utils/string.h"
#include "utils/variant.h"
#include <list>
#include <map>

#include "../../jsoncpp-0.5.0/include/json/value.h"

namespace Channels
{
	class EntryTS : public Channels::Entry
	{
		string_t cmd_name_;
		std::map<string_t, std::list<StateCheckerPtr> > src_checkers_map_;

	public:
		EntryTS(const string_t& ts_name, const Json::Value& content);
		~EntryTS();

		void Init();

		// Entry interface
		void Update(const string_t& tag_name);
		bool IsChanged() const;
		std::list<string_t> GetSrcList() const;

		// EntryTS interface
		string_t GetCmdName() const;
	private:
		uint16_t ExecuteConvert(uint16_t state);
		uint16_t ExecuteCmd(uint16_t cmd, uint16_t state);
		void AddSrcChecker(const string_t& src_name, StateCheckerPtr checker);

	};

	typedef boost::shared_ptr<EntryTS> EntryTsPtr;
}

#endif // TS_h__
