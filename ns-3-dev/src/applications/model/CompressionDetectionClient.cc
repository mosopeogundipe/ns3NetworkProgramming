/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

 /*
	Authors:
		Ian Granger

	Purpose:
		Creates a client application, to send two packet trains. 
		The first packect train will be 6000 packets, 
		each with 1100 bytes of payload, which should be set to 
		all 0's. The second packet train will be sent 5 seconds 
		later, and have the same specs, except it will be 
		filled with random data. Mostly a copy of udp-client, 
		with some necessary changes.

	Date:
		2/25/19

	Todo:
		Pretty much everything
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
#include "CompressionDetectionClient.h"
#include "seq-ts-header.h"
#include <cstdlib>
#include <cstdio>
#include <istream>
#include <fnctl.h> 

namespace ns3 {
 
	NS_LOG_COMPONENT_DEFINE ("CompressionDetectionClient");
	 
	NS_OBJECT_ENSURE_REGISTERED (CompressionDetectionClient);
	 
	TypeId
	CompressionDetectionClient::GetTypeId (void)
	{
		static TypeId tid = TypeId ("ns3::CompressionDetectionClient")
			.SetParent<Application> ()
			.SetGroupName("Applications")
			.AddConstructor<CompressionDetectionClient> ()
			//Any additional attributes needed?
			.AddAttribute ("MaxPackets",
							"The maximum number of packets the application will send",
							UintegerValue (12000), //changed to 12000, for num of packets to send
							MakeUintegerAccessor (&CompressionDetectionClient::m_count),
							MakeUintegerChecker<uint32_t> ())
			.AddAttribute ("Interval",
							"The time to wait between packets",
							TimeValue (Seconds (0.001)), //should be 0, or very close to, as per faq
							MakeTimeAccessor (&CompressionDetectionClient::m_interval),
							MakeTimeChecker ())
			.AddAttribute ("RemoteAddress",
							"The destination Address of the outbound packets",
							AddressValue (),
							MakeAddressAccessor (&CompressionDetectionClient::m_peerAddress),
							MakeAddressChecker ())
			.AddAttribute ("RemotePort", 
			            	"The destination port of the outbound packets",
							UintegerValue (100),
							MakeUintegerAccessor (&CompressionDetectionClient::m_peerPort),
							MakeUintegerChecker<uint16_t> ())
			.AddAttribute ("PacketSize",
							"Size of packets generated. The minimum packet size is 12 bytes which is the size of the header carrying the sequence number and the time stamp.",
							UintegerValue (1112), //additional 12 bytes added for header and timestamp, 1100 bytes of payload required
							MakeUintegerAccessor (&CompressionDetectionClient::m_size),
							MakeUintegerChecker<uint32_t> (12,65507))
		;
		return tid;
	}
	 
	CompressionDetectionClient::CompressionDetectionClient ()
	{
		NS_LOG_FUNCTION (this);
		m_sent = 0;
		m_socket = 0;

		//note: if we stop while they both have that same Id, that'll probably cause an error
		m_sendTrain1 = EventId();
		m_sendTrain2 = EventId();
	}
	 
	CompressionDetectionClient::~CompressionDetectionClient ()
	{
		NS_LOG_FUNCTION (this);
	}
	 
	void
	CompressionDetectionClient::SetRemote (Address ip, uint16_t port)
	{
		NS_LOG_FUNCTION (this << ip << port);
		m_peerAddress = ip;
		m_peerPort = port;
	}
	 
	void
	CompressionDetectionClient::SetRemote (Address addr)
	{
		NS_LOG_FUNCTION (this << addr);
		m_peerAddress = addr;
	}
	 
	void
	CompressionDetectionClient::DoDispose (void)
	{
		NS_LOG_FUNCTION (this);
		Application::DoDispose ();
	}
	 
	void
	CompressionDetectionClient::StartApplication (void)
	{
		NS_LOG_FUNCTION (this);
	 
		if (m_socket == 0)
			{
				TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
				m_socket = Socket::CreateSocket (GetNode (), tid);
				if (Ipv4Address::IsMatchingType(m_peerAddress) == true)
					{
						if (m_socket->Bind () == -1)
							{
								NS_FATAL_ERROR ("Failed to bind socket");
							}
						m_socket->Connect (InetSocketAddress (Ipv4Address::ConvertFrom(m_peerAddress), m_peerPort));
					}
				else if (Ipv6Address::IsMatchingType(m_peerAddress) == true)
					{
						if (m_socket->Bind6 () == -1)
							{
								NS_FATAL_ERROR ("Failed to bind socket");
							}
						m_socket->Connect (Inet6SocketAddress (Ipv6Address::ConvertFrom(m_peerAddress), m_peerPort));
					}
				else if (InetSocketAddress::IsMatchingType (m_peerAddress) == true)
					{
						if (m_socket->Bind () == -1)
							{
								NS_FATAL_ERROR ("Failed to bind socket");
							}
						m_socket->Connect (m_peerAddress);
					}
				else if (Inet6SocketAddress::IsMatchingType (m_peerAddress) == true)
					{
						if (m_socket->Bind6 () == -1)
							{
								NS_FATAL_ERROR ("Failed to bind socket");
							}
						m_socket->Connect (m_peerAddress);
					}
				else
					{
						NS_ASSERT_MSG (false, "Incompatible address type: " << m_peerAddress);
					}
			}
	 
		m_socket->SetRecvCallback (MakeNullCallback<void, Ptr<Socket> > ());
		m_socket->SetAllowBroadcast (true);

		//where the functions to send the two trains are called
			//is a 5 sec delay good?
		m_sendTrain1 = Simulator::Schedule (Seconds (0.0), &CompressionDetectionClient::SendEmptyTrain, this);
		m_sendTrain2 = Simulator::Schedule (Seconds (5.0), &CompressionDetectionClient::SendRandomTrain, this);
	}
	 
	void
	CompressionDetectionClient::StopApplication (void)
	{
		NS_LOG_FUNCTION (this);
		Simulator::Cancel (m_sendTrain1);
		Simulator::Cancel (m_sendTrain2);
	}

	//all we need to do is change it so we send a train of 6000
		//already send them empty, do we really need ro read fro /dev/nul?
	void
	CompressionDetectionClient::SendEmptyTrain (void)
	{
		NS_LOG_FUNCTION (this);
		NS_ASSERT (m_sendTrain1.IsExpired ());
		SeqTsHeader seqTs;
		seqTs.SetSeq (m_sent);
		Ptr<Packet> p = Create<Packet> (m_size-(8+4)); // 8+4 : the size of the seqTs header
		p->AddHeader (seqTs);
	 
		std::stringstream peerAddressStringStream;
		if (Ipv4Address::IsMatchingType (m_peerAddress))
			{
				peerAddressStringStream << Ipv4Address::ConvertFrom (m_peerAddress);
			}
		else if (Ipv6Address::IsMatchingType (m_peerAddress))
			{
				peerAddressStringStream << Ipv6Address::ConvertFrom (m_peerAddress);
			}
	 
		if ((m_socket->Send (p)) >= 0)
			{
				++m_sent;
				NS_LOG_INFO ("TraceDelay TX " << m_size << " bytes to "
												<< peerAddressStringStream.str () << " Uid: "
												<< p->GetUid () << " Time: "
												<< (Simulator::Now ()).GetSeconds ());

			}
		else
			{
				NS_LOG_INFO ("Error while sending " << m_size << " bytes to "
														<< peerAddressStringStream.str ());
			}
	 
	 	//since we send the empty packets first, we only send half of the max (only 1 train)
		if (m_sent < m_count/2)
			{
				m_sendTrain1 = Simulator::Schedule (m_interval, &CompressionDetectionClient::SendEmptyTrain, this);
			}
	}

	//all we need to do is change it so we send a train of 6000
		//and read in 1100 bytes from /dev/random
	void
	CompressionDetectionClient::SendRandomTrain (void)
	{
		//first, read 1100 bytes into the buffer
			//possible issues:
				//unsigned char is not the same as uint8_t
				//not const enough
        unsigned char buffer[1100];
        int fd = open("/dev/random", O_RDONLY);
        read(fd, &buffer[0], 1100);
        close(fd);

		uint8_t buffer
		NS_LOG_FUNCTION (this);
		NS_ASSERT (m_sendTrain2.IsExpired ());
		SeqTsHeader seqTs;
		seqTs.SetSeq (m_sent);
		Ptr<Packet> p = Create<Packet> (&buffer[0], m_size-(8+4)); // 8+4 : the size of the seqTs header
		p->AddHeader (seqTs);
	 
		std::stringstream peerAddressStringStream;
		if (Ipv4Address::IsMatchingType (m_peerAddress))
			{
				peerAddressStringStream << Ipv4Address::ConvertFrom (m_peerAddress);
			}
		else if (Ipv6Address::IsMatchingType (m_peerAddress))
			{
				peerAddressStringStream << Ipv6Address::ConvertFrom (m_peerAddress);
			}
	 
		if ((m_socket->Send (p)) >= 0)
			{
				++m_sent;
				NS_LOG_INFO ("TraceDelay TX " << m_size << " bytes to "
												<< peerAddressStringStream.str () << " Uid: "
												<< p->GetUid () << " Time: "
												<< (Simulator::Now ()).GetSeconds ());
	 
			}
		else
			{
				NS_LOG_INFO ("Error while sending " << m_size << " bytes to "
														<< peerAddressStringStream.str ());
			}
	 
		//don't divide by 2 this time, because we're sending the remaining half
			//cleaner way might be:
			//m_sent%6000 == 0
		if (m_sent < m_count)
			{
				m_sendTrain2 = Simulator::Schedule (m_interval, &CompressionDetectionClient::SendRandomTrain, this);
			}
	}
 
} // Namespace ns3
