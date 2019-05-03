//#ifndef StrictPriorityQueue_H
#define StrictPriorityQueue_H

#include <queue>
#include <vector>
#include "ns3/packet.h"
#include "ns3/queue.h"
#include "ns3/traffic-class.h"
#include "ns3/ptr.h"
#include "ns3/diff-serv.h"

namespace ns3 {
    class StrictPriorityQueue : public DiffServ{
public:    
    static TypeId GetTypeId (void);	
	StrictPriorityQueue ();
	~StrictPriorityQueue ();
	Ptr<Packet> Schedule ();
	uint32_t number_of_queues;
    std::vector<uint32_t> priority_levels;
	using Queue<Packet>::Dequeue;
	Ptr<Packet> Dequeue (void);
	Ptr<const Packet> Peek (void) const;
	Ptr<Packet> Remove (void);

private:
	std::string configFile;
	bool DoEnqueue (Ptr<Packet> p);
    Ptr<Packet> DoDequeue ();
	Ptr<Packet> DoRemove ();
	Ptr<const ns3::Packet> DoPeek () const;
    void ReadFromConfig(std::string config_file_name);
	void CreateFilters();
    };
}
//#endif
