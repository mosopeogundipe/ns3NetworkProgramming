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
	
*/
 
#include "ns3/log.h"
#include "ns3/ipv4-address.h"
#include "ns3/nstime.h"
#include "ns3/inet-socket-address.h"
#include "ns3/inet6-socket-address.h"
#include "ns3/socket.h"
#include "ns3/simulator.h"
#include "ns3/socket-factory.h"
#include "ns3/packet.h"
#include "ns3/uinteger.h"
#include "packet-loss-counter.h"

#include "seq-ts-header.h"
#include "compression-detection-server.h"

namespace ns3 {

	NS_LOG_COMPONENT_DEFINE ("CompressionDetectionServer");

	NS_OBJECT_ENSURE_REGISTERED (CompressionDetectionServer);
	
	
	TypeId
	CompressionDetectionServer::GetTypeId (void)
	{

	static TypeId tid = TypeId ("ns3::CompressionDetectionServer")
			.SetParent<Application> ()
			.SetGroupName("Applications")
			.AddConstructor<CompressionDetectionServer> ()
			.AddAttribute ("Port",
							"Port on which we listen for incoming packets.",
							UintegerValue (100),
							MakeUintegerAccessor (&CompressionDetectionServer::m_port),
							MakeUintegerChecker<uint16_t> ())
			.AddAttribute ("PacketWindowSize",
							"The size of the window used to compute the packet loss. This value should be a multiple of 8.",
							UintegerValue (32),
							MakeUintegerAccessor (&CompressionDetectionServer::GetPacketWindowSize,
												 &CompressionDetectionServer::SetPacketWindowSize),
							MakeUintegerChecker<uint16_t> (8,256))
			.AddTraceSource ("Rx", "A packet has been received",
							MakeTraceSourceAccessor (&CompressionDetectionServer::m_rxTrace),
							"ns3::Packet::TracedCallback")
			.AddTraceSource ("RxWithAddresses", "A packet has been received",
							MakeTraceSourceAccessor (&CompressionDetectionServer::m_rxTraceWithAddresses),
							"ns3::Packet::TwoAddressTracedCallback")
		;
		return tid;
	}
	
	CompressionDetectionServer::CompressionDetectionServer () : m_lossCounter (0)
	{
		NS_LOG_FUNCTION (this);
		difference = 0;
		m_received=0;
	}
	
	CompressionDetectionServer::~CompressionDetectionServer ()
	{
		NS_LOG_FUNCTION (this);
	}
	
	uint16_t
	CompressionDetectionServer::GetPacketWindowSize () const
	{
		NS_LOG_FUNCTION (this);
		return m_lossCounter.GetBitMapSize ();
	}
	
	void
	CompressionDetectionServer::SetPacketWindowSize (uint16_t size)
	{
		NS_LOG_FUNCTION (this << size);
		m_lossCounter.SetBitMapSize (size);
	}
	
	uint32_t
	CompressionDetectionServer::GetLost (void) const
	{
		NS_LOG_FUNCTION (this);
		return m_lossCounter.GetLost ();
	}
	
	uint64_t
	CompressionDetectionServer::GetReceived (void) const
	{
		NS_LOG_FUNCTION (this);
		return m_received;
	}
	
	Time 
	CompressionDetectionServer::GetTimeDifference(void)
	{
		return diff;
	}

	void
	CompressionDetectionServer::DoDispose (void)
	{
		NS_LOG_FUNCTION (this);
		Application::DoDispose ();
	}
	
	void
	CompressionDetectionServer::StartApplication (void)
	{
		NS_LOG_FUNCTION (this);
	
		if (m_socket == 0)
			{
				TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
				m_socket = Socket::CreateSocket (GetNode (), tid);
				InetSocketAddress local = InetSocketAddress (Ipv4Address::GetAny (),
															m_port);
				if (m_socket->Bind (local) == -1)
					{
						NS_FATAL_ERROR ("Failed to bind socket");
					}
			}
	
		m_socket->SetRecvCallback (MakeCallback (&CompressionDetectionServer::HandleRead, this));
	
		if (m_socket6 == 0)
			{
				TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
				m_socket6 = Socket::CreateSocket (GetNode (), tid);
				Inet6SocketAddress local = Inet6SocketAddress (Ipv6Address::GetAny (),
																m_port);
				if (m_socket6->Bind (local) == -1)
					{
						NS_FATAL_ERROR ("Failed to bind socket");
					}
			}
	
		m_socket6->SetRecvCallback (MakeCallback (&CompressionDetectionServer::HandleRead, this));
	
	}
	
