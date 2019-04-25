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
>>>>>>> 9f6ab75327517973f0b7551b599d3838358b2a64
	DRR (std::string config);
	DRR();
	~DRR ();

<<<<<<< HEAD
=======
	
	uint32_t Classify (Ptr<Packet> p);
>>>>>>> 9f6ab75327517973f0b7551b599d3838358b2a64
	uint32_t num_queues;
    std::vector<uint32_t> quantum;
	
private:
	std::vector<TrafficClass> q_class;
	std::vector<uint32_t> deficit;
	uint8_t curr_queue_index;
	bool DoEnqueue (Ptr<Packet> p);
    //virtual bool Enqueue(TrafficClass DRRQueue, Ptr<Packet> p);
   	Ptr<Packet> DoDequeue (void);
	Ptr<Packet> DoPeek (void);
<<<<<<< HEAD
    void ConfigReader(std::string config_file_name);
=======
    	void ConfigReader(std::string config_file_name);
>>>>>>> 9f6ab75327517973f0b7551b599d3838358b2a64
    };
}
