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
	//enum QueueMode { packet, byte };
    
    static TypeId GetTypeId (void);	
	StrictPriorityQueue ();
	//StrictPriorityQueue (std::string config_file_name);
	~StrictPriorityQueue ();

	//void SetMode (QueueMode mode);
	//QueueMode GetMode ();
	Ptr<Packet> Schedule (); //this only calls DoDequeue so....
	//uint32_t Classify (Ptr<Packet> p);
	uint32_t number_of_queues;
    std::vector<uint32_t> priority_levels;
	
private:
	std::vector<TrafficClass> q_class;
	std::string configFile;
	std::vector<Filter> filters;
	//QueueMode m_mode;

	//need to overwrite all of these marked virtual
	bool DoEnqueue (Ptr<Packet> p);
    bool IsEnqueuingSuccessful(TrafficClass queue, Ptr<Packet> p);
    Ptr<Packet> DoDequeue ();
	Ptr<Packet> DoRemove (); // why is this here? it's the same as DoDequeue
	Ptr<Packet> DoPeek (); // same logic as DoDequeue but no removal
    void ReadFromConfig(std::string config_file_name);
	void CreateFilters();
    };
}