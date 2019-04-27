
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
	std::cout << "Entered DiffServ: GetTypeId";
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
	return DoPeek ();
}

Ptr<Packet>
DiffServ::Remove (void)
{
	return DoRemove ();
}

DiffServ::DiffServ ()
{
<<<<<<< HEAD
	std::cout << "Entered DiffServ";
	//NS_LOG_FUNCTION (this);
=======
>>>>>>> 36b58395f837be7093a588cfaa49c751cd45d261
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

	if (m_mode == packet && target.GetPackets () < target.GetMaxPackets ())
		{
			return target.Enqueue (p);
		}

	if (m_mode == byte && target.GetBytes () + pSize <= target.GetMaxPackets ())
		{
			return target.Enqueue (p);
		}

	return false;
}

Ptr<Packet>
DiffServ::DoDequeue (void)
{
	std::cout << "DiffServ" << std::endl;
	exit (0);
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
DiffServ::DoRemove (void)
{
	// Actual logic should be same as DoDequeue
	return DoDequeue ();
}

Ptr<const Packet>
DiffServ::DoPeek (void) const
{
	//same logic as DoDequeue () but we don't remove the packet
	Ptr<const Packet> packet;

	// Will use the first TrafficClass by default
	for (std::vector<TrafficClass>::const_iterator it = q_class.begin (); it != q_class.end (); ++it)
		{
			packet = it->Peek ();
			if (packet != NULL)
				{
					return packet;
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
DiffServ::GetMode (void)
{
	return m_mode;
}

Ptr<Packet>
DiffServ::Schedule (void)
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
						break; // break out once matching queue class is found
					}
				i++;
		}

	return pos;
}
} // namespace ns3
