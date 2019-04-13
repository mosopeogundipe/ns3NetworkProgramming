
#include "ns3/log.h"
#include "ns3/enum.h"
#include "ns3/uinteger.h"
#include "diff-serv.h"
#include "ns3/traffic-class.h"

#define NOCLASSIFY 0xffffffff

namespace ns3 {

NS_LOG_COMPONENT_DEFINE("DiffServ");
NS_OBJECT_ENSURE_REGISTERED (DiffServ);

TypeId
DiffServ::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::DiffServ")
    .SetParent<QueueBase> ()
    .SetGroupName ("Network")
		.AddConstructor<DiffServ> ()
  ;
  return tid;
}

DiffServ::DiffServ ()
{
	NS_LOG_FUNCTION (this);
}

DiffServ::~DiffServ ()
{
	NS_LOG_FUNCTION (this);
}

bool
DiffServ::DoEnqueue (Ptr<Packet> p)
{
	NS_LOG_FUNCTION (this);

/*
	// this needs actual logic from QOS class
	uint32_t queuePos = Classify (p);

	if (q_class.empty () || queuePos >= q_class.size ())
	{
		// there is nothing in the queue, or the packet does not belong in any
		return false;
	}
*/
	return false;
}

Ptr<Packet>
DiffServ::DoDequeue ()
{
	NS_LOG_FUNCTION (this);

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
	NS_LOG_FUNCTION (this);

	// Actual logic should be same as DoDequeue
	// Will use the first TrafficClass by default
	//TrafficClass first = q_class.front ();
	return NULL;
}

Ptr<Packet>
DiffServ::DoPeek ()
{
	NS_LOG_FUNCTION (this);

	// Will use the first TrafficClass by default
	//TrafficClass first = q_class.front ();
	// No first.pop () because we are not dequeue-ing
	return NULL;
}

void
DiffServ::SetMode (QueueMode mode)
{
	NS_LOG_FUNCTION (this);

	m_mode = mode;
}

DiffServ::QueueMode
DiffServ::GetMode ()
{
	NS_LOG_FUNCTION (this);
	return m_mode;
}

Ptr<Packet>
DiffServ::Schedule ()
{
	NS_LOG_FUNCTION (this);

	// all we need to do here is call DoDequeue which should have QOS logic
	return DoDequeue ();
}

uint32_t
DiffServ::Classify (Ptr<Packet> p)
{
	NS_LOG_FUNCTION (this);

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
