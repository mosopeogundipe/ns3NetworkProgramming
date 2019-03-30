
#ifndef TRAFFIC_CLASS_H
#define TRAFFIC_CLASS_H

#include "ns3/object.h"
#include <stdio.h>
#include <math.h>
#include "ns3/packet.h"
#include "ns3/ptr.h"
#include <queue>
#include <vector>
#include "ns3/filter.h"

namespace ns3 {

class TrafficClass:public Object {
public:
	std::vector<Ptr<Filter>> filters;

	void SetBytes (uint32_t b);
	void SetPackets (uint32_t p);
	void SetMaxBytes (uint32_t b);
	void SetMaxPackets (uint32_t p);
	void SetWeight (double_t w);
	void SetPriorityLevel (uint32_t p);
	void SetIsDefault (bool b);
	void SetMQueue (std::queue<Ptr<ns3::Packet>> q);
	void Unref ();

  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);

	TrafficClass ();
	~TrafficClass ();

	bool Enqueue (Ptr<ns3::Packet> p);
	Ptr<ns3::Packet> Dequeue ();
	bool Match (Ptr<ns3::Packet> p);

private:
	uint32_t bytes;
	uint32_t packets;
	uint32_t maxBytes;
	uint32_t maxPackets;
	uint32_t priority_level;
	double_t weight;
	bool isDefault;
	std::queue<Ptr<ns3::Packet>> m_queue;
};

} // namespace ns3

#endif /* TRAFFIC_CLASS */
