
#include "ns3/log.h"
#include "ns3/enum.h"
#include "ns3/uinteger.h"
#include "diff-serv.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE("DiffServ");
NS_OBJECT_ENSURE_REGISTERED (DiffServ);

TypeId
DiffServ::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::DiffServ")
    .SetParent<QueueBase> ()
    .SetGroupName ("Network")
		.AddConstructor<DiffServ> ()
  ;
  return tid;
}

DiffServ::DiffServ ()
{
	NS_LOG_FUNCTION (this);
}

DiffServ::~DiffServ ()
{
	NS_LOG_FUNCTION (this);
}

bool
DiffServ::DoEnqueue (Ptr<Packet> p)
{
	NS_LOG_FUNCTION (this);
	return false;
}

Ptr<Packet>
DiffServ::DoDequeue ()
{
	NS_LOG_FUNCTION (this);
	return NULL;
}

Ptr<Packet>
DiffServ::DoRemove ()
{
	NS_LOG_FUNCTION (this);
	return NULL;
}

Ptr<Packet>
DiffServ::DoPeek ()
{
	NS_LOG_FUNCTION (this);
	return NULL;
}

void
DiffServ::SetMode (QueueMode mode)
{
	NS_LOG_FUNCTION (this);
	m_mode = mode;
}

DiffServ::QueueMode
DiffServ::GetMode ()
{
	NS_LOG_FUNCTION (this);
	return m_mode;
}

Ptr<Packet>
DiffServ::Schedule ()
{
	NS_LOG_FUNCTION (this);
	return NULL;
}

uint32_t
DiffServ::Classify (Ptr<Packet> p)
{
	NS_LOG_FUNCTION (this);
	return 0;
}
} // namespace ns3
