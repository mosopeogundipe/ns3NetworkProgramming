
#include "ns3/packet.h"
#include "ns3/log.h"
#include "ns3/packet-filter.h"
#include <vector>
#include "ns3/ptr.h"
#include "filter-list.h"


namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("FilterList");
NS_OBJECT_ENSURE_REGISTERED (FilterList);

TypeId
FilterList::GetTypeId (void)
{
	static TypeId tid = TypeId ("ns3::FilterList")
	.SetParent<Object> ()
	.SetGroupName("TrafficControl")
	;
	return tid;
}

FilterList::FilterList ()
{
	NS_LOG_FUNCTION (this);
}

FilterList::~FilterList ()
{
	NS_LOG_FUNCTION (this);
}

void
FilterList::AddFilter (Ptr<PacketFilter> filter)
{
	NS_LOG_FUNCTION (this);
	filters.push_back (filter);
}

bool
FilterList::Match (Ptr<Packet> packet)
{
	NS_LOG_FUNCTION (this);
	return false;
}

} // namespace ns3

