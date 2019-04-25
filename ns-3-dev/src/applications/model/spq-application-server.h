/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

 /*
  Authors:
    Ian Granger

  Purpose:


  Date:
    4/16/19

  Todo:

  
*/
 
 #ifndef SPQ_APPLICATION_SERVER_H
 #define SPQ_APPLICATION_SERVER_H
 
 #include "ns3/udp-server.h"
 #include "ns3/event-id.h"
 #include "ns3/ptr.h"
 #include "ns3/address.h"
 #include "ns3/traced-callback.h"
 #include "packet-loss-counter.h"
 
 namespace ns3 {
 class SpqApplicationServer : public Application
 {
 public:
   static TypeId GetTypeId (void);
   SpqApplicationServer ();
   virtual ~SpqApplicationServer ();

   uint32_t GetLost (void) const;
 
   uint64_t GetReceived (void) const;
 
   uint16_t GetPacketWindowSize () const;
 
   void SetPacketWindowSize (uint16_t size);

   Time GetTimeDifference(void);
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
    Time first= Time();
	  Time last= Time();
    Time diff = Time();
    //Time firstHigh= Time();
	  //Time lastHigh= Time();

    bool hasSeenFirstLowEntropyPacket = false;
    bool hasSeenFirstHighEntropyPacket = false;
    bool IsLowEntropyPacket(Ptr<Packet> packet);
    void PrintResult();    
    int64_t difference;

   TracedCallback<Ptr<const Packet> > m_rxTrace;
 
   TracedCallback<Ptr<const Packet>, const Address &, const Address &> m_rxTraceWithAddresses;
 
 };
 
 } // namespace ns3
 
 #endif /* SPQ_APPLICATION_SERVER_H */
