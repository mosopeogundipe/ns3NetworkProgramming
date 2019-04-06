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
#include "ns3/filter.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("ControlTest");

int 
main (int argc, char *argv[])
{
	Packet p;
	TcpHeader tHead;
	Ipv4Header iHead;

	Ipv4Address addr = Ipv4Address ("111.111.001.113");
	//Ipv4Address addr2 = Ipv4Address ("112.112.112.112");

	Ipv4Address start1 = Ipv4Address ("000.000.000.000");
	Ipv4Address end1 = Ipv4Address ("255.255.255.255");

	Ipv4Address start2 = Ipv4Address ("111.111.000.111");
	Ipv4Address end2 = Ipv4Address ("200.194.234.111");

	Ipv4Address start3 = Ipv4Address ("111.111.000.255");
	Ipv4Address end3 = Ipv4Address ("199.122.255.255");

	SourceMaskFilterElement f1 = SourceMaskFilterElement (start1, end1);
	SourceMaskFilterElement f2 = SourceMaskFilterElement (start2, end2);
	SourceMaskFilterElement f3 = SourceMaskFilterElement (start3, end3);

	Ptr<SourceMaskFilterElement> fe1 (&f1);
	Ptr<SourceMaskFilterElement> fe2 (&f2);
	Ptr<SourceMaskFilterElement> fe3 (&f3);

	Filter filter;

	filter.AddFilter (fe1);
	filter.AddFilter (fe2);
	filter.AddFilter (fe3);

	tHead.SetDestinationPort (11);
	iHead.SetSource (addr);
	p.AddHeader (iHead);
	//p.AddHeader (tHead);

	ProtocolFilterElement f = ProtocolFilterElement (12);


	Ptr<Packet> pp = Ptr<Packet> (&p);

	std::cout << "f match " << filter.Match (pp) << std::endl;
}

