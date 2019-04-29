#include <iostream>
#include <fstream>
#include <string>
#include <cassert>

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/csma-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-static-routing-helper.h"
#include "ns3/packet.h"
#include "ns3/tcp-header.h"
#include "ns3/ipv4-header.h"
#include "ns3/ipv4-address.h"
#include "ns3/source-addr-filter-element.h"
#include "ns3/source-port-filter-element.h"
#include "ns3/source-mask-filter-element.h"
#include "ns3/dest-addr-filter-element.h"
#include "ns3/dest-port-filter-element.h"
#include "ns3/dest-mask-filter-element.h"
#include "ns3/protocol-filter-element.h"
#include "ns3/traffic-class.h"
#include "ns3/filter.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("ControlTest");

int 
main (int argc, char *argv[])
{
	std::cout << "Hello, world!" << std::endl;

	Ptr<TrafficClass> tc = CreateObject<TrafficClass> ();

	std::cout << tc->GetMaxBytes () << std::endl;

	tc->SetMaxBytes (32);

	std::cout << tc->GetMaxBytes () << std::endl;

	return 0;
}

