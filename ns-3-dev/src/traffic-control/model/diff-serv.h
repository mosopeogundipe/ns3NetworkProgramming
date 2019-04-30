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


	void SetMode (QueueMode mode);
	QueueMode GetMode ();
	uint32_t Classify (Ptr<Packet> p);
	bool Enqueue (Ptr<Packet> p);

	virtual Ptr<Packet> Schedule (void); //this only calls DoDequeue so....
	Ptr<Packet> Dequeue (void);
	virtual Ptr<const Packet> Peek (void) const;
	virtual Ptr<Packet> Remove (void);

	using Queue<Packet>::Dequeue;
private:
	QueueMode m_mode;

	//need to overwrite all of these marked virtual
	bool IsEnqueuingSuccessful(TrafficClass* queue, Ptr<Packet> p);
	bool DoEnqueue (Ptr<Packet> p);
	virtual Ptr<Packet> DoDequeue (void);
	virtual Ptr<Packet> DoRemove (void); // why is this here? it's the same as DoDequeue
	virtual Ptr<const Packet> DoPeek (void) const; // same logic as DoDequeue but no removal
protected:
	std::vector<TrafficClass*> q_class;
};

}
#endif // DIFFSERVE

