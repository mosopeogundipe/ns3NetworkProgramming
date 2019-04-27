/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

 /*
	Authors:
		Ian Granger

	Purpose:


	Date:
		4/16/19

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
#include "drr-application-server.h"

namespace ns3 {

	NS_LOG_COMPONENT_DEFINE ("DrrApplicationServer");

	NS_OBJECT_ENSURE_REGISTERED (DrrApplicationServer);
	
	
	TypeId
	DrrApplicationServer::GetTypeId (void)
	{

	static TypeId tid = TypeId ("ns3::DrrApplicationServer")
			.SetParent<Application> ()
			.SetGroupName("Applications")
			.AddConstructor<DrrApplicationServer> ()
			.AddAttribute ("Port",
							"Port on which we listen for incoming packets.",
							UintegerValue (100),
							MakeUintegerAccessor (&DrrApplicationServer::m_port),
							MakeUintegerChecker<uint16_t> ())
			.AddAttribute ("PacketWindowSize",
							"The size of the window used to compute the packet loss. This value should be a multiple of 8.",
							UintegerValue (32),
							MakeUintegerAccessor (&DrrApplicationServer::GetPacketWindowSize,
												 &DrrApplicationServer::SetPacketWindowSize),
							MakeUintegerChecker<uint16_t> (8,256))
			.AddTraceSource ("Rx", "A packet has been received",
							MakeTraceSourceAccessor (&DrrApplicationServer::m_rxTrace),
							"ns3::Packet::TracedCallback")
			.AddTraceSource ("RxWithAddresses", "A packet has been received",
							MakeTraceSourceAccessor (&DrrApplicationServer::m_rxTraceWithAddresses),
							"ns3::Packet::TwoAddressTracedCallback")
		;
		return tid;
	}
	
	DrrApplicationServer::DrrApplicationServer () : m_lossCounter (0)
	{
		NS_LOG_FUNCTION (this);
		m_received=0;
	}
	
	DrrApplicationServer::~DrrApplicationServer ()
	{
		NS_LOG_FUNCTION (this);
	}
	
	uint16_t
	DrrApplicationServer::GetPacketWindowSize () const
	{
		NS_LOG_FUNCTION (this);
		return m_lossCounter.GetBitMapSize ();
	}
	
	void
	DrrApplicationServer::SetPacketWindowSize (uint16_t size)
	{
		NS_LOG_FUNCTION (this << size);
		m_lossCounter.SetBitMapSize (size);
	}
	
	uint32_t
	DrrApplicationServer::GetLost (void) const
	{
		NS_LOG_FUNCTION (this);
		return m_lossCounter.GetLost ();
	}
	
	uint64_t
	DrrApplicationServer::GetReceived (void) const
	{
		NS_LOG_FUNCTION (this);
		return m_received;
	}

	void
	DrrApplicationServer::DoDispose (void)
	{
		NS_LOG_FUNCTION (this);
		Application::DoDispose ();
	}
	
	void
	DrrApplicationServer::StartApplication (void)
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
	
		m_socket->SetRecvCallback (MakeCallback (&DrrApplicationServer::HandleRead, this));
	
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
	
		m_socket6->SetRecvCallback (MakeCallback (&DrrApplicationServer::HandleRead, this));
	
	}
	
	void
	DrrApplicationServer::StopApplication ()
	{
		NS_LOG_FUNCTION (this);
		//PrintResult();
	
		if (m_socket != 0)
			{
				m_socket->SetRecvCallback (MakeNullCallback<void, Ptr<Socket> > ());
			}
	}
	
	void
	DrrApplicationServer::HandleRead (Ptr<Socket> socket)
	{
		//NS_LOG_FUNCTION (this << socket);
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
						NS_LOG_INFO ("server recieved packet: "<< m_received+1 << " on port "<< m_port);

						SeqTsHeader seqTs;
						packet->RemoveHeader (seqTs);
						uint32_t currentSequenceNumber = seqTs.GetSeq ();
						
						m_lossCounter.NotifyReceived(currentSequenceNumber);
						m_received++;

					}
			}
}
	
 } // Namespace ns3
