/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

 /*
  Authors:
   Ian Granger

  Purpose:

  Date:
   2/25/19

  Todo:
  
*/

#ifndef DRR_APPLICATION_HELPER_H
#define DRR_APPLICATION_HELPER_H
 
#include <stdint.h>
#include "ns3/application-container.h"
#include "ns3/node-container.h"
#include "ns3/object-factory.h"
#include "ns3/ipv4-address.h"
#include "ns3/drr-application-client.h"
#include "ns3/drr-application-server.h"


namespace ns3 {
  class DrrServerHelper
  {
  public:
    DrrServerHelper ();
   
    DrrServerHelper (uint16_t port);
   
    void SetAttribute (std::string name, const AttributeValue &value);
   
    ApplicationContainer Install (NodeContainer c);
   
    Ptr<CompressionDetectionServer> GetServer (void); //not sure if needed, may delete later
  private:
    ObjectFactory m_factory; 
    Ptr<CompressionDetectionServer> m_server; 
   };
   
  class DrrClientHelper
  {
   
  public:
    DrrClientHelper ();
   
    DrrClientHelper (Address ip, uint16_t port);
    DrrClientHelper (Address addr);
   
    void SetAttribute (std::string name, const AttributeValue &value);
   
    ApplicationContainer Install (NodeContainer c);
   
  private:
    ObjectFactory m_factory; 
  };
 
} // namespace ns3
 
#endif /* DRR_APPLICATION_HELPER_H */