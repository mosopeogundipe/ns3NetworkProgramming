#include "ns3/log.h"
#include "ns3/enum.h"
#include "ns3/uinteger.h"
#include "diff-serv.h"
#include "ns3/traffic-class.h"
#include "strict-priority-queue.h"
#include <vector>
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
    .SetParent<DiffServ> ()
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
	CreateFilters();

    //ReadFromConfig(configFile);   // ---->>>> HINT.SOPE: Add logic in this function!

	//BUGS IN CODE BELOW!!!

    // if(number_of_queues == priority_levels.size()){
	// 	//sort in decreasing order of priority
	// 	std::cout << "Processing... " << number_of_queues << "queues" << std::endl;
	// 	std::sort(priority_levels.begin(), priority_levels.end(), std::greater<u_int32_t>()); 
	// 	std::vector<uint32_t>::iterator iter = priority_levels.begin();
	// 	//std::vector<Filter>::iterator filterIter = filters.begin();
	// 	FilterElement* fe;
	// 	Filter* filter;
	// 	//insert in decreasing order of priority. 
	// 	//Highest priority is the first queue in list, defualt queue is the last one in list
    //     for (int i=0; i<(int)number_of_queues; i++){
    //     TrafficClass* queue = new TrafficClass();   //traffic class represents our queue in this case	
	// 	queue->filters.clear();	
	// 	switch(i) {
	// 		case 0:
	// 		fe = (FilterElement*) new DestPortFilterElement(9999);
	// 		//fe = new DestPortFilterElement(9999);
	// 		filter = new Filter();
	// 		filter->AddFilter(fe);
	// 		queue->filters.push_back(filter);
	// 		//std::cout << "Got Here..." << std::endl;
	// 		//queue.filters.push_back(*filter);	
	// 		case 1:
	// 		fe = (FilterElement*) new DestPortFilterElement(1111);
	// 		//fe = new DestPortFilterElement(1111);
	// 		filter = new Filter();
	// 		filter->AddFilter(fe);
	// 		//queue.filters.push_back(*filter);	
	// 		queue->filters.push_back(filter);
	// 	}		
    //     queue->SetPriorityLevel(*iter);
    //     queue->SetDefault(false);
	// 	//queue.filters.push_back(*filterIter);
    //     q_class.push_back(queue);
    //     std::advance(iter, 1);
	// 	//std::advance(filterIter, 1);
    //     }
    //     //creating default queue, where packets not matching any filters go:
    //     TrafficClass* default_queue = new TrafficClass();
    //     default_queue->SetDefault(true);
	// 	//default_queue.filters.push_back(filters.back());//add low priority filter to default queue. Low priority filter is the last one in list, based on this design
    //     fe = (FilterElement*) new DestPortFilterElement(1111);
	// 	//fe = new DestPortFilterElement(1111);
	// 	filter = new Filter();
	// 	filter->AddFilter(fe);
	// 	//default_queue.filters.push_back(*filter);
	// 	default_queue->filters.push_back(filter);
	// 	q_class.push_back(default_queue);
	// 	std::cout << "Exiting SPQ constructor..." << std::endl;
    // }else{
    //     NS_ABORT_MSG("'Number of Queues' doesn't match 'Priority levels' in Config file!");
    // }
    
}

StrictPriorityQueue::~StrictPriorityQueue ()
{
	//NS_LOG_FUNCTION (this);
}


void
StrictPriorityQueue::CreateFilters(){
	TrafficClass* highpriorityqueue = new TrafficClass();
	TrafficClass* lowpriorityqueue = new TrafficClass();
	TrafficClass* defaultqueue = new TrafficClass();
	FilterElement* destPortHigh = new DestPortFilterElement(9999);

	Filter* highPriority = new Filter();
	highPriority -> AddFilter(destPortHigh); 
	highpriorityqueue->filters.push_back(highPriority);
	highpriorityqueue->SetPriorityLevel(2);
	//highpriorityqueue -> SetMaxPackets(1000);
    highpriorityqueue->SetDefault(false);
	q_class.push_back(highpriorityqueue);

	
	FilterElement* destPortLow = new DestPortFilterElement(1111);

	Filter* lowPriority = new Filter();
	lowPriority-> AddFilter(destPortLow); 
	lowpriorityqueue->filters.push_back(lowPriority);
	lowpriorityqueue->SetPriorityLevel(1);
	//lowpriorityqueue -> SetMaxPackets(1000);
    lowpriorityqueue->SetDefault(false);
	q_class.push_back(lowpriorityqueue);


	FilterElement* destPortDefault = new DestPortFilterElement(1111);

	Filter* defaultPriority = new Filter();
	defaultPriority-> AddFilter(destPortDefault); 
	defaultqueue->filters.push_back(defaultPriority);
	//defaultqueue -> SetMaxPackets(1000);
	defaultqueue->SetDefault(true);
	q_class.push_back(defaultqueue);

}

Ptr<Packet>
StrictPriorityQueue::Dequeue (void)
{
	std::cout << "SPQ Dequeue Called" << std::endl;
	//exit (0);
	//return DoDequeue ();
	return Schedule ();
}

