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
#include "spq-application-server.h"

namespace ns3 {

	NS_LOG_COMPONENT_DEFINE ("SpqApplicationServer");

	NS_OBJECT_ENSURE_REGISTERED (SpqApplicationServer);
	
	
	TypeId
	SpqApplicationServer::GetTypeId (void)
	{

	static TypeId tid = TypeId ("ns3::SpqApplicationServer")
			.SetParent<Application> ()
			.SetGroupName("Applications")
			.AddConstructor<SpqApplicationServer> ()
			.AddAttribute ("Port",
							"Port on which we listen for incoming packets.",
							UintegerValue (100),
							MakeUintegerAccessor (&SpqApplicationServer::m_port),
							MakeUintegerChecker<uint16_t> ())
			.AddAttribute ("PacketWindowSize",
							"The size of the window used to compute the packet loss. This value should be a multiple of 8.",
							UintegerValue (32),
							MakeUintegerAccessor (&SpqApplicationServer::GetPacketWindowSize,
												 &SpqApplicationServer::SetPacketWindowSize),
							MakeUintegerChecker<uint16_t> (8,256))
			.AddTraceSource ("Rx", "A packet has been received",
							MakeTraceSourceAccessor (&SpqApplicationServer::m_rxTrace),
							"ns3::Packet::TracedCallback")
			.AddTraceSource ("RxWithAddresses", "A packet has been received",
							MakeTraceSourceAccessor (&SpqApplicationServer::m_rxTraceWithAddresses),
							"ns3::Packet::TwoAddressTracedCallback")
		;
		return tid;
	}
	
	SpqApplicationServer::SpqApplicationServer () : m_lossCounter (0)
	{
		NS_LOG_FUNCTION (this);
		difference = 0;
		m_received=0;
	}
	
	SpqApplicationServer::~SpqApplicationServer ()
	{
		NS_LOG_FUNCTION (this);
	}
	
	uint16_t
	SpqApplicationServer::GetPacketWindowSize () const
	{
		NS_LOG_FUNCTION (this);
		return m_lossCounter.GetBitMapSize ();
	}
	
	void
	SpqApplicationServer::SetPacketWindowSize (uint16_t size)
	{
		NS_LOG_FUNCTION (this << size);
		m_lossCounter.SetBitMapSize (size);
	}
	
	uint32_t
	SpqApplicationServer::GetLost (void) const
	{
		NS_LOG_FUNCTION (this);
		return m_lossCounter.GetLost ();
	}
	
	uint64_t
	SpqApplicationServer::GetReceived (void) const
	{
		NS_LOG_FUNCTION (this);
		return m_received;
	}
	
	Time 
	SpqApplicationServer::GetTimeDifference(void)
	{
		return diff;
	}

	void
	SpqApplicationServer::DoDispose (void)
	{
		NS_LOG_FUNCTION (this);
		Application::DoDispose ();
	}
	
	void
	SpqApplicationServer::StartApplication (void)
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
	
		m_socket->SetRecvCallback (MakeCallback (&SpqApplicationServer::HandleRead, this));
	
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
	
		m_socket6->SetRecvCallback (MakeCallback (&SpqApplicationServer::HandleRead, this));
	
	}
	
	void
	SpqApplicationServer::StopApplication ()
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
	SpqApplicationServer::HandleRead (Ptr<Socket> socket)
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
						NS_LOG_INFO ("server recieved packet: "<< m_received << " on port "<< m_port);

						SeqTsHeader seqTs;
						packet->RemoveHeader(seqTs);
						uint32_t currentSequenceNumber = seqTs.GetSeq ();

						if(currentSequenceNumber == 0){
							first = Simulator::Now();
						}
						last = Simulator::Now();

						m_lossCounter.NotifyReceived(currentSequenceNumber);
						m_received++;

					}
			}
	}
	
	//checks if a packet's data contains only zeros. If so, it's a low entropy packet
	//if we have >= 1100 zeros, then it's a low entropy packet
	bool SpqApplicationServer::IsLowEntropyPacket(Ptr<Packet> packet){
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
	
	void SpqApplicationServer::PrintResult(){		
		if(difference >= 100){
			NS_LOG_INFO ("Compression detected!\n\tDifference In arrival times: "<< difference);
		}
		else{
			NS_LOG_INFO ("No Compression detected!\n\tDifference In arrival times: "<< difference);

		}
		difference = 0;
	} 

 } // Namespace ns3
