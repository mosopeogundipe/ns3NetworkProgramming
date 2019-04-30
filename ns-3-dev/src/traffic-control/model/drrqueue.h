/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

//#ifndef DRR_H
#define DRR_H

#include <queue>
#include <vector>
#include "ns3/packet.h"
#include "ns3/queue.h"
#include "ns3/traffic-class.h"
#include "ns3/ptr.h"
#include "ns3/diff-serv.h"

namespace ns3 {

  class DRR : public DiffServ{
public:
<<<<<<< HEAD
    static TypeId GetTypeId (void);	
=======

  static TypeId GetTypeId (void);	
>>>>>>> origin/sope-p2
	DRR (std::string config);
	DRR();
	~DRR ();

	uint32_t num_queues;
  std::vector<uint32_t> quantum;
	Ptr<Packet> Dequeue (void);
	Ptr<const Packet> Peek (void) const;
	Ptr<Packet> Remove (void);

private:
	std::vector<TrafficClass> q_class;
	std::vector<uint32_t> deficit;
	std::string configFile;
	uint8_t curr_queue_index;
	bool DoEnqueue (Ptr<Packet> p);
    //virtual bool Enqueue(TrafficClass DRRQueue, Ptr<Packet> p);
<<<<<<< HEAD
   	Ptr<Packet> DoDequeue (void);
	Ptr<Packet> DoPeek (void);
    void ConfigReader(std::string config_file_name);
    };
=======
	Ptr<Packet> DoDequeue (void);
	Ptr<Packet> DoRemove (void);
	Ptr<const Packet> DoPeek (void) const;
	void ConfigReader(std::string config_file_name);

};
>>>>>>> origin/sope-p2
}

//#endif

