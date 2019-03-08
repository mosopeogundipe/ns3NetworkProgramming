/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

 /*
  Authors:
   Ian Granger

  Purpose:

  Date:
   2/25/19

  Todo:
  
*/

#ifndef COMPRESSION_DETECTION_CLIENT_SERVERHELPER_H
#define COMPRESSION_DETECTION_CLIENT_SERVERHELPER_H
 
#include <stdint.h>
#include "ns3/application-container.h"
#include "ns3/node-container.h"
#include "ns3/object-factory.h"
#include "ns3/ipv4-address.h"
#include "ns3/compression-detection-server.h"
#include "ns3/compression-detection-client.h"


namespace ns3 {
  class CompressionDetectionServerHelper
  {
  public:
    CompressionDetectionServerHelper ();
   
    CompressionDetectionServerHelper (uint16_t port);
   
    void SetAttribute (std::string name, const AttributeValue &value);
   
    ApplicationContainer Install (NodeContainer c);
   
    Ptr<CompressionDetectionServer> GetServer (void);
  private:
    ObjectFactory m_factory; 
    Ptr<CompressionDetectionServer> m_server; 
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