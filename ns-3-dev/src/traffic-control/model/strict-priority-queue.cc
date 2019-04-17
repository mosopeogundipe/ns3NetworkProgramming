#include "ns3/log.h"
#include "ns3/enum.h"
#include "ns3/uinteger.h"
#include "diff-serv.h"
#include "ns3/traffic-class.h"
#include "ns3/strict-priority-queue.h"
#include <bits/stdc++.h>

#define NOCLASSIFY 0xffffffff

namespace ns3 {
NS_LOG_COMPONENT_DEFINE("StrictPriorityQueue");
NS_OBJECT_ENSURE_REGISTERED (StrictPriorityQueue);

TypeId
StrictPriorityQueue::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::StrictPriorityQueue")
    .SetParent<DiffServ> ()
    .SetGroupName ("TrafficControl")
		.AddConstructor<StrictPriorityQueue> ()
  ;
  return tid;
}
StrictPriorityQueue::StrictPriorityQueue(){
	NS_LOG_FUNCTION (this);
}
StrictPriorityQueue::StrictPriorityQueue (std::string config_file_name)
{
	NS_LOG_FUNCTION (this);
    ReadFromConfig(config_file_name);   // ---->>>> HINT.SOPE: Add logic in this function!
    if(number_of_queues == priority_levels.size()){
		//sort in decreasing order of priority
		std::sort(priority_levels.begin(), priority_levels.end(), std::greater<u_int32_t>()); 
        std::vector<uint32_t>::iterator iter = priority_levels.begin();
		//insert in decreasing order of priority. 
		//Highest priority is the first queue in list, defualt queue is the last one in list
        for (int i=0; i<(int)number_of_queues; i++){
        TrafficClass queue;   //traffic class represents our queue in this case
        queue.SetPriorityLevel(*iter);
        queue.SetDefault(false);
        q_class.push_back(queue);
        std::advance(iter, 1);
        }
        //creating default queue, where packets not matching any filters go:
        TrafficClass default_queue;
        default_queue.SetDefault(true);
        q_class.push_back(default_queue);
    }else{
        NS_ABORT_MSG("'Number of Queues' doesn't match 'Priority levels' in Config file!");
    }
    
}

StrictPriorityQueue::~StrictPriorityQueue ()
{
	NS_LOG_FUNCTION (this);
}

bool
StrictPriorityQueue::DoEnqueue (Ptr<Packet> p)
{
	NS_LOG_FUNCTION (this);

	// this needs actual logic from QOS class
	uint32_t queuePos = Classify (p);	//HINT.SOPE: Should I override classify function to add logic to classify as high and low priority packets?
    TrafficClass target;    

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
	}

	target = q_class[queuePos];		//HINT.SOPE: Should I change this and get queue where queuePos == queue's priority?
    return IsEnqueuingSuccessful(target, p);	
	
}

bool
StrictPriorityQueue::IsEnqueuingSuccessful(TrafficClass queue, Ptr<Packet> p){
    uint32_t pSize = p->GetSize ();
    if (GetMode () == packet && queue.GetPackets () < queue.GetMaxPackets () - 1)
		{
			return queue.Enqueue (p);
		}

	if (GetMode () == byte && queue.GetBytes () + pSize < queue.GetMaxPackets ())
		{
			return queue.Enqueue (p);
		}
    return false;
}

Ptr<Packet>
StrictPriorityQueue::DoDequeue ()
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
StrictPriorityQueue::DoRemove ()
{
	NS_LOG_FUNCTION (this);

	// Actual logic should be same as DoDequeue
	return DoDequeue ();
}

Ptr<Packet>
StrictPriorityQueue::DoPeek ()
{
	NS_LOG_FUNCTION (this);

	//same logic as DoDequeue () but we don't remove the packet
	Ptr<Packet> packet;

	// Will use the first TrafficClass by default
	for (std::vector<TrafficClass>::iterator it = q_class.begin (); it != q_class.end (); ++it)
		{
			packet = it->Peek ();
			if (packet != NULL)
				{
					return packet;
				}
		}
	return NULL;
}

Ptr<Packet>
StrictPriorityQueue::Schedule ()
{
	NS_LOG_FUNCTION (this);

	// all we need to do here is call DoDequeue which should have QOS logic
	return DoDequeue ();
}

void
StrictPriorityQueue::ReadFromConfig(std::string config_file_name){
    //Logic that reads the number_of_queues and priority_levels from config goes here...
}

}