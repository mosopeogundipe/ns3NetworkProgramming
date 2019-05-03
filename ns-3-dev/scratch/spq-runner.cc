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
#include "ns3/strict-priority-queue.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("ControlTest");

int 
main (int argc, char *argv[])
{
  Time::SetResolution(Time::NS);

  // setup logs
  LogComponentEnable ("ControlTest", LOG_LEVEL_ALL);
  LogComponentEnable ("SpqApplicationClient", LOG_LEVEL_ALL);
  LogComponentEnable ("SpqApplicationServer", LOG_LEVEL_ALL);
  //LogComponentEnable ("StrictPriorityQueue", LOG_LEVEL_ALL); //log for the queue
  
  //create variables we will need
  std::string configPath = "";
  uint16_t portHigh = 9999;
  uint16_t portLow  = 1111;


  //read command line argument
    //in this case, only config file
  CommandLine cmd;
  cmd.AddValue ("config", "The path to the config file that will be read", configPath);
  cmd.Parse (argc, argv);


  //----------------------------------- create nodes -----------------------------------
	NodeContainer c;
	c.Create(3);

  
  //----------------------------------- create links -----------------------------------

  // We create the channels first without any IP addressing information
  PointToPointHelper p2p;
  //std::string str;

  // Point-to-point links
  NodeContainer c01 = NodeContainer(c.Get (0), c.Get (1)); //link 1
  NodeContainer c12 = NodeContainer(c.Get (1), c.Get (2)); //link 2

  // populate link 1 
  p2p.SetDeviceAttribute ("DataRate", StringValue ("4Mbps"));
  p2p.SetChannelAttribute ("Delay", StringValue ("1ms"));
  NetDeviceContainer d01 = p2p.Install (c01);
  p2p.EnablePcap("pre_SPQ", d01.Get(0), BooleanValue(false));


  //populate link 2
  p2p.SetDeviceAttribute ("DataRate", StringValue("1Mbps"));
  p2p.SetChannelAttribute ("Delay", StringValue ("1ms"));
	//p2p.AddQueueToOne ("ns3::StrictPriorityQueue<Packet>");
  NetDeviceContainer d12 = p2p.Install(c12);
  Ptr<PointToPointNetDevice> net_device = DynamicCast<PointToPointNetDevice>(d12.Get(0));
  Ptr<StrictPriorityQueue> SPQ = new StrictPriorityQueue();
  SPQ->SetMode(DiffServ::packet);
  net_device->SetQueue(SPQ);
  p2p.EnablePcap("post_SPQ",d12.Get(0), BooleanValue(false));

  //not quite sure what this does, tbh
  p2p.SetCompress (BooleanValue (false));

  //----------------------------------- add queue to middle node -----------------------------------
  //todo:
    //there's no way it's this easy
    //this sets all queues to SPQ. Do we only want to set the middle?
      //is there more that one queue?
  //p2p.SetQueue(std::string("ns3::StrictPriorityQueue<Packet>"));
  //p2p.SetQueue(std::string("ns3::DropTailQueue"));


  //----------------------------------- add to internet -----------------------------------
  InternetStackHelper internet;
  internet.Install (c);


  NS_LOG_INFO ("Assign IP Addresses.");
  Ipv4AddressHelper ipv4;

  ipv4.SetBase ("10.0.1.0", "255.255.255.0");
  Ipv4InterfaceContainer i01 = ipv4.Assign (d01);

  ipv4.SetBase ("10.0.2.0", "255.255.255.0");
  Ipv4InterfaceContainer i12 = ipv4.Assign (d12);

  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();


  //----------------------------------- add application -----------------------------------

  //two servers, one for high priority, and one for low
  SpqServerHelper highServer(portHigh);
  ApplicationContainer appsHigh = highServer.Install(c.Get (2));
  appsHigh.Start(Seconds (0.0));
  appsHigh.Stop(Seconds (40.0));

  SpqServerHelper lowServer(portLow);
  ApplicationContainer appsLow = lowServer.Install(c.Get (2));
  //appsLow = lowServer.Install(c.Get (2));
  appsLow.Start(Seconds (0.0));
  appsLow.Stop(Seconds (40.0));



  // two clients, one for high priority, one for low
    //note: not sure that's the correct way to get the destination address
  SpqClientHelper highClient(i12.GetAddress(1), portHigh);
  appsHigh = highClient.Install (c.Get (0));
  appsHigh.Start (Seconds (10.0)); //start sending at 12s
  appsHigh.Stop (Seconds (40.0));

  SpqClientHelper lowClient(i12.GetAddress(1), portLow);
  appsLow = lowClient.Install (c.Get (0));
  appsLow.Start (Seconds (0.0)); //start sending at immediately 
  appsLow.Stop (Seconds (40.0));


  Simulator::Run ();
  Simulator::Destroy ();


	return 0;
}

