#ifndef ID_MGR_H
#define ID_MGR_H

#include "utils/types.h"
#include "utils/singleton.h"
#include <boost/noncopyable.hpp>

class IdMgr : private boost::noncopyable
{
	static uint32_t startId;
public:
	IdMgr();
	~IdMgr();

	uint32_t Generate();
};

typedef Singleton< IdMgr > idGenerator;

#endif // ID_MGR_H