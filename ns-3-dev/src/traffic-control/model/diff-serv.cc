
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
	return DoPeek ();
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

bool
DiffServ::DoEnqueue (Ptr<Packet> p)
{
	//NS_LOG_FUNCTION (this);
	uint32_t queuePos = Classify (p);
	TrafficClass* target;    

	if (q_class.empty ())
	{
		// there is nothing in the queue
		return false;
	}

    if (queuePos == NOCLASSIFY)
	{
		//the packet does not belong in any queue, add to default queue
        target = q_class.back();   //default queue is always last in list, according to my design
		IsEnqueuingSuccessful(target, p);
	}else{
		target = q_class[queuePos];	
	}

    return IsEnqueuingSuccessful(target, p);		
}

bool
DiffServ::IsEnqueuingSuccessful(TrafficClass* queue, Ptr<Packet> p){
    uint32_t pSize = p->GetSize ();
    if (GetMode () == packet && queue->GetPackets () < queue->GetMaxPackets () - 1)
		{
			return queue->Enqueue (p);
		}

	if (GetMode () == byte && queue->GetBytes () + pSize < queue->GetMaxPackets ())
		{
			return queue->Enqueue (p);
		}
    return false;
}

Ptr<Packet>
DiffServ::DoDequeue (void)
{
	Ptr<Packet> packet;

	for (TrafficClass* tc : q_class) {
		packet = tc->Dequeue ();
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
	return DoDequeue ();
}

Ptr<const Packet>
DiffServ::DoPeek (void) const
{
	//same logic as DoDequeue () but we don't remove the packet
	Ptr<const Packet> packet;
	for (TrafficClass* tc : q_class) {
		packet = tc->Peek ();
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
	for (TrafficClass* tc : q_class) {
		// we want the default class, otherwise, the first matching class
		if (tc->Match (p) || tc->IsDefault ())
			{
				pos = i;
				break; // break out once matching queue class is found
			}
		i++;
	}

	return pos;
}
} // namespace ns3
