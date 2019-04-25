#include "ns3/log.h"
#include "ns3/enum.h"
#include "ns3/uinteger.h"
#include "diff-serv.h"
#include "ns3/traffic-class.h"
#include "drrqueue.h"
#include <bits/stdc++.h>

#define NOCLASSIFY 0xffffffff

namespace ns3 {
   NS_LOG_COMPONENT_DEFINE("DRR");
   NS_OBJECT_ENSURE_REGISTERED (DRR);

//#include 
//ns3/traffic-class.h

DRR::DRR (std::string configFile)
{
       	NS_LOG_FUNCTION (this);	
	num_queues = 0;	   
	ConfigReader (configFile);

	std::vector<uint32_t>::iterator iter = quantum.begin();
        for (int i=0; i<(int)num_queues; i++){
	        TrafficClass queue;
<<<<<<< HEAD
			//deficit.push_back(0);
       		queue.SetWeight(*iter);
		deficit.push_back(queue.GetWeight());
=======
			deficit.push_back(0);
       		queue.SetWeight(*iter);
>>>>>>> 9f6ab75327517973f0b7551b599d3838358b2a64
        	queue.SetDefault(false);
        	q_class.push_back(queue);
        	std::advance(iter, 1);
        }
}

DRR::~DRR ()
{
        NS_LOG_FUNCTION (this);
}

TypeId
DRR::GetTypeId (void)
{
	static TypeId tid = TypeId ("ns3::DRR")
        .SetParent<Object> ()
        .SetGroupName ("TrafficControl")
	.AddConstructor<DRR> ()
	;
	return tid;
}


bool
<<<<<<< HEAD
DRR:DoEnqueue (Ptr<Packet> p)
=======
DiffServ::DoEnqueue (Ptr<Packet> p)
>>>>>>> 9f6ab75327517973f0b7551b599d3838358b2a64
{
	NS_LOG_FUNCTION (this);
 	uint32_t queuePos = Classify (p); //what logic should be in classify?
	if (q_class.empty()){
		return false;
	}
	TrafficClass drrQueue = q_class[queuePos];

	return drrQueue.Enqueue(p);
}


Ptr<Packet>
<<<<<<< HEAD
DRR::DoPeek ()
=======
DiffServ::DoPeek ()
>>>>>>> 9f6ab75327517973f0b7551b599d3838358b2a64
{
	NS_LOG_FUNCTION (this);
	Ptr<Packet> packet;

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

// bool
// DRR::Enqueue(TrafficClass drrQueue, Ptr<Packet> p){
// 	//don't have a normal queue logic here, I put packets by order in rounds
// 	int count = 0;
// 	while (true)
//                 for (int i=0; i<numQueues; i++){
// 			if (!q_class.empty()){
// 				q_class[i].Enqueue(p);
// 			}
// 			else if (q_class[i].size()<q_class[i+1].size()){
// 				q_class[i].enqueue(p);
// 			} else if (q_class[i].size()==q_class[i+1].size()) {
// 				count++;
// 			}
// 		}
// 	//all queues have the same size
// 	if (count == numQueues) && (!q_class[0].empty()) {
// 		q_class[0].enqueue(p);
// 	}
// }

Ptr<Packet>
DRR::DoDequeue() {
	uint16_t num_empty = 0;
	while(true) {
<<<<<<< HEAD
=======
		deficit[curr_queue_index]+=q_class[curr_queue_index].GetWeight();
>>>>>>> 9f6ab75327517973f0b7551b599d3838358b2a64
		Ptr<Packet>p = q_class[curr_queue_index].Peek();
		if (p==NULL) {
			num_empty++;
			if (num_empty == num_queues) {
				return NULL;
			}
			curr_queue_index++;
			continue;
		}
<<<<<<< HEAD
		if (p->GetSize()<=deficit[curr_queue_index]) {
			deficit[curr_queue_index] = deficit[curr_queue_index] - p->GetSize();
			//curr_queue_index++;
			return q_class[curr_queue_index].Dequeue();
		} else {
			deficit[curr_queue_index]+=q_class[curr_queue_index].GetWeight();
=======
		if (p->GetSize()<deficit[curr_queue_index]) {
			deficit[curr_queue_index] = deficit[curr_queue_index] - p->GetSize();
			curr_queue_index++;
			return q_class[curr_queue_index].Dequeue();
		} else {
>>>>>>> 9f6ab75327517973f0b7551b599d3838358b2a64
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
						//quantum = new uint16_t [std::stoi (line)];
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
