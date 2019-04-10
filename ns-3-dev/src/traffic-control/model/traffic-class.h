
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
	std::vector<Filter> filters;

	void Unref (); //required for pointers

	double_t GetWeight ();
	uint32_t GetPriorityLevel ();
	bool IsDefault ();

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
