#ifndef WXZIP_H
#define WXZIP_H

#define WIN32_LEAN_AND_MEAN

#include "../../FRL/include/frl_exception.h"
#include <list>


namespace Util
{
	namespace Zip
	{
		FRL_EXCEPTION_CLASS( FileNotExist );
		FRL_EXCEPTION_CLASS( InvalidUpdate );
		FRL_EXCEPTION_CLASS( InvalidArchive );

		std::string GetEntry(const std::string& filename, const std::string& entry);
		void UpdateEntry(const std::string& filename, const std::string& entry, const std::string& data);
		void GetListEntry(const std::string& filename, std::list<std::string>& list);
		bool IsValidArchive(const std::string& filename);
	}
}

#endif WXZIP_H