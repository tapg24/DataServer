#include "../dependency/DataServerEngine/include/utils/string.h"
#include "../dependency/DataServerEngine/include/core/project_mgr.h"
#include "../dependency/DataServerEngine/include/channels/channel_mgr.h"
#include "../dependency/DataServerEngine/include/channels/cache_mgr.h"
#include "../dependency/DataServerEngine/include/channels/channel_opc/channel_opc.h"

#include <set>

#define BOOST_TEST_MODULE DataServerEngineTestModule

#include <boost/test/included/unit_test.hpp>

namespace impl {
} // namespace impl

BOOST_AUTO_TEST_CASE(opc_channel_test_case) {

	projectMgr::getInstance().LoadProject();
	Channels::ChannelMgr* channelMgr = projectMgr::getInstance().GetChannelMgr();
	BOOST_CHECK( channelMgr );

	Channels::OPC::ChannelOPC* channel = new Channels::OPC::ChannelOPC("server", "localhost", "KEPware.KEPServerEx.V4");
	BOOST_CHECK( channel );

	channel->Start();
	std::set<string_t> items;
	items.insert("OPC_1.0000001.f1");
	items.insert("OPC_1.0000001.DI16");
	items.insert("OPC_1.0000001.CMD");
	channel->AddItems(items);
	BOOST_CHECK_EQUAL( channel->ItemCount(), items.size() );

	channelMgr->Attach(channel);
	BOOST_CHECK_EQUAL( channelMgr->Count(), 1 );

	projectMgr::getInstance().CloseProject();

}