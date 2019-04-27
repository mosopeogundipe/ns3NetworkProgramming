/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

 /*
  Authors:
    Ian Granger

  Purpose:


  Date:
    4/16/19

  Todo:
  
*/
 
 #ifndef DRR_APPLICATION_SERVER_H
 #define DRR_APPLICATION_SERVER_H
 
 #include "ns3/udp-server.h"
 #include "ns3/event-id.h"
 #include "ns3/ptr.h"
 #include "ns3/address.h"
 #include "ns3/traced-callback.h"
 #include "packet-loss-counter.h"
 
 namespace ns3 {
 class DrrApplicationServer : public Application
 {
 public:
   static TypeId GetTypeId (void);
   DrrApplicationServer ();
   virtual ~DrrApplicationServer ();

   uint32_t GetLost (void) const;
 
   uint64_t GetReceived (void) const;
 
   uint16_t GetPacketWindowSize () const;
 
   void SetPacketWindowSize (uint16_t size);

 protected:
   virtual void DoDispose (void);
 
 private:
 
   virtual void StartApplication (void);
   virtual void StopApplication (void);
 
   void HandleRead (Ptr<Socket> socket);
 
   uint16_t m_port; 
   Ptr<Socket> m_socket; 
   Ptr<Socket> m_socket6; 
   uint64_t m_received; 
   PacketLossCounter m_lossCounter; 

   TracedCallback<Ptr<const Packet> > m_rxTrace;
 
   TracedCallback<Ptr<const Packet>, const Address &, const Address &> m_rxTraceWithAddresses;
 
 };
 
 } // namespace ns3
 
 #endif /* DRR_APPLICATION_SERVER_H */
