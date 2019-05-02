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
	quantum.push_back(30);
	quantum.push_back(20);
	quantum.push_back(10);
}

void
DRR::CreateFilters(){
	TrafficClass* highqueue = new TrafficClass();
	TrafficClass* middlequeue = new TrafficClass();
	TrafficClass* lowqueue = new TrafficClass();

	FilterElement* destPortHigh = (FilterElement*) new DestPortFilterElement(9999);

	Filter* highPriority = new Filter();
	highPriority -> AddFilter(destPortHigh); 
	highqueue->filters.push_back(highPriority);
	highqueue->SetPriorityLevel(2);
	//highpriorityqueue -> SetMaxPackets(1000);
    highqueue->SetDefault(false);
	q_class.push_back(highqueue);

	FilterElement* destPortMiddle = (FilterElement*) new DestPortFilterElement(5555);

	Filter* middlePriority = new Filter();
	middlePriority-> AddFilter(destPortMiddle); 
	middlequeue->filters.push_back(middlePriority);
	//defaultqueue -> SetMaxPackets(1000);
	middlequeue->SetDefault(true);
	q_class.push_back(middlequeue);
	
	FilterElement* destPortLow = (FilterElement*) new DestPortFilterElement(1111);

	Filter* lowPriority = new Filter();
	lowPriority-> AddFilter(destPortLow); 
	lowqueue->filters.push_back(lowPriority);
	lowqueue->SetPriorityLevel(1);
	//lowpriorityqueue -> SetMaxPackets(1000);
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
        .SetParent<DiffServ> ()
        .SetGroupName ("TrafficControl")
	.AddConstructor<DRR> ()
	;
	return tid;
}

<<<<<<< HEAD

bool
DRR::DoEnqueue (Ptr<Packet> p)
=======
Ptr<Packet>
DRR::Dequeue (void)
>>>>>>> origin/sope-p2
{
	std::cout << "drr dequeue" << std::endl;
	//exit (0);
	return DoDequeue ();
}

Ptr<const Packet>
DRR::Peek (void) const
{
	std::cout << "drr peek" << std::endl;
	return DoPeek();
}

Ptr<Packet>
DRR::Remove (void)
{
	return DRR::DoRemove ();
}

Ptr<Packet>
<<<<<<< HEAD
DRR::DoPeek ()
=======
DRR::DoRemove (void)
>>>>>>> origin/sope-p2
{
	return DoDequeue ();
}

Ptr<const Packet>
DRR::DoPeek (void) const
{
	//same logic as DoDequeue () but we don't remove the packet
	std::cout<<"DoPeek1"<< std::endl;
	Ptr<const Packet> packet;

	// // Will use the first TrafficClass by default
	// for (std::vector<TrafficClass>::const_iterator it = q_class.begin(); it != q_class.end (); ++it)
	// 	{
	// 		packet = it->Peek ();
	// 		if (packet != NULL)
	// 			{
	// 				return packet;
	// 			}
	// 	}
	// return NULL;

	// Will use the first TrafficClass by default
	for (TrafficClass* tc : q_class) 
		{
			packet = tc->Peek ();
			std::cout<<"DoPeek2"<< std::endl;
			if (packet != NULL)
				{
					return packet;
				}
		}
	return NULL;
}

Ptr<Packet>
DRR::DoDequeue() {
	std::cout<<"DoDequeueeeeeeeeee"<< std::endl;
	uint16_t num_empty = 0;
	std::cout<<"DoDequeueeeeeeeeee2"<< std::endl;
	while(true) {
		std::cout<<"DoDequeueeeeeeeeee3"<< std::endl;
		//std::cout << "Num queues = " << num_queues << " , deficit = " << deficit[curr_queue_index] << std::endl;
		//exit (0);
		if (curr_queue_index>num_queues){
			curr_queue_index = 0;
		}
		std::cout<<q_class[curr_queue_index]<<std::endl;
		Ptr<const Packet>p = q_class[curr_queue_index]->Peek();
		
		if (p==NULL) {
			num_empty++;
			if (num_empty == num_queues) {
				return NULL;
			}
			curr_queue_index++;
			continue;
		}
		if (p->GetSize()<=deficit[curr_queue_index]) {
			deficit[curr_queue_index] = deficit[curr_queue_index] - p->GetSize();
			return q_class[curr_queue_index]->Dequeue();
		} else {
			deficit[curr_queue_index]+=q_class[curr_queue_index]->GetWeight();
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