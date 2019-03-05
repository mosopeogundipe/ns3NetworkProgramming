/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

 /*
  Authors:
   Ian Granger

  Purpose:

  Date:
   2/25/19

  Todo:
  
*/

#ifndef COMPRESSIONDETECTIONCLIENTSERVERHELPER_H
#define COMPRESSIONDETECTIONCLIENTSERVERHELPER_H
 
#include <stdint.h>
#include "ns3/application-container.h"
#include "ns3/node-container.h"
#include "ns3/object-factory.h"
#include "ns3/ipv4-address.h"
#include "ns3/udp-server.h"
#include "ns3/udp-client.h"


namespace ns3 {
  class CompressionDetectionServerHelper
  {
  public:
    CompressionDetectionServerHelper ();
   
    CompressionDetectionServerHelper (uint16_t port);
   
    void SetAttribute (std::string name, const AttributeValue &value);
   
    ApplicationContainer Install (NodeContainer c);
   
    Ptr<UdpServer> GetServer (void);
  private:
    ObjectFactory m_factory; 
    Ptr<UdpServer> m_server; 
   };
   
  class CompressionDetectionClientHelper
  {
   
  public:
    CompressionDetectionClientHelper ();
   
    CompressionDetectionClientHelper (Address ip, uint16_t port);
    CompressionDetectionClientHelper (Address addr);
   
    void SetAttribute (std::string name, const AttributeValue &value);
   
    ApplicationContainer Install (NodeContainer c);
   
  private:
    ObjectFactory m_factory; 
  };
 
} // namespace ns3
 
#endif /* UDP_CLIENT_SERVER_H */