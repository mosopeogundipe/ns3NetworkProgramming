#include "ns3/log.h"
#include "ns3/enum.h"
#include "ns3/uinteger.h"
#include "diff-serv.h"
#include "ns3/traffic-class.h"
<<<<<<< HEAD
<<<<<<< HEAD
#include "ns3/drrqueue.h"
=======
#include "drrqueue.h"
>>>>>>> 268ba1f7133403f085b26b14b451162e6b586a7d
=======
#include "drrqueue.h"
>>>>>>> f6dd3561ce82654f8afc374b0eddb2b36501a52a
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
			//deficit.push_back(0);
       		queue.SetWeight(*iter);
		deficit.push_back(queue.GetWeight());
        	queue.SetDefault(false);
        	q_class.push_back(queue);
        	std::advance(iter, 1);
        }
}

DRR::~DRR ()
{
        NS_LOG_FUNCTION (this);
}

<<<<<<< HEAD
<<<<<<< HEAD
DRR::DRR ()
{
        NS_LOG_FUNCTION (this);
}

=======
>>>>>>> 268ba1f7133403f085b26b14b451162e6b586a7d
=======
>>>>>>> f6dd3561ce82654f8afc374b0eddb2b36501a52a
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
<<<<<<< HEAD
DRR::DoEnqueue (Ptr<Packet> p)
=======
DiffServ::DoEnqueue (Ptr<Packet> p)
>>>>>>> 268ba1f7133403f085b26b14b451162e6b586a7d
=======
DRR:DoEnqueue (Ptr<Packet> p)
>>>>>>> f6dd3561ce82654f8afc374b0eddb2b36501a52a
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
>>>>>>> 268ba1f7133403f085b26b14b451162e6b586a7d
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
		Ptr<Packet>p = q_class[curr_queue_index].Peek();
		if (p==NULL) {
			num_empty++;
<<<<<<< HEAD
<<<<<<< HEAD
			if (num_empty == num_queues) {//put as condition for while loop
=======
			if (num_empty == num_queues) {
>>>>>>> f6dd3561ce82654f8afc374b0eddb2b36501a52a
				return NULL;
			}
			curr_queue_index++;
			continue;
		}
<<<<<<< HEAD

		else if (p->GetSize()<deficit[curr_queue_index]) {
=======
			if (num_empty == num_queues) {
				return NULL;
			}
			curr_queue_index++;
			continue;
		}
		if (p->GetSize()<deficit[curr_queue_index]) {
>>>>>>> 268ba1f7133403f085b26b14b451162e6b586a7d
=======
		if (p->GetSize()<=deficit[curr_queue_index]) {
>>>>>>> f6dd3561ce82654f8afc374b0eddb2b36501a52a
			deficit[curr_queue_index] = deficit[curr_queue_index] - p->GetSize();
			//curr_queue_index++;
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