Ptr<const Packet>
StrictPriorityQueue::Peek (void) const
{
	return DoPeek ();
}

Ptr<Packet>
StrictPriorityQueue::Remove (void)
{
	return StrictPriorityQueue::DoRemove ();
}

bool
StrictPriorityQueue::Enqueue (Ptr<Packet> p)
{
	//std::cout << "DoEnqueue: SPQ" << std::endl;
	// this needs actual logic from QOS class
	uint32_t queuePos = Classify (p);	//HINT.SOPE: Should I override classify function to add logic to classify as high and low priority packets?
    q_class[queuePos]->Enqueue(p);
	return true;
}
 
bool
StrictPriorityQueue::DoEnqueue (Ptr<Packet> p)
{
	//NS_LOG_FUNCTION (this);
	std::cout << "DoEnqueue: SPQ" << std::endl;
	// this needs actual logic from QOS class
	uint32_t queuePos = Classify (p);	//HINT.SOPE: Should I override classify function to add logic to classify as high and low priority packets?
    q_class[queuePos]->Enqueue(p);
	return true;    	
}

// bool
// StrictPriorityQueue::IsEnqueuingSuccessful(TrafficClass* queue, Ptr<Packet> p){
//     uint32_t pSize = p->GetSize ();
// 	std::cout << "Packet size: " << pSize << "Queue Mode: "<< GetMode()<< std::endl;
//     if (GetMode () == packet && queue->GetPackets () < queue->GetMaxPackets () - 1)
// 		{
// 			std::cout << "Entered condition 1: DiffServ" << std::endl;
// 			return queue->Enqueue (p);
// 		}

// 	if (GetMode () == byte && queue->GetBytes () + pSize < queue->GetMaxPackets ())
// 		{
// 			std::cout << "Entered condition 2: DiffServ" << std::endl;
// 			return queue->Enqueue (p);
// 		}
//     return false;
// }

Ptr<Packet>
StrictPriorityQueue::DoDequeue ()
{
	//NS_LOG_FUNCTION (this);
	std::cout << "DoDequeue || qclass size: " << q_class.size() << std::endl;
	Ptr<Packet> packet;

	// for (TrafficClass* tc : q_class) {
		
	// 	packet = tc->Dequeue ();
	// 	std::cout << "DoDequeue || Checking queue of priority " << tc->GetPriorityLevel() << std::endl;
	// 		if (packet != NULL)
	// 			{
	// 				std::cout << "DoDequeue || Returning packet from queue of priority " << tc->GetPriorityLevel() << std::endl;
	// 				return packet;
	// 			}
	// }

	if (!q_class[0]->m_queue.empty() && q_class[0] -> GetPriorityLevel() == 2){
		std::cout << "DoDequeue: entered 1"<<std::endl;
			packet = q_class[0]->Dequeue();
			return packet;
	}else if (!q_class[1]->m_queue.empty() && q_class[1] -> GetPriorityLevel() == 1){
			std::cout << "DoDequeue: entered 2"<<std::endl;
			packet = q_class[1]->Dequeue();
			return packet;
	}else{
			
		}

	return NULL;
}

Ptr<Packet>
StrictPriorityQueue::DoRemove ()
{
	//NS_LOG_FUNCTION (this);

	// Actual logic should be same as DoDequeue
	return DoDequeue ();
}

Ptr<const ns3::Packet>
StrictPriorityQueue::DoPeek () const
{
	//NS_LOG_FUNCTION (this);

	//same logic as DoDequeue () but we don't remove the packet
	Ptr<const ns3::Packet> packet;

	for (TrafficClass* tc : q_class) {
		packet = tc->Peek ();
		std::cout << "DoPeek || Checking queue of priority " << tc->GetPriorityLevel() << std::endl;
			if (packet != NULL)
				{
					std::cout << "DoPeek || Returning packet from queue of priority " << tc->GetPriorityLevel() << std::endl;
					return packet;
				}
	}
	return NULL;
}

Ptr<Packet>
StrictPriorityQueue::Schedule ()
{
	//NS_LOG_FUNCTION (this);
	//std::cout << "DoDequeue || qclass size: " << q_class.size() << std::endl;
	Ptr<Packet> packet;
	if(q_class[0]->m_queue.empty()){
		std::cout << "High is empty. Sending Low"<<std::endl;
	}
	if (!q_class[0]->m_queue.empty() && q_class[0] -> GetPriorityLevel() == 2){
		//std::cout << "DoDequeue: entered 1"<<std::endl;
			packet = q_class[0]->Dequeue();
			return packet;
	}else if (!q_class[1]->m_queue.empty() && q_class[1] -> GetPriorityLevel() == 1){
			std::cout << "DoDequeue: entered 2"<<std::endl;
			packet = q_class[1]->Dequeue();
			return packet;
	}else{
		if(!q_class[2]->m_queue.empty()){
			//std::cout << "DoDequeue: entered 3"<<std::endl;
			packet = q_class[2]->Dequeue();
			return packet;
		}			
	}

	return NULL;
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
