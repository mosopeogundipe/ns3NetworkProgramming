
#include "ns3/log.h"
#include <stdio.h>
#include <math.h>
#include "ns3/packet.h"
#include "ns3/ptr.h"
#include <queue>
#include <vector>
#include "ns3/traffic-class.h"

namespace ns3 {


NS_LOG_COMPONENT_DEFINE ("TrafficClass");

NS_OBJECT_ENSURE_REGISTERED (TrafficClass);

TypeId 
TrafficClass::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::TrafficClass")
    .SetParent<Object> ()
    .SetGroupName ("TrafficControl")
  ;
  return tid;
}

void
TrafficClass::Unref ()
{
	NS_LOG_FUNCTION (this);
}

TrafficClass::TrafficClass ()
{
	NS_LOG_FUNCTION (this);
}

TrafficClass::~TrafficClass ()
{
	NS_LOG_FUNCTION (this);
	delete &m_queue;
	delete &filters;
}

void
TrafficClass::SetBytes (uint32_t b)
{
	NS_LOG_FUNCTION (this << b);

	bytes = b;
}

void
TrafficClass::SetPackets (uint32_t p)
{
	NS_LOG_FUNCTION (this << p);

	packets = p;
}

void
TrafficClass::SetMaxPackets (uint32_t p)
{
	NS_LOG_FUNCTION (this << p);

	maxPackets = p;
}

void
TrafficClass::SetMaxBytes (uint32_t b)
{
	NS_LOG_FUNCTION (this << b);

	maxBytes = b;
}

void
TrafficClass::SetWeight (double_t w)
{
	NS_LOG_FUNCTION (this << w);

	weight = w;
}

double_t
TrafficClass::GetWeight ()
{
	NS_LOG_FUNCTION (this);

	return weight;
}

void
TrafficClass::SetPriorityLevel (uint32_t p)
{
	NS_LOG_FUNCTION (this << p);

	priority_level = p;
}

uint32_t
TrafficClass::GetPriorityLevel ()
{
	NS_LOG_FUNCTION (this);

	return priority_level;
}

void
TrafficClass::SetIsDefault (bool d)
{
	NS_LOG_FUNCTION (this << d);

	isDefault = d;
}

bool
TrafficClass::IsDefault ()
{
	NS_LOG_FUNCTION (this);

	return isDefault;
}

void
TrafficClass::SetMQueue (std::queue<Ptr<ns3::Packet>> m)
{
	NS_LOG_FUNCTION (this);

	m_queue = m;
}

bool
TrafficClass::Enqueue (Ptr<ns3::Packet> p)
{
	NS_LOG_FUNCTION (this);

	// if this queue is already full, we can't add more
	if (bytes >= maxBytes || packets >= maxPackets)
		{
			return false;
		}

	uint32_t packetBytes = p->GetSize ();

	// if adding this packet would result in too many bytes, return false
	// TODO does user call match or do we?
	if (bytes + packetBytes > maxBytes)
		{
			return false;
		}
	
	// at this point we know that the packet can fit in the queue
	m_queue.push (p);

	bytes += packetBytes;
	packets++;

	// method was allowed to run to completion, packet is now in queue
	return true;
}

Ptr<ns3::Packet>
TrafficClass::Dequeue ()
{
	NS_LOG_FUNCTION (this);

	// if queue is empty return null
	if (packets == 0)
		{
			return NULL;
		}

	Ptr<Packet> packet = m_queue.front ();
	m_queue.pop (); // this only removes the first item, doesn't return it

	packets--;
	bytes -= packet->GetSize ();

	return packet;
}

bool
TrafficClass::Match (Ptr<ns3::Packet> p)
{
	NS_LOG_FUNCTION (this);

	// specification says ANY filter should match, so if only 1 does
	// we should return true
	for (Filter filter : filters)
		{
			if (filter.Match (p))
				{
					return true;
				}
		}

	// none of the filters matched so we return false
	return false;
}

} // namespace ns3

