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
  //LogComponentEnable ("ControlExample", LOG_LEVEL_FUNCTION);

  uint16_t port = 9;  // well-known echo port number
  uint32_t packetSize = 32; // this will be set by the app
  uint32_t maxPacketCount = 1; // this will be set by the app
	uint8_t compLinkSpeed = 8;
	uint8_t regLinkSpeed = 8;
	bool enableCompression = false;
	bool enableGlobalCompression = false;
	std::string fill = "hello there"; // will be set by app, 
																			//have here for testing

  Time interPacketInterval = Seconds (1.);
  CommandLine cmd;
  cmd.AddValue ("port", "Port being used to commuincate", port);
  cmd.AddValue ("compLinkSpeed", "Speed (Mbps) of the compression link.", compLinkSpeed);
  cmd.AddValue ("regLinkSpeed", "Speed (Mbps) of the regular links.", regLinkSpeed);
  cmd.AddValue ("enableCompression", "Compression enabled on the center link, between nodes 2 and 3.", enableCompression);
  cmd.AddValue ("enableGlobalCompression", "Compression enabled on all links.", enableGlobalCompression);
  cmd.AddValue ("fill", "String to fill packets with.", fill);
  cmd.Parse (argc, argv);

	NodeContainer c;
	c.Create (4);

  // Point-to-point links
  NodeContainer c01 = NodeContainer (c.Get (0), c.Get (1));
  NodeContainer c12 = NodeContainer (c.Get (1), c.Get (2));
  NodeContainer c23 = NodeContainer (c.Get (2), c.Get (3));

  // We create the channels first without any IP addressing information
  PointToPointHelper p2p;
  p2p.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  p2p.SetChannelAttribute ("Delay", StringValue ("0ms"));

	NetDeviceContainer d01 = p2p.Install (c01);
  NetDeviceContainer d12 = p2p.Install (c12);
  NetDeviceContainer d23 = p2p.Install (c23);

	// tell node 1 it needs to encrypt, node 2 will decrypt automatically
	//((PointToPointNetDevice) d12.Get (0))->SetCompress (true);

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

  P2PServerHelper server (port);
	ApplicationContainer apps = server.Install (c.Get (3));
  apps.Start (Seconds (1.0));
  apps.Stop (Seconds (10.0));



  P2PClientHelper client (i23.GetAddress (1), port);
  client.SetAttribute ("MaxPackets", UintegerValue (maxPacketCount));
  client.SetAttribute ("Interval", TimeValue (interPacketInterval));
  client.SetAttribute ("PacketSize", UintegerValue (packetSize));
  apps = client.Install (c.Get (0));
  apps.Start (Seconds (2.0));
  apps.Stop (Seconds (10.0));
	client.SetFill (apps.Get (0), fill);

  // Create router nodes, initialize routing database and set up the routing
  // tables in the nodes.
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  AsciiTraceHelper ascii;
  p2p.EnableAsciiAll (ascii.CreateFileStream ("p2p.tr"));
  p2p.EnablePcapAll ("control");

  Simulator::Run ();
  Simulator::Destroy ();
}

