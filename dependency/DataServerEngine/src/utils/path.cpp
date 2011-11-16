#include "utils/path.h"
#include <Windows.h>

namespace Util
{
	boost::filesystem::path GetModulePath()
	{
		TCHAR buffer[MAX_PATH];
		GetModuleFileName ( NULL, buffer, sizeof(buffer) );
		boost::filesystem::path module_path(buffer);
		return module_path.remove_filename();
	}
}