#include "ns3/log.h"
#include "ns3/enum.h"
#include "ns3/uinteger.h"
#include "diff-serv.h"
#include "ns3/traffic-class.h"
#include "ns3/drrqueue.h"
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
//	NS_LOG_FUNCTION (this);
}

DRR::DRR (std::string configFile)
{
//    NS_LOG_FUNCTION (this);	
	num_queues = 0;	
	ConfigReader (configFile);
	configFile = "drr-config.txt";
	//num_queues = 3;
	//quantum.push_back(30);
	//quantum.push_back(20);
	//quantum.push_back(10);
	FilterElement* fe;
	Filter* filter;
	std::vector<uint32_t>::iterator iter = quantum.begin();
    for (int i=0; i<(int)num_queues; i++){
	    TrafficClass queue;
		//deficit.push_back(0);
       	queue.SetWeight(*iter);
		deficit.push_back(queue.GetWeight());
        queue.SetDefault(false);
        q_class.push_back(queue);
        std::advance(iter, 1);
		//destination port hard code
		switch(i) {
			case 0:
			fe = (FilterElement*) new DestPortFilterElement(9999);
			filter = new Filter();
			filter->AddFilter(fe);
			queue.filters.push_back(*filter);		
			case 1:
			fe = (FilterElement*) new DestPortFilterElement(5555);
			filter = new Filter();
			filter->AddFilter(fe);
			queue.filters.push_back(*filter);	
			case 2:
			fe = (FilterElement*) new DestPortFilterElement(1111);
			filter = new Filter();
			filter->AddFilter(fe);
			queue.filters.push_back(*filter);	
		}
    }
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
	exit (0);
	return DoDequeue ();
}

Ptr<const Packet>
DRR::Peek (void) const
{
	return DoPeek ();
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
DRR::DoDequeue() {
	uint16_t num_empty = 0;
	while(true) {
		std::cout << "Num queues = " << num_queues << " , deficit = " << deficit[curr_queue_index] << std::endl;
		exit (0);
		Ptr<const Packet>p = q_class[curr_queue_index].Peek();
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
			return q_class[curr_queue_index].Dequeue();
		} else {
			deficit[curr_queue_index]+=q_class[curr_queue_index].GetWeight();
			curr_queue_index++;
		}
	}
}

//the number of queues and the corresponding quantum value for each queue
void
DRR::ConfigReader(std::string configFile) {
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
