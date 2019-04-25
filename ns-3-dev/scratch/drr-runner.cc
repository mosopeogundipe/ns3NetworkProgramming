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
  LogComponentEnable ("DrrApplicationClient", LOG_LEVEL_ALL);
  LogComponentEnable ("DrrApplicationServer", LOG_LEVEL_ALL);
  
  //create variables we will need
  std::string configPath = "";
  bool enableGlobalCompression = false;
  uint16_t portHigh = 9999;
  uint16_t portMed  = 5555;
  uint16_t portLow  = 1111;


  //read command line argument
    //in this case, only config file
  CommandLine cmd;
  cmd.AddValue ("config", "The path to the config file that will be read", configPath);
  cmd.Parse (argc, argv);

  //Todo:
    //read values from config file


  //----------------------------------- create nodes -----------------------------------
  NodeContainer c;
  c.Create(3);

  
  //----------------------------------- create links -----------------------------------

  // We create the channels first without any IP addressing information
  PointToPointHelper p2p;
  std::string str;

  // Point-to-point links
  NodeContainer c01 = NodeContainer(c.Get (0), c.Get (1)); //link 1
  NodeContainer c12 = NodeContainer(c.Get (1), c.Get (2)); //link 2

  // populate link 1 
  p2p.SetDeviceAttribute ("DataRate", StringValue ("4Mbps"));
  p2p.SetChannelAttribute ("Delay", StringValue ("2ms"));
  NetDeviceContainer d01 = p2p.Install (c01);
  p2p.EnablePcap("UDPsender", d01.Get(0), BooleanValue(false));


  //populate link 2
  NS_LOG_INFO("Second link speed: 1Mbps");
  p2p.SetDeviceAttribute ("DataRate", StringValue("1Mbps"));
  p2p.SetChannelAttribute ("Delay", StringValue ("2ms"));
  NetDeviceContainer d12 = p2p.Install(c12);
  p2p.EnablePcap("Receiver",d12.Get(0), BooleanValue(false));

  //not quite sure what this does, tbh
  p2p.SetCompress (BooleanValue (false));

  //----------------------------------- add queue to middle node -----------------------------------
  //todo:
    //there's no way it's this easy
    //this sets all queues to SPQ. Do we only want to set the middle?
      //is there more that one queue?
  p2p.SetQueue("ns3::StrictPriorityQueue")


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
  DrrServerHelper highServer(portHigh)
  ApplicationContainer apps = highServer.Install(c.Get (0));
  apps.Start(Seconds (0.0));
  apps.Stop(Seconds (150.0));

  DrrServerHelper lowServer(portLow)
  ApplicationContainer apps = lowServer.Install(c.Get (2));
  apps.Start(Seconds (0.0));
  apps.Stop(Seconds (150.0));



  // two clients, one for high priority, one for low
    //note: not sure that's the correct way to get the destination address
  DrrClientHelper highClient(i12.GetAddress(1), portHigh)
  highClient.SetAttribute("SetEntropy", BooleanValue (false));
  apps = highClient.Install (c.Get (0));
  apps.Start (Seconds (0.0)); //all start at same time
  apps.Stop (Seconds (150.0));

  DrrClientHelper MedClient(i12.GetAddress(1), portMed)
  MedClient.SetAttribute("SetEntropy", BooleanValue (false));
  apps = MedClient.Install (c.Get (0));
  apps.Start (Seconds (0.0)); //all start at same time
  apps.Stop (Seconds (150.0));

  DrrClientHelper lowClient(i12.GetAddress(1), portLow)
  lowClient.SetAttribute("SetEntropy", BooleanValue (false));
  apps = lowClient.Install (c.Get (0));
  apps.Start (Seconds (0.0)); //all start at same time
  apps.Stop (Seconds (150.0));


  Simulator::Run ();
  Simulator::Destroy ();


  return 0;
}

