#ifndef DIFFSERVE_H
#define DIFFSERVE_H

#include <queue>
#include <vector>
#include "ns3/packet.h"
#include "ns3/queue.h"
#include "ns3/traffic-class.h"
#include "ns3/ptr.h"

namespace ns3 {

class DiffServ : public QueueBase
{
public:
	enum QueueMode { packet, byte };

  static TypeId GetTypeId (void);
	DiffServ ();
	~DiffServ ();

	void SetMode (QueueMode mode);
	QueueMode GetMode ();
	Ptr<Packet> Schedule (); //this only calls DoDequeue so....
	uint32_t Classify (Ptr<Packet> p);

private:
	std::vector<TrafficClass> q_class;
	QueueMode m_mode;

	//need to overwrite all of these marked virtual
	bool DoEnqueue (Ptr<Packet> p);
	virtual Ptr<Packet> DoDequeue ();
	Ptr<Packet> DoRemove (); // why is this here? it's the same as DoDequeue
	virtual Ptr<Packet> DoPeek ();
};

}
#endif // DIFFSERVE

