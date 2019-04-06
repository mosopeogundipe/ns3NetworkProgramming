
#include "ns3/packet.h"
#include "ns3/log.h"
#include "ns3/packet-filter.h"
#include <vector>
#include "ns3/ptr.h"
#include "filter.h"
#include "filter-element.h"


namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("Filter");
NS_OBJECT_ENSURE_REGISTERED (Filter);

TypeId
Filter::GetTypeId (void)
{
	static TypeId tid = TypeId ("ns3::Filter")
	.SetParent<Object> ()
	.SetGroupName("TrafficControl")
	;
	return tid;
}

Filter::Filter ()
{
	NS_LOG_FUNCTION (this);
}

Filter::~Filter ()
{
	NS_LOG_FUNCTION (this);
}

void
Filter::Unref ()
{
	NS_LOG_FUNCTION (this);
}

void
Filter::AddFilter (Ptr<FilterElement> element)
{
	NS_LOG_FUNCTION (this);
	elements.push_back (element);
}

bool
Filter::Match (Ptr<Packet> packet)
{
	NS_LOG_FUNCTION (this);
	for ( Ptr<FilterElement> element : elements)
		{
			if (!element->Match (packet))
				{
					return false;
				}
		}

	return true;
}

} // namespace ns3

