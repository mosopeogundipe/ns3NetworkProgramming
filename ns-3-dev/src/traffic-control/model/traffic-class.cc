
#include "ns3/log.h"
#include <stdio.h>
#include <math.h>
#include "ns3/packet.h"
#include "ns3/ptr.h"
#include <queue>
#include <vector>
#include "ns3/traffic-class.h"
#include "ns3/uinteger.h"
#include "ns3/double.h"
#include "ns3/boolean.h"

namespace ns3 {


NS_LOG_COMPONENT_DEFINE ("TrafficClass");

NS_OBJECT_ENSURE_REGISTERED (TrafficClass);

TypeId 
TrafficClass::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::TrafficClass")
    .SetGroupName ("TrafficControl")
		.AddConstructor<TrafficClass> ()
		.AddAttribute ("MaxBytes",
									 "The maximum number of bytes that will be held in this TrafficClass' queue",
									 UintegerValue (UINT32_MAX),
									 MakeUintegerAccessor (&TrafficClass::maxBytes),
									 MakeUintegerChecker<uint32_t> ())
		.AddAttribute ("MaxPackets",
									 "The maximum number of packets that will be held in this TrafficClass' queue",
									 UintegerValue (UINT32_MAX),
									 MakeUintegerAccessor (&TrafficClass::maxPackets),
									 MakeUintegerChecker<uint32_t> ())
		.AddAttribute ("PriorityLevel",
									 "The priority level assigned to this TrafficClass",
									 UintegerValue (0),
									 MakeUintegerAccessor (&TrafficClass::priority_level),
									 MakeUintegerChecker<uint32_t> ())
		.AddAttribute ("Weight",
									 "The weight assigned to this TrafficClass",
									 DoubleValue (0),
									 MakeDoubleAccessor (&TrafficClass::weight),
									 MakeDoubleChecker<double_t> ())
		.AddAttribute ("IsDefault",
									 "Whether or not this is the default TrafficClass",
									 BooleanValue (false),
									 MakeBooleanAccessor (&TrafficClass::isDefault),
									 MakeBooleanChecker ())
  ;
  return tid;
}

TrafficClass::TrafficClass ()
{
	NS_LOG_FUNCTION (this);
}

TrafficClass::~TrafficClass ()
{
	NS_LOG_FUNCTION (this);
}

void
TrafficClass::Unref ()
{
	NS_LOG_FUNCTION (this);
}

bool
TrafficClass::IsDefault ()
{
	NS_LOG_FUNCTION (this);
	return isDefault;
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
TrafficClass::Match (ns3::Ptr<ns3::Packet> p)
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

