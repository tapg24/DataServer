#include "utils/id_mgr.h"

uint32_t IdMgr::startId = 0;

IdMgr::IdMgr()
{
}

IdMgr::~IdMgr()
{
}

uint32_t IdMgr::Generate()
{
	return ++startId;
}