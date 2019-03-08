/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

 /*
 	Authors:
 		Ian Granger
 		Sope

	Purpose:
		Then end goal for this file is twofold. First, to create an 
		application that will take a give amount of data, and convert it into 
		PPP's. And second, to create a pipeline that will beable to correctly 
		encode and decode this data.

	Date:
		2/25/19

	Todo:
		Ian:
			* - create an application and an application helper as a child of 
			Application. Look at UdpEchoClientApplication for an example
	
*/

/* 
	Tutorial found at: https://www.nsnam.org/docs/release/3.12/tutorial/singlehtml/index.html
*/


#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"

using namespace ns3

NS_LOG_COMPONENT_DEFINE ("CompressionNode");	

int main(int argc, char *argv[]) {

	//creates our two devices
	NodeContainer nodes;
	nodes.Create(2);


	PointToPointHelper pointToPoint;
	pointToPoint.SetDeviceAttribute("DataRate", StringValue ("5Mbps"));
	pointToPoint.SetChannelAttribute("Delay", StringValue ("2ms"));

	NetDeviceContainer devices;
	devices = pointToPoint.Install(nodes);

	InternetStackHelper stack;
	stack.Install(nodes);

	Ipv4AddressHelper address;
	address.SetBase("10.1.1.0", "255.255.255.0");
	Ipv4InterfaceContainer interfaces = address.Assign(devices);


	// part we need to change to our own application / helper
	CompressionDetectionServerHelper compressionServer(9);

	ApplicationContainer serverApps = compressionServer.Install(nodes.Get(1));
	serverApps.Start(Seconds(1.0));
	serverApps.Stop(Seconds(10.0));

	CompressionDetectionClientHelper compressionClient (interfaces.GetAddress(1), 9);
	// echoClient.SetAttribute("MaxPackets", UintegerValue(1));
	// echoClient.SetAttribute("Interval", TimeValue(Seconds(1.)));
	// echoClient.SetAttribute("PacketSize", UintegerValue(1024));

	ApplicationContainer clientApps = compressionClient.Install(nodes.Get(0));
	clientApps.Start(Seconds (2.0));
	clientApps.Stop(Seconds (10.0));

	Simulator::Run();

	Simulator::Destroy();
	return 0;

}