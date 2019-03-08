/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

 /*
  Authors:
    Ian Granger

  Purpose:
    Recieves two packet trains of 6000 packets each. 
    From there, it looks at the arrival time of the first 
    and last packet. If that arrival time > 100ms, then 
    compression was detected.

  Date:
    2/25/19

  Todo:
    Everything
  
*/
 
 #ifndef COMPRESSION_DETECTION_SERVER_H
 #define COMPRESSION_DETECTION_SERVER_H
 
 #include "ns3/udp-server.h"
 #include "ns3/event-id.h"
 #include "ns3/ptr.h"
 #include "ns3/address.h"
 #include "ns3/traced-callback.h"
 #include "packet-loss-counter.h"
 
 namespace ns3 {
 class CompressionDetectionServer : public Application
 {
 public:
   static TypeId GetTypeId (void);
   CompressionDetectionServer ();
   virtual ~CompressionDetectionServer ();

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
   //void CheckCompression(uint64_t rxTimestamp); //function used to check if there has been compression
 
   uint16_t m_port; 
   Ptr<Socket> m_socket; 
   Ptr<Socket> m_socket6; 
   uint64_t m_received; 
   PacketLossCounter m_lossCounter; 

   //variables used to test compression
    Time firstLow= Time();
	  Time lastLow= Time();
    Time firstHigh= Time();
	  Time lastHigh= Time();

    bool hasSeenFirstLowEntropyPacket = false;
    bool hasSeenFirstHighEntropyPacket = false;
    bool IsLowEntropyPacket(Ptr<Packet> packet);
    void PrintResult();
    int64_t difference;

   TracedCallback<Ptr<const Packet> > m_rxTrace;
 
   TracedCallback<Ptr<const Packet>, const Address &, const Address &> m_rxTraceWithAddresses;
 
 };
 
 } // namespace ns3
 
 #endif /* COMPRESSIONDETECTIONSERVER_H */