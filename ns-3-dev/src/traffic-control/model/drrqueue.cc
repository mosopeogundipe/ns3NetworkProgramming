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
	quantum.push_back(300);
	quantum.push_back(200);
	quantum.push_back(100);
	deficit.push_back(300);
	deficit.push_back(200);
	deficit.push_back(100);
}

void
DRR::CreateFilters(){
	TrafficClass* highqueue = new TrafficClass();
	TrafficClass* middlequeue = new TrafficClass();
	TrafficClass* lowqueue = new TrafficClass();

	FilterElement* destPortHigh = new DestPortFilterElement(9999);

	Filter* highPriority = new Filter();
	highPriority -> AddFilter(destPortHigh); 
	highqueue->filters.push_back(highPriority);
	//highqueue->SetPriorityLevel(2);
	//highpriorityqueue -> SetMaxPackets(1000);
    highqueue->SetDefault(false);
	highqueue->SetWeight(300);
	q_class.push_back(highqueue);

	FilterElement* destPortMiddle = new DestPortFilterElement(5555);

	Filter* middlePriority = new Filter();
	middlePriority-> AddFilter(destPortMiddle); 
	middlequeue->filters.push_back(middlePriority);
	//defaultqueue -> SetMaxPackets(1000);
	middlequeue->SetDefault(false);
	middlequeue->SetWeight(200);
	q_class.push_back(middlequeue);
	
	FilterElement* destPortLow = new DestPortFilterElement(1111);

	Filter* lowPriority = new Filter();
	lowPriority-> AddFilter(destPortLow); 
	lowqueue->filters.push_back(lowPriority);
	//lowqueue->SetPriorityLevel(1);
	//lowpriorityqueue -> SetMaxPackets(1000);
    lowqueue->SetDefault(false);
	lowqueue->SetWeight(100);
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
        .SetParent<DiffServ> ()
        .SetGroupName ("TrafficControl")
	.AddConstructor<DRR> ()
	;
	return tid;
}

// bool
// DRR::Enqueue (Ptr<Packet> p)
// {
// 	////std::cout << "DoEnqueue: SPQ" << std::endl;
// 	// this needs actual logic from QOS class
// 	uint32_t queuePos = Classify (p);	//HINT.SOPE: Should I override classify function to add logic to classify as high and low priority packets?
//     q_class[queuePos]->Enqueue(p);
// 	return true;
// }

bool
DRR::DoEnqueue (Ptr<Packet> p)
{
	uint32_t queuePos = Classify (p);	//HINT.SOPE: Should I override classify function to add logic to classify as high and low priority packets?
    q_class[queuePos]->Enqueue(p);
	return true;		
}

Ptr<Packet>
DRR::Dequeue (void)
{
	return Schedule();
}

Ptr<const Packet>
DRR::Peek (void) const
{
	//std::cout << "drr peek" << std::endl;
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
	//same logic as DoDequeue () but we don't remove the packet
	//std::cout<<"DoPeek1"<< std::endl;
	Ptr<const Packet> packet;
	for (TrafficClass* tc : q_class) 
		{
			packet = tc->Peek ();
			//std::cout<<"DoPeek2"<< std::endl;
			if (packet != NULL)
				{
					return packet;
				}
		}
	return NULL;
}

Ptr<Packet>
DRR::DoDequeue() {
	//std::cout << "DoDequeue || qclass size: " << q_class.size() << std::endl;
	uint16_t num_empty = 0;
	while(true) {
		//std::cout << "Num queues = " << num_queues << " , deficit = " << deficit[curr_queue_index] << " curr queue index: "<< int(curr_queue_index) << std::endl;
		//exit (0);
		if (curr_queue_index>num_queues-1){
			curr_queue_index = 0;
		}
		////std::cout<<q_class[curr_queue_index]<<std::endl;
		Ptr<const Packet>p = q_class[curr_queue_index]->Peek();
		//std::cout << "packet size: " << p->GetSize() << std::endl;
		if (p==NULL) {
			num_empty++;
			if (num_empty == num_queues) {
				return NULL;
			}
			curr_queue_index++;
			continue;
		}
		if (p->GetSize()<=deficit[curr_queue_index]) {
			//std::cout << "Dequeueing at queue index:" << int(curr_queue_index) << std::endl;
			deficit[curr_queue_index] = deficit[curr_queue_index] - p->GetSize();
			return q_class[curr_queue_index]->Dequeue();
		} else {
			//std::cout << "queue index: " << int(curr_queue_index) << " weight: " << q_class[curr_queue_index]->GetWeight() << std::endl;
			deficit[curr_queue_index]+=uint32_t(q_class[curr_queue_index]->GetWeight());
			curr_queue_index++;
		}
	}
}

Ptr<Packet>
DRR::Schedule ()
{
	//std::cout << "Schedule || qclass size: " << q_class.size() << std::endl;
	uint16_t num_empty = 0;
	uint16_t num_of_loops = 10;
	while(true) {		
		//exit (0);
		if (curr_queue_index>num_queues-1){
			curr_queue_index = 0;
		}
		//std::cout << "deficit = " << unsigned(deficit[curr_queue_index]) << " curr queue index: "<< int(curr_queue_index) << std::endl;
		////std::cout<<q_class[curr_queue_index]<<std::endl;
		Ptr<const Packet>p = q_class[curr_queue_index]->Peek();		
		if (p==NULL) {
			//std::cout << "Dequeue: packet is NULL" << std::endl;
			num_empty++;
			if (num_empty == num_of_loops) {
				//std::cout << "Dequeue: returning NULL packet" << std::endl;
				return NULL;
			}
			curr_queue_index++;
			continue;
		}
		//std::cout << "packet size: " << p->GetSize() << std::endl;
		if (p->GetSize()<=deficit[curr_queue_index]) {
			//std::cout << "Dequeueing at queue index:" << unsigned(curr_queue_index) << std::endl;
			deficit[curr_queue_index] = deficit[curr_queue_index] - p->GetSize();
			return q_class[curr_queue_index]->Dequeue();
		} else {
			//std::cout << "increasing size at queue index: " << unsigned(curr_queue_index) << " weight: " << q_class[curr_queue_index]->GetWeight() << std::endl;
			//deficit[curr_queue_index]+=uint32_t(q_class[curr_queue_index]->GetWeight());
			deficit[curr_queue_index]+= 300;
			curr_queue_index++;
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
						quantum [i - 1] = std::stoi (line);
					}
					i++;
				}
		}
}

}