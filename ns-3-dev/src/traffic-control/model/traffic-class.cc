
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
}

void
TrafficClass::SetBytes (uint32_t b) {
	NS_LOG_FUNCTION (this << b);

	bytes = b;
}

void
TrafficClass::SetPackets (uint32_t p) {
	NS_LOG_FUNCTION (this << p);

	packets = p;
}

void
TrafficClass::SetMaxPackets (uint32_t p) {
	NS_LOG_FUNCTION (this << p);

	maxPackets = p;
}

void
TrafficClass::SetMaxBytes (uint32_t b) {
	NS_LOG_FUNCTION (this << b);

	maxBytes = b;
}

void
TrafficClass::SetWeight (double_t w) {
	NS_LOG_FUNCTION (this << w);

	weight = w;
}

void
TrafficClass::SetPriorityLevel (uint32_t p) {
	NS_LOG_FUNCTION (this << p);

	priority_level = p;
}

void
TrafficClass::SetIsDefault (bool d) {
	NS_LOG_FUNCTION (this << d);

	isDefault = d;
}

void
TrafficClass::SetMQueue (std::queue<Ptr<ns3::Packet>> m) {
	NS_LOG_FUNCTION (this);

	m_queue = m;
}

bool
TrafficClass::Enqueue (Ptr<ns3::Packet> p)
{
	NS_LOG_FUNCTION (this);

	//TODO
	return false;
}

Ptr<ns3::Packet>
TrafficClass::Dequeue ()
{
	NS_LOG_FUNCTION (this);

	return NULL;
}

bool
TrafficClass::Match (Ptr<ns3::Packet> p)
{
	NS_LOG_FUNCTION (this);

	return false;
}

} // namespace ns3

