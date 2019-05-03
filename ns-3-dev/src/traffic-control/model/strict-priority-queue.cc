#include "ns3/log.h"
#include "ns3/enum.h"
#include "ns3/uinteger.h"
#include "diff-serv.h"
#include "ns3/traffic-class.h"
#include "strict-priority-queue.h"
#include <vector>
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
	configFile = "spq-config.txt";
	ReadFromConfig(configFile);
	// std::cout << "num "<< number_of_queues << std::endl;
	// std::cout << "priority_levels "<< priority_levels[0] << std::endl;
	// std::cout << "priority_levels "<< priority_levels[1] << std::endl;
	CreateFilters();    
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
	highpriorityqueue->SetPriorityLevel(priority_levels[1]);
	//highpriorityqueue -> SetMaxPackets(1000);
    highpriorityqueue->SetDefault(false);
	q_class.push_back(highpriorityqueue);

	
	FilterElement* destPortLow = new DestPortFilterElement(1111);

	Filter* lowPriority = new Filter();
	lowPriority-> AddFilter(destPortLow); 
	lowpriorityqueue->filters.push_back(lowPriority);
	lowpriorityqueue->SetPriorityLevel(priority_levels[0]);
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
StrictPriorityQueue::DoEnqueue (Ptr<Packet> p)
{
	//NS_LOG_FUNCTION (this);
	uint32_t queuePos = Classify (p);
    q_class[queuePos]->Enqueue(p);
	return true;    	
}

Ptr<Packet>
StrictPriorityQueue::DoDequeue ()
{
	//NS_LOG_FUNCTION (this);

	Ptr<Packet> packet;
	if (!q_class[0]->m_queue.empty() && q_class[0] -> GetPriorityLevel() == 2){
			packet = q_class[0]->Dequeue();
			return packet;
	}else if (!q_class[1]->m_queue.empty() && q_class[1] -> GetPriorityLevel() == 1){
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
	Ptr<Packet> packet;
	if(q_class[0]->m_queue.empty()){
		std::cout << "High is empty. Sending Low"<<std::endl;
	}
	if (!q_class[0]->m_queue.empty() && q_class[0] -> GetPriorityLevel() == 2){
			packet = q_class[0]->Dequeue();
			return packet;
	}else if (!q_class[1]->m_queue.empty() && q_class[1] -> GetPriorityLevel() == 1){
			packet = q_class[1]->Dequeue();
			return packet;
	}else{
		if(!q_class[2]->m_queue.empty()){
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
					if ( i == 0 )
					{
						number_of_queues = std::stoi (line);
					}
					else
					{
						priority_levels.push_back(std::stoi (line));
					}
					i++;
				}
		}
}

}
