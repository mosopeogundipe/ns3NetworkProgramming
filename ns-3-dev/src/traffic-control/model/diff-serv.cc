
#include <queue>
#include "ns3/enum.h"
#include "ns3/uinteger.h"
#include "diff-serv.h"
#include "ns3/traffic-class.h"
#include "ns3/queue.h"

#define NOCLASSIFY 0xffffffff

namespace ns3 {

NS_LOG_COMPONENT_DEFINE("DiffServ");
NS_OBJECT_ENSURE_REGISTERED (DiffServ);

TypeId
DiffServ::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::DiffServ")
    .SetParent<Queue<ns3::Packet>> ()
    .SetGroupName ("TrafficControl")
		.AddConstructor<DiffServ> ()
  ;
  return tid;
}

//Methods that are virtual in Queue (parent)
bool
DiffServ::Enqueue (Ptr<Packet> p)
{
	return DoEnqueue (p);
}

Ptr<Packet>
DiffServ::Dequeue (void)
{
	return DoDequeue ();
}

Ptr<const Packet>
DiffServ::Peek (void) const
{
	
	return NULL;
}

Ptr<Packet>
DiffServ::Remove (void)
{
	return DoRemove ();
}

DiffServ::DiffServ ()
{
}

DiffServ::~DiffServ ()
{
}

void
DiffServ::AddTrafficClass (TrafficClass t)
{
	q_class.push_back (t);
}

bool
DiffServ::DoEnqueue (Ptr<Packet> p)
{
	// this needs actual logic from QOS class
	uint32_t queuePos = Classify (p);

	if (q_class.empty () || queuePos == NOCLASSIFY)
	{
		// there is nothing in the queue, or the packet does not belong in any
		return false;
	}

	TrafficClass target = q_class[queuePos];
	uint32_t pSize = p->GetSize ();

	if (m_mode == packet && target.GetPackets () < target.GetMaxPackets () - 1)
		{
			return target.Enqueue (p);
		}

	if (m_mode == byte && target.GetBytes () + pSize < target.GetMaxPackets ())
		{
			return target.Enqueue (p);
		}

	return false;
}

Ptr<Packet>
DiffServ::DoDequeue ()
{
	Ptr<Packet> packet;

	// Will use the first TrafficClass by default
	for (std::vector<TrafficClass>::iterator it = q_class.begin (); it != q_class.end (); ++it)
		{
			packet = it->Dequeue ();
			if (packet != NULL)
				{
					return packet;
				}
		}

	return NULL;
}

Ptr<Packet>
DiffServ::DoRemove ()
{
	// Actual logic should be same as DoDequeue
	return DoDequeue ();
}

Ptr<const Packet>
DiffServ::DoPeek ()
{
	//same logic as DoDequeue () but we don't remove the packet
	Ptr<Packet> packet;
	Ptr<const Packet> c_packet;

	// Will use the first TrafficClass by default
	for (std::vector<TrafficClass>::iterator it = q_class.begin (); it != q_class.end (); ++it)
		{
			packet = it->Peek ();
			if (packet != NULL)
				{
					c_packet = packet->Copy ();
					return c_packet;
				}
		}
	return NULL;
}

void
DiffServ::SetMode (QueueMode mode)
{
	m_mode = mode;
}

DiffServ::QueueMode
DiffServ::GetMode ()
{
	return m_mode;
}

Ptr<Packet>
DiffServ::Schedule ()
{
	// all we need to do here is call DoDequeue which should have QOS logic
	return DoDequeue ();
}

uint32_t
DiffServ::Classify (Ptr<Packet> p)
{
	// default is max value to signify no match
	uint32_t pos = NOCLASSIFY;
	uint32_t i = 0;
	TrafficClass target;

	for (std::vector<TrafficClass>::iterator it = q_class.begin (); it != q_class.end (); ++it)
		{
				target = (*it);
				// we want the default class, otherwise, the first matching class
				if (target.Match (p) && (pos == NOCLASSIFY || target.IsDefault ()))
					{
						pos = i;
					}
				i++;
		}

	return pos;
}
} // namespace ns3
