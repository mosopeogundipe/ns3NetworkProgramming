#include "ns3/log.h"
#include "ns3/enum.h"
#include "ns3/uinteger.h"
#include "diff-serv.h"
#include "ns3/traffic-class.h"
#include "strict-priority-queue.h"
//#include "ns3/source-addr-filter-element.h"
//#include "ns3/source-port-filter-element.h"
//#include "ns3/dest-addr-filter-element.h"
#include "dest-port-filter-element.h"
#include "filter.h"
#include <bits/stdc++.h>

#define NOCLASSIFY 0xffffffff

namespace ns3 {
NS_LOG_COMPONENT_DEFINE("StrictPriorityQueue");
NS_OBJECT_ENSURE_REGISTERED (StrictPriorityQueue);

TypeId
StrictPriorityQueue::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::StrictPriorityQueue<Packet>")
    .SetParent<Object> ()
    .SetGroupName ("TrafficControl")
		.AddConstructor<StrictPriorityQueue> ()
  ;
  return tid;
}

StrictPriorityQueue::StrictPriorityQueue ()
{
	//NS_LOG_FUNCTION (this);
	std::cout << "SPQ default constructor" << std::endl;
	//exit (0);
	configFile = "spq-config.txt";
	
    ReadFromConfig(configFile);   // ---->>>> HINT.SOPE: Add logic in this function!
    if(number_of_queues == priority_levels.size()){
		//sort in decreasing order of priority
		std::cout << "Processing... " << number_of_queues << "queues" << std::endl;
		std::sort(priority_levels.begin(), priority_levels.end(), std::greater<u_int32_t>()); 
        std::vector<uint32_t>::iterator iter = priority_levels.begin();
		//std::vector<Filter>::iterator filterIter = filters.begin();
		FilterElement* fe;
		Filter* filter;
		//insert in decreasing order of priority. 
		//Highest priority is the first queue in list, defualt queue is the last one in list
        for (int i=0; i<(int)number_of_queues; i++){
        TrafficClass queue;   //traffic class represents our queue in this case
		switch(i) {
			case 0:
			fe = (FilterElement*) new DestPortFilterElement(9999);
			filter = new Filter();
			filter->AddFilter(fe);
			queue.filters.push_back(*filter);	
			case 1:
			fe = (FilterElement*) new DestPortFilterElement(1111);
			filter = new Filter();
			filter->AddFilter(fe);
			queue.filters.push_back(*filter);	
		}
        queue.SetPriorityLevel(*iter);
        queue.SetDefault(false);
		//queue.filters.push_back(*filterIter);
        q_class.push_back(queue);
        std::advance(iter, 1);
		//std::advance(filterIter, 1);
        }
        //creating default queue, where packets not matching any filters go:
        TrafficClass default_queue;
        default_queue.SetDefault(true);
		//default_queue.filters.push_back(filters.back());//add low priority filter to default queue. Low priority filter is the last one in list, based on this design
        fe = (FilterElement*) new DestPortFilterElement(1111);
		filter = new Filter();
		filter->AddFilter(fe);
		default_queue.filters.push_back(*filter);
		q_class.push_back(default_queue);
    }else{
        NS_ABORT_MSG("'Number of Queues' doesn't match 'Priority levels' in Config file!");
    }
    
}

StrictPriorityQueue::~StrictPriorityQueue ()
{
	//NS_LOG_FUNCTION (this);
}


// void
// StrictPriorityQueue::CreateFilters(){	
// 	SourceAddrFilterElement srcAddrHigh = SourceAddrFilterElement(Ipv4Address("10.0.1.0"));
// 	SourcePortFilterElement srcPortHigh = SourcePortFilterElement(9999);
// 	DestAddrFilterElement destAddrHigh = DestAddrFilterElement(Ipv4Address("255.255.255.0"));
// 	DestPortFilterElement destPortHigh = DestPortFilterElement(9999);

// 	Filter highPriority;
// 	highPriority.AddFilter(&srcAddrHigh);
// 	highPriority.AddFilter(&srcPortHigh);
// 	highPriority.AddFilter(&destAddrHigh);
// 	highPriority.AddFilter(&destPortHigh); 
// 	filters.push_back(highPriority);

// 	SourceAddrFilterElement srcAddrLow = SourceAddrFilterElement(Ipv4Address("10.0.1.0"));
// 	SourcePortFilterElement srcPortLow = SourcePortFilterElement(1111);
// 	DestAddrFilterElement destAddrLow = DestAddrFilterElement(Ipv4Address("255.255.255.0"));
// 	DestPortFilterElement destPortLow = DestPortFilterElement(1111);

// 	Filter lowPriority;
// 	lowPriority.AddFilter(&srcAddrLow);
// 	lowPriority.AddFilter(&srcPortLow);
// 	lowPriority.AddFilter(&destAddrLow);
// 	lowPriority.AddFilter(&destPortLow); 
// 	filters.push_back(lowPriority);
// }

bool
StrictPriorityQueue::DoEnqueue (Ptr<Packet> p)
{
	//NS_LOG_FUNCTION (this);
	std::cout << "DoEnqueue: " << std::endl;
	exit(0);
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
	}else{
		target = q_class[queuePos];		//HINT.SOPE: Should I change this and get queue where queuePos == queue's priority?
	}

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
	//NS_LOG_FUNCTION (this);
	std::cout << "DoDequeue: " << std::endl;
	Ptr<Packet> packet;

	// Will use the first TrafficClass by default
	for (std::vector<TrafficClass>::iterator it = q_class.begin (); it != q_class.end (); ++it)
		{
			packet = it->Dequeue ();
			std::cout << "DoDequeue || Checking queue of priority " << it->GetPriorityLevel() << std::endl;
			if (packet != NULL)
				{
					std::cout << "DoDequeue || Returning packet from queue of priority " << it->GetPriorityLevel() << std::endl;
					return packet;
				}
		}

	return NULL;
}

// Ptr<Packet>
// StrictPriorityQueue::DoRemove ()
// {
// 	//NS_LOG_FUNCTION (this);

// 	// Actual logic should be same as DoDequeue
// 	return DoDequeue ();
// }

Ptr<const ns3::Packet>
StrictPriorityQueue::DoPeek () const
{
	//NS_LOG_FUNCTION (this);

	//same logic as DoDequeue () but we don't remove the packet
	Ptr<const ns3::Packet> packet;

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

Ptr<Packet>
StrictPriorityQueue::Schedule ()
{
	//NS_LOG_FUNCTION (this);

	// all we need to do here is call DoDequeue which should have QOS logic
	return DoDequeue ();
}

void
StrictPriorityQueue::ReadFromConfig(std::string config_file_name){
    //Logic that reads the number_of_queues and priority_levels from config goes here...
	//Format: line 1 - number of queues, lines 2 - k contains priority levels, lines k - n contains filter details
	std::cout << "Reading from Config File: " << config_file_name << std::endl;
	std::string line;
	std::ifstream readFile (config_file_name);
	if (readFile.is_open ())
		{
			std::cout << "File is Open" << std::endl;
			uint8_t i = 0;
			while (getline (readFile,line) )
				{
					std::cout << "i : " << int(i) << std::endl;		
					if ( i == 0 )
					{
						number_of_queues = std::stoi (line);
					}
					else
					{
						//priority_levels [i - 1] = std::stoi (line);
						priority_levels.push_back(std::stoi (line));
					}
					i++;
				}
		}
}

}