	void
	CompressionDetectionServer::StopApplication ()
	{
		NS_LOG_FUNCTION (this);
		//PrintResult();
		diff = last - first;
		if (m_socket != 0)
			{
				m_socket->SetRecvCallback (MakeNullCallback<void, Ptr<Socket> > ());
			}
	}
	
	void
	CompressionDetectionServer::HandleRead (Ptr<Socket> socket)
	{
		NS_LOG_FUNCTION (this << socket);
		Ptr<Packet> packet;
		Address from;
		Address localAddress;
		
		while ((packet = socket->RecvFrom (from)))
			{
				socket->GetSockName (localAddress);
				m_rxTrace (packet);
				m_rxTraceWithAddresses (packet, from, localAddress);
				if (packet->GetSize () > 0)
					{
						NS_LOG_INFO ("server recieved packet: "<< m_received%6000);

						SeqTsHeader seqTs;
						packet->RemoveHeader (seqTs);
						uint32_t currentSequenceNumber = seqTs.GetSeq ();

						if(currentSequenceNumber == 0){
							first = Simulator::Now();
						}
						last = Simulator::Now();
						// if(!hasSeenFirstLowEntropyPacket && IsLowEntropyPacket(packet)){
						// 	firstLow = Simulator::Now();
						// 	hasSeenFirstLowEntropyPacket = true;
						// }
						// else if(hasSeenFirstLowEntropyPacket && IsLowEntropyPacket(packet)){
						// 	lastLow = Simulator::Now();
						// }
						// else if(!hasSeenFirstHighEntropyPacket && !IsLowEntropyPacket(packet)){
						// 	firstHigh = Simulator::Now();
						// 	hasSeenFirstHighEntropyPacket = true;
						// }
						// else if(hasSeenFirstHighEntropyPacket && !IsLowEntropyPacket(packet)){
						// 	lastHigh = Simulator::Now();
						// }
						
						m_lossCounter.NotifyReceived (currentSequenceNumber);
						m_received++;

					}
			}
			// int64_t firstLowMs = firstLow.GetMilliSeconds();
			// int64_t lastLowMs = lastLow.GetMilliSeconds();

			// int64_t firstHighMs = firstHigh.GetMilliSeconds();
			// int64_t lastHighMs = lastHigh.GetMilliSeconds();
			// int64_t deltaLow = lastLowMs - firstLowMs;
			// int64_t deltaHigh = lastHighMs - firstHighMs;

			// difference = deltaHigh - deltaLow; 	//abs value was important to make it detect compression in links, was getting negative values for valid compression links
	}
	
	//checks if a packet's data contains only zeros. If so, it's a low entropy packet
	//if we have >= 1100 zeros, then it's a low entropy packet
	bool CompressionDetectionServer::IsLowEntropyPacket(Ptr<Packet> packet){
		int size = packet -> GetSize();
		uint8_t raw_data[size];
    	packet->CopyData(raw_data, size);
		int numberOfZeros = 0;
		for(int i=0; i<size; i++){
			if((int)raw_data[i] == 0){
				numberOfZeros ++;
			}
		}
		if(numberOfZeros >= 1100){//1100 is our packet data size
			return true;
		}
		return false;
	}
	
	void CompressionDetectionServer::PrintResult(){		
		if(difference >= 100){
			NS_LOG_INFO ("Compression detected!\n\tDifference In arrival times: "<< difference);
		}
		else{
			NS_LOG_INFO ("No Compression detected!\n\tDifference In arrival times: "<< difference);
			// NS_LOG_INFO ("No compression was detected.\n"
			// <<"\tfirst low entropy packet time: " << firstLow << "\n"
			// <<"\tlast low entropy packet time: " << lastLow << "\n"
			// <<"\tfirst high entropy packet time: " << firstHigh << "\n"
			// <<"\tlast high entropy packet time: " << lastHigh << "\n"
			// <<"\t\tDifference In arrival times: "<< dif);
		}
		difference = 0;
	} 

 } // Namespace ns3
