#ifndef DIFFSERVE_H
#define DIFFSERVE_H

#include <queue>
#include <vector>
#include "ns3/packet.h"
#include "ns3/queue.h"
#include "ns3/traffic-class.h"
#include "ns3/ptr.h"

namespace ns3 {

class DiffServ : public ns3::Queue<ns3::Packet> {
public:
	enum QueueMode { packet, byte };

  static TypeId GetTypeId (void);
	DiffServ ();
	~DiffServ ();

	void AddTrafficClass (TrafficClass t);

	void SetMode (QueueMode mode);
	QueueMode GetMode ();
	virtual Ptr<Packet> Schedule (); //this only calls DoDequeue so....
	uint32_t Classify (Ptr<Packet> p);
	bool Enqueue (Ptr<Packet> p);
	Ptr<Packet> Dequeue (void);
	Ptr<const Packet> Peek (void) const;
	Ptr<Packet> Remove (void);

private:
	std::vector<TrafficClass> q_class;
	QueueMode m_mode;

	//need to overwrite all of these marked virtual
	bool DoEnqueue (Ptr<Packet> p);
	virtual Ptr<Packet> DoDequeue ();
	virtual Ptr<Packet> DoRemove (); // why is this here? it's the same as DoDequeue
	virtual Ptr<Packet> DoPeek (); // same logic as DoDequeue but no removal
};

}
#endif // DIFFSERVE

