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

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("ControlTest");

int 
main (int argc, char *argv[])
{
  //LogComponentEnable ("P2PServerApplication", LOG_LEVEL_ALL);
  //LogComponentEnable ("P2PClientApplication", LOG_LEVEL_ALL);
  LogComponentEnable ("PointToPointNetDevice", LOG_LEVEL_ALL);
  LogComponentEnable ("CompressionDetectionClient", LOG_LEVEL_INFO);
  LogComponentEnable ("CompressionDetectionServer", LOG_LEVEL_INFO);
  LogComponentEnable ("ControlTest", LOG_LEVEL_ERROR);
  //LogComponentEnable ("PointToPointNetDevice", LOG_LEVEL_ERROR);
  //LogComponentEnable ("PointToPointHelper", LOG_LEVEL_ERROR);

  uint16_t port = 9;  // well-known echo port number
  //uint32_t packetSize = 32; // this will be set by the app
  uint32_t maxPacketCount = 12000; // this will be set by the app
	//uint8_t midLinkSpeed = 1;
	//uint8_t outerLinkSpeed = 8;
  std::string midLinkSpeed = "1";
	std::string outerLinkSpeed = "8";
	bool enableCompression = false;
	bool enableGlobalCompression = false;
	//std::string fill = "praise jesus"; // will be set by app, 
																			//have here for testing

  //Time interPacketInterval = Seconds (1.); //this will be set by the app
  CommandLine cmd;
	cmd.AddValue ("numPackets", "Total number of packets to send", maxPacketCount);
  cmd.AddValue ("port", "Port being used to commuincate", port);
  cmd.AddValue ("midLinkSpeed", "Speed (Mbps) of the link between nodes 1 and 2. (middle link)", midLinkSpeed);
  cmd.AddValue ("outerLinkSpeed", "Speed (Mbps) of the links between nodes 0 and 1, and 2 and 3.", outerLinkSpeed);
  cmd.AddValue ("enableCompression", "Compression enabled on the center link, between nodes 2 and 3.", enableCompression);
  cmd.AddValue ("enableGlobalCompression", "Compression enabled on all links.", enableGlobalCompression);
  //cmd.AddValue ("fill", "String to fill packets with.", fill);
  cmd.Parse (argc, argv);

	NodeContainer c;
	c.Create (4);

  // Point-to-point links
  NodeContainer c01 = NodeContainer (c.Get (0), c.Get (1));
  NodeContainer c12 = NodeContainer (c.Get (1), c.Get (2));
  NodeContainer c23 = NodeContainer (c.Get (2), c.Get (3));

  // We create the channels first without any IP addressing information
  PointToPointHelper p2p;
	std::string str;

	//setting compression link speed
	//str = std::to_string (midLinkSpeed) + "Mbps";
  str = midLinkSpeed + "Mbps";
  NS_LOG_ERROR("mid link speed: " << str);
  p2p.SetDeviceAttribute ("DataRate", StringValue(str));
  p2p.SetChannelAttribute ("Delay", StringValue ("0ms"));
  NS_LOG_ERROR("enable compression:" << enableCompression);
	p2p.SetCompress (enableCompression);
  NetDeviceContainer d12 = p2p.Install (c12);

	//setting the regular link speeds
	p2p.SetCompress (enableGlobalCompression);
	//str = std::to_string (outerLinkSpeed) + "Mbps";

  p2p.SetDeviceAttribute ("DataRate", StringValue ("8Mbps"));
  p2p.SetChannelAttribute ("Delay", StringValue ("0ms"));
	NetDeviceContainer d01 = p2p.Install (c01);

  p2p.SetDeviceAttribute ("DataRate", StringValue ("8Mbps"));
  p2p.SetChannelAttribute ("Delay", StringValue ("0ms"));
	NetDeviceContainer d23 = p2p.Install (c23);

  InternetStackHelper internet;
  internet.Install (c);

  // Later, we add IP addresses.  The middle two octets correspond to 
  // the channel number.
  NS_LOG_INFO ("Assign IP Addresses.");
  Ipv4AddressHelper ipv4;

  ipv4.SetBase ("10.0.1.0", "255.255.255.0");
  Ipv4InterfaceContainer i01 = ipv4.Assign (d01);

  ipv4.SetBase ("10.0.2.0", "255.255.255.0");
  Ipv4InterfaceContainer i12 = ipv4.Assign (d12);

  ipv4.SetBase ("10.0.3.0", "255.255.255.0");
  Ipv4InterfaceContainer i23 = ipv4.Assign (d23);

  CompressionDetectionServerHelper server (port);
	ApplicationContainer apps = server.Install(c.Get (3));
  apps.Start(Seconds (1.0));
  apps.Stop(Seconds (500.0));

  CompressionDetectionClientHelper client ( i23.GetAddress(1), port);
  client.SetAttribute ("MaxPackets", UintegerValue (maxPacketCount));
  //client.SetAttribute ("Interval", TimeValue (interPacketInterval));
  //client.SetAttribute ("PacketSize", UintegerValue (packetSize));
	apps = client.Install (c.Get (0));
  apps.Start (Seconds (2.0));
  apps.Stop (Seconds (500.0));


	//client.SetFill (apps.Get (0), fill);

  // Create router nodes, initialize routing database and set up the routing
  // tables in the nodes.
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  AsciiTraceHelper ascii;
  p2p.EnableAsciiAll (ascii.CreateFileStream ("finalRunner.tr"));
  p2p.EnablePcapAll ("finalRunner");

  Simulator::Run ();
  Simulator::Destroy ();
}

