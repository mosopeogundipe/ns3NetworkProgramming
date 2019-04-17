/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

 /*
  Authors:
   Ian Granger

  Purpose:

  Date:
   2/25/19

  Todo:
  
*/

#ifndef SPQ_APPLICATION_HELPER_H
#define SPQ_APPLICATION_HELPER_H
 
#include <stdint.h>
#include "ns3/application-container.h"
#include "ns3/node-container.h"
#include "ns3/object-factory.h"
#include "ns3/ipv4-address.h"
#include "ns3/spq-application-client.h"
#include "ns3/spq-application-server.h"


namespace ns3 {
  class SpqServerHelper
  {
  public:
    SpqServerHelper ();
   
    SpqServerHelper (uint16_t port);
   
    void SetAttribute (std::string name, const AttributeValue &value);
   
    ApplicationContainer Install (NodeContainer c);
   
    Ptr<CompressionDetectionServer> GetServer (void);
  private:
    ObjectFactory m_factory; 
    Ptr<CompressionDetectionServer> m_server; 
   };
   
  class SpqClientHelper
  {
   
  public:
    SpqClientHelper ();
   
    SpqClientHelper (Address ip, uint16_t port);
    SpqClientHelper (Address addr);
   
    void SetAttribute (std::string name, const AttributeValue &value);
   
    ApplicationContainer Install (NodeContainer c);
   
  private:
    ObjectFactory m_factory; 
  };
 
} // namespace ns3
 
#endif /* UDP_CLIENT_SERVER_H */