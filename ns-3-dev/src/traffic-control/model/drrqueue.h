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

  static TypeId GetTypeId (void);	
	DRR();
	~DRR ();
	uint32_t num_queues;
  std::vector<uint32_t> quantum;
	Ptr<Packet> Dequeue (void);
	Ptr<const Packet> Peek (void) const;
	Ptr<Packet> Remove (void);
	Ptr<Packet> Schedule ();

private:
	std::vector<uint32_t> deficit;
	std::string configFile;
	uint32_t curr_queue_index;
	bool DoEnqueue (Ptr<Packet> p);
	Ptr<Packet> DoDequeueNewQueue (int index);
	Ptr<Packet> DoDequeue (void);
	Ptr<Packet> DoRemove (void);
	Ptr<const ns3::Packet> DoPeek () const;
	void ReadFromConfig(std::string config_file_name);
	void CreateFilters();
};
}

//#endif