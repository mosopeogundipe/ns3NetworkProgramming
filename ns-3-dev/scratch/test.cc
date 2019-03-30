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

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("ControlTest");

int 
main (int argc, char *argv[])
{
	Packet p;
	TcpHeader tHead;
	Ipv4Header iHead;

	Ipv4Address addr = Ipv4Address ("111.111.111.111");
	//Ipv4Address addr2 = Ipv4Address ("112.112.112.112");

	//Ipv4Address start = Ipv4Address ("111.111.111.000");
	//Ipv4Address end = Ipv4Address ("111.111.111.255");

	uint8_t port = 12;

	tHead.SetDestinationPort (11);
	iHead.SetDestination (addr);
	iHead.SetProtocol (port);
	p.AddHeader (iHead);
	//p.AddHeader (tHead);

	ProtocolFilterElement f = ProtocolFilterElement (12);


	Ptr<Packet> pp = Ptr<Packet> (&p);

	std::cout << "f match " << f.Match (pp) << std::endl;
}

