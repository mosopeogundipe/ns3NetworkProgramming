
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
	std::cout << "Entered DiffServ: GetTypeId" << std::endl;
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
	std::cout << "Enqueue in Diffserv" << std::endl;
	return DoEnqueue (p);
}

Ptr<Packet>
DiffServ::Dequeue (void)
{
	std::cout << "Dequeue in Diffserv" << std::endl;
	exit (0);
	return DoDequeue ();
}

Ptr<const Packet>
DiffServ::Peek (void) const
{
	std::cout << "diffserv Peek" << std::endl;
	exit (0);
	return DoPeek ();
}

Ptr<Packet>
DiffServ::Remove (void)
{
	std::cout << "diffserv Remove" << std::endl;
	exit (0);
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
	std::cout << "DoEnqueue: DiffServ" << std::endl;
	// this needs actual logic from QOS class
	uint32_t queuePos = Classify (p);	//HINT.SOPE: Should I override classify function to add logic to classify as high and low priority packets?
    std::cout << "Diffserv classify: "<< queuePos << std::endl;
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
		target = q_class[queuePos];		//HINT.SOPE: Should I change this and get queue where queuePos == queue's priority?
	}

    return IsEnqueuingSuccessful(target, p);		
}

bool
DiffServ::IsEnqueuingSuccessful(TrafficClass* queue, Ptr<Packet> p){
    uint32_t pSize = p->GetSize ();
	std::cout << "Packet size: " << pSize << "Queue Mode: "<< GetMode()<< std::endl;
	std::cout << "GetPackets: " << queue->GetPackets () << "MaxPackets: " << queue->GetMaxPackets () <<std::endl;
    if (GetMode () == packet && queue->GetPackets () < queue->GetMaxPackets () - 1)
		{
			std::cout << "Entered condition 1: DiffServ" << std::endl;
			return queue->Enqueue (p);
		}

	if (GetMode () == byte && queue->GetBytes () + pSize < queue->GetMaxPackets ())
		{
			std::cout << "Entered condition 2: DiffServ" << std::endl;
			return queue->Enqueue (p);
		}
    return false;
}

Ptr<Packet>
DiffServ::DoDequeue (void)
{
	std::cout << "DiffServ DoDequeue" << std::endl;
	exit (0);
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
	std::cout << "diffserv DoRemove" << std::endl;
	exit (0);
	// Actual logic should be same as DoDequeue
	return DoDequeue ();
}

Ptr<const Packet>
DiffServ::DoPeek (void) const
{
	std::cout << "diffserv DoPeek" << std::endl;
	exit (0);
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
	std::cout << "diffserv Schedule" << std::endl;
	exit (0);
	// all we need to do here is call DoDequeue which should have QOS logic
	return DoDequeue ();
}

uint32_t
DiffServ::Classify (Ptr<Packet> p)
{
	// default is max value to signify no match
	uint32_t pos = NOCLASSIFY;
	uint32_t i = 0;
	//TrafficClass* target;
	//std::cout << "q_class size: "<< q_class.size() << std::endl;
	for (TrafficClass* tc : q_class) {
		//std::cout << "classify i = " << i << std::endl;
		// we want the default class, otherwise, the first matching class
		if (tc->Match (p) || tc->IsDefault ())
			{
				//std::cout << "matched with queue at: " << i << std::endl;
				//exit (0);
				pos = i;
				break; // break out once matching queue class is found
			}
		i++;
	}

	return pos;
}
} // namespace ns3
