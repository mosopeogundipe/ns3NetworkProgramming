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
#include "CompressionDetectionServer.h"

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

		Time head= Time::Time();
		Time tail= Time::Time();
		
		while ((packet = socket->RecvFrom (from)))
			{
				socket->GetSockName (localAddress);
				m_rxTrace (packet);
				m_rxTraceWithAddresses (packet, from, localAddress);
				if (packet->GetSize () > 0)
					{

						SeqTsHeader seqTs;
						packet->RemoveHeader (seqTs);
						uint32_t currentSequenceNumber = seqTs.GetSeq ();

						//if early in packet train, set head
						if(m_received%6000 == 1){
							head = Simulator::Now();
						} //if close to end of train. Can't use exact end, as some packets may be lost
						else if (m_received%6000 == 5750){
							tail = Simulator::Now();
						}

						int64_t headMs = head.GetMilliSeconds();
						int64_t tailMs = tail.GetMilliSeconds();
						if((headMs != 0) & (tailMs != 0)){
							int64_t dif = tailMs - headMs;

							if(dif >= 100){
								NS_LOG_INFO ("Compression detected!\n\tDifference In arriva times: "<< dif);
							}
							else{
								NS_LOG_INFO ("No compression was detected.\n\tDifference In arriva times: "<< dif);
							}
						}

	
						m_lossCounter.NotifyReceived (currentSequenceNumber);
						m_received++;
					}
			}
	}
	
 } // Namespace ns3
