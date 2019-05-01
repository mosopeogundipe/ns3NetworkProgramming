
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
	std::vector<Filter*> filters;

	void Unref (); //required for pointers

  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);

	TrafficClass ();
	~TrafficClass ();

	void SetMaxBytes (uint32_t b);
	void SetMaxPackets (uint32_t p);
	void SetPriorityLevel (uint32_t p);
	void SetWeight (double_t w);
	void SetDefault (bool d);

	uint32_t GetMaxBytes (void);
	uint32_t GetBytes (void);
	uint32_t GetMaxPackets (void);
	uint32_t GetPackets (void);
	uint32_t GetPriorityLevel (void);
	double_t GetWeight (void);
	bool IsDefault (void);

	bool Enqueue (Ptr<ns3::Packet> p);
	Ptr<ns3::Packet> Dequeue (void);
	Ptr<const ns3::Packet> Peek (void) const;
	bool Match (ns3::Ptr<ns3::Packet> p);
	std::queue<Ptr<ns3::Packet>> m_queue; //TODO: make private and have getters and setters

private:
	uint32_t bytes;
	uint32_t packets;
	uint32_t maxBytes;
	uint32_t maxPackets;
	uint32_t priority_level;
	double_t weight;
	bool isDefault;
	//std::queue<Ptr<ns3::Packet>> m_queue;
};

} // namespace ns3

#endif /* TRAFFIC_CLASS */
