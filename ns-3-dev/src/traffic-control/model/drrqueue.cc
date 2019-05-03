#include "ns3/log.h"
#include "ns3/enum.h"
#include "ns3/uinteger.h"
#include "diff-serv.h"
#include "ns3/traffic-class.h"
//#include "ns3/drrqueue.h"
#include "drrqueue.h"
#include <bits/stdc++.h>
#include "dest-port-filter-element.h"
#include "filter.h"

#define NOCLASSIFY 0xffffffff

namespace ns3 {
   NS_LOG_COMPONENT_DEFINE("DRR");
   NS_OBJECT_ENSURE_REGISTERED(DRR);
	
DRR::DRR ()
{
//    NS_LOG_FUNCTION (this);	
	//num_queues = 0;	
	//configFile = "drr-config.txt";
	//ReadFromConfig (configFile);
	curr_queue_index = 0;
	CreateFilters();
	num_queues = 3;
//	quantum.push_back(300);
//	quantum.push_back(200);
//	quantum.push_back(100);
	deficit.push_back(300);
	deficit.push_back(200);
	deficit.push_back(100);
}

void
DRR::CreateFilters(){
	TrafficClass* highqueue = new TrafficClass();
	highqueue->SetWeight(300);
	TrafficClass* middlequeue = new TrafficClass();
	middlequeue->SetWeight(200);
	TrafficClass* lowqueue = new TrafficClass();
	lowqueue->SetWeight(100);
//	deficit.push_back(300);
//	deficit.push_back(200);
//	deficit.push_back(100);

	FilterElement* destPortHigh = (FilterElement*) new DestPortFilterElement(9999);
	Filter* highPriority = new Filter();
	highPriority -> AddFilter(destPortHigh);
	highqueue->filters.push_back(highPriority);
//	highqueue->SetPriorityLevel(3);
    highqueue->SetDefault(false);
	q_class.push_back(highqueue);

	FilterElement* destPortMiddle = (FilterElement*) new DestPortFilterElement(5555);
	Filter* middlePriority = new Filter();
	middlePriority-> AddFilter(destPortMiddle); 
	middlequeue->filters.push_back(middlePriority);
	middlequeue->SetDefault(false);
	q_class.push_back(middlequeue);
	
	FilterElement* destPortLow = (FilterElement*) new DestPortFilterElement(1111);
	Filter* lowPriority = new Filter();
	lowPriority-> AddFilter(destPortLow); 
	lowqueue->filters.push_back(lowPriority);
//	lowqueue->SetPriorityLevel(1);
	lowqueue->SetDefault(false);
	q_class.push_back(lowqueue);
}

DRR::~DRR ()
{
//	NS_LOG_FUNCTION (this);
}

TypeId
DRR::GetTypeId (void)
{
	static TypeId tid = TypeId ("ns3::DRR<Packet>")
		.SetParent<Object> ()
		.SetGroupName ("TrafficControl")
		.AddConstructor<DRR> ()
		;
	return tid;
}

Ptr<Packet>
DRR::Dequeue (void)
{
	std::cout << "drr dequeue" << std::endl;
	//exit (0);
	return DoDequeue ();
}

Ptr<const Packet>
DRR::Peek (void) const
{
	return DoPeek();
}

Ptr<Packet>
DRR::Remove (void)
{
	return DRR::DoRemove ();
}

Ptr<Packet>
DRR::DoRemove (void)
{
	return DoDequeue ();
}

Ptr<const Packet>
DRR::DoPeek (void) const
{
	std::cout << "DoPeek in DRR" << std::endl;
	exit (1);
	//same logic as DoDequeue () but we don't remove the packet
	Ptr<const Packet> packet;

	// Will use the first TrafficClass by default
	for (TrafficClass* tc : q_class) 
		{
			packet = tc->Peek ();
			if (packet != NULL)
				{
					return packet;
				}
		}
	return NULL;
}

Ptr<Packet>
DRR::DoDequeue() {
	std::cout << "DoDequeue DRR" << std::endl;
	uint16_t num_empty = 0;
	uint16_t size = 0;
	std::cout << "num_empty is: " << num_empty << std::endl;
	while(true) {
		if (curr_queue_index >= q_class.size ()) {
			std::cout << "curr_queue_index got reset to 0" << std::endl;
			curr_queue_index = 0;
		}
		//std::cout << "Num queues = " << num_queues << " , deficit = " << deficit[curr_queue_index] << std::endl;
		//exit (0);
		Ptr<const Packet>p = q_class[curr_queue_index]->Peek();
		std::cout << "got packet with size: " << p->GetSize () << "from index: " << curr_queue_index << std::endl;
		if (p==NULL) {
			std::cout << "Empty queue in DRR at index: " << curr_queue_index << std::endl;
			num_empty++;
			if (num_empty == num_queues) {
				std::cout << "num_empty == num_queues at: " << num_empty << std::endl;
				return NULL;
			}
			curr_queue_index++;
			continue;
		}
		deficit[curr_queue_index] = q_class[curr_queue_index]->GetWeight();
		size = p->GetSize ();
		std::cout << "DoDequeue Info" << std::endl;
		std::cout << p->GetSize () << std::endl;
		std::cout << deficit[curr_queue_index] << std::endl;
		std::cout << q_class[curr_queue_index]->GetWeight() << std::endl;
		bool i = size<=deficit[curr_queue_index];
		std::cout << "boolean is " << i << std::endl;
		if (size<=deficit[curr_queue_index]) {
			std::cout << "Sending packet with size: " << p->GetSize () << std::endl;
			deficit[curr_queue_index] = deficit[curr_queue_index] - p->GetSize();
			return q_class[curr_queue_index]->Dequeue();
		} else {
			deficit[curr_queue_index]+=q_class[curr_queue_index]->GetWeight();
			curr_queue_index++;
			std::cout << "Deficit too small, is now: " <<deficit[curr_queue_index - 1]<< std::endl;
			exit (0);
		}
	}
}

//the number of queues and the corresponding quantum value for each queue
void
DRR::ReadFromConfig(std::string configFile) {
	std::string line;
	std::ifstream readFile (configFile);
	if (readFile.is_open ())
		{
			uint8_t i = 0;
			while (getline (readFile,line) )
				{
					if ( i == 0 )
					{
						num_queues = std::stoi (line);
					}
					else
					{
						std::cout << "adding to vectors " << i << std::endl;
						quantum[i-1] = std::stoi (line);
						deficit[i-1] = quantum[i-1];
					}
					i++;
				}
		}
}

}