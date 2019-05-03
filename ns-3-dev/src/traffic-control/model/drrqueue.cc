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
	configFile = "drr-config.txt";
	ReadFromConfig (configFile);
	curr_queue_index = 0;
//	CreateFilters();
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
    highqueue->SetDefault(false);
	highqueue->SetWeight(quantum[0]);
	q_class.push_back(highqueue);

	FilterElement* destPortMiddle = new DestPortFilterElement(5555);

	Filter* middlePriority = new Filter();
	middlePriority-> AddFilter(destPortMiddle); 
	middlequeue->filters.push_back(middlePriority);
	middlequeue->SetDefault(false);
	middlequeue->SetWeight(quantum[1]);
	q_class.push_back(middlequeue);
	
	FilterElement* destPortLow = new DestPortFilterElement(1111);

	Filter* lowPriority = new Filter();
	lowPriority-> AddFilter(destPortLow); 
	lowqueue->filters.push_back(lowPriority);
    lowqueue->SetDefault(false);
	lowqueue->SetWeight(quantum[2]);
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

bool
DRR::DoEnqueue (Ptr<Packet> p)
{
	uint32_t queuePos = Classify (p);
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
	Ptr<const Packet> packet;
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
	if (curr_queue_index>num_queues-1){
		curr_queue_index = 0;
	}
	return DoDequeueNewQueue(curr_queue_index);
}

Ptr<Packet>
DRR::DoDequeueNewQueue(int index) {
	//std::cout<<"==========================================="<<std::endl;
	Ptr<const Packet>p = q_class[index]->Peek();
	int size = 0;
	int num_empty = 0;
	//std::cout<<"Deficit in the beginning = "<<deficit[index]<<std::endl;
	//std::cout<<"Queue index = "<<index<<std::endl;
	if (p==NULL) {
			num_empty++;
			if ((unsigned)num_empty == num_queues) {
				return NULL;
			}
			curr_queue_index++;
			return DoDequeueNewQueue(index++);
	} else if (p->GetSize() <= deficit[index]){
		size = p->GetSize();
		deficit[index] = deficit[index]-size;
		//std::cout<<"Should send the packet with size = "<<size<<std::endl;
		//std::cout<<"Deficit now is = "<<deficit[index]<<std::endl;
		//std::cout<<"==========================================="<<std::endl;
		return q_class[index]->Dequeue();;
	} else {
		deficit[index] = deficit[index] + q_class[curr_queue_index]->GetWeight();
		//std::cout<<"Too big packet with size = "<<size<<std::endl;
		//std::cout<<"Deficit now is = "<<deficit[index]<<std::endl;
		//std::cout<<"==========================================="<<std::endl;
		curr_queue_index = index++;
		return DoDequeueNewQueue(index++);
	}
}

Ptr<Packet>
DRR::Schedule ()
{
	uint16_t num_empty = 0;
	uint16_t num_of_loops = 10;
	while(true) {		
		if (curr_queue_index>num_queues-1){
			curr_queue_index = 0;
		}
		Ptr<const Packet>p = q_class[curr_queue_index]->Peek();		
		if (p==NULL) {
			num_empty++;
			if (num_empty == num_of_loops) {
				return NULL;
			}
			curr_queue_index++;
			continue;
		}
		if (p->GetSize()<=deficit[curr_queue_index]) {
			deficit[curr_queue_index] = deficit[curr_queue_index] - p->GetSize();
			return q_class[curr_queue_index]->Dequeue();
		} else {
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
						num_queues = std::stoi (line);;
					}
					else
					{
						quantum.push_back(std::stoi (line));
						deficit.push_back(std::stoi (line));

						TrafficClass* tc = new TrafficClass ();
						tc->SetWeight (stoi (line));
						tc->SetDefault (false);

						getline (readFile,line);
						Filter* filt = new Filter();
						FilterElement* fe = new DestPortFilterElement (stoi (line));
						filt->AddFilter (fe);
						tc->filters.push_back (filt);
						q_class.push_back (tc);
					}
					i++;
				}
		}
}

}