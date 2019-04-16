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
#include "ns3/boolean.h"

#include "seq-ts-header.h"
#include "spq-application-client.h"

#include <cstdlib>
#include <cstdio>
#include <fstream> 
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <iostream>

namespace ns3 {
 
	NS_LOG_COMPONENT_DEFINE ("SpqApplicationClient");
	 
	NS_OBJECT_ENSURE_REGISTERED (SpqApplicationClient);
	 
	TypeId
	SpqApplicationClient::GetTypeId (void)
	{
		static TypeId tid = TypeId ("ns3::SpqApplicationClient")
			.SetParent<Application> ()
			.SetGroupName("Applications")
			.AddConstructor<SpqApplicationClient> ()
			//Any additional attributes needed?
			.AddAttribute ("MaxPackets",
							"The maximum number of packets the application will send",
							UintegerValue (6000), //changed to 12000, for num of packets to send
							MakeUintegerAccessor (&SpqApplicationClient::m_count),
							MakeUintegerChecker<uint32_t> ())
			.AddAttribute ("Interval",
							"The time to wait between packets",
							TimeValue (Seconds (0.001)), //0.00001 gives good results //should be 0, or very close to, as per faq
							MakeTimeAccessor (&SpqApplicationClient::m_interval),
							MakeTimeChecker ())
			.AddAttribute ("RemoteAddress",
							"The destination Address of the outbound packets",
							AddressValue (),
							MakeAddressAccessor (&SpqApplicationClient::m_peerAddress),
							MakeAddressChecker ())
			.AddAttribute ("RemotePort", 
			            	"The destination port of the outbound packets",
							UintegerValue (100),
							MakeUintegerAccessor (&SpqApplicationClient::m_peerPort),
							MakeUintegerChecker<uint16_t> ())
			.AddAttribute ("PacketSize",
							"Size of packets generated. The minimum packet size is 12 bytes which is the size of the header carrying the sequence number and the time stamp.",
							UintegerValue (1100), //additional 12 bytes added for header and timestamp, 1100 bytes of payload required
							//UintegerValue (1100),
							MakeUintegerAccessor (&SpqApplicationClient::m_size),
							//MakeUintegerChecker<uint32_t> (12,65507))
							MakeUintegerChecker<uint32_t> ())
			.AddAttribute ("SetEntropy",
							"If true, create high entropy packets. If false, create low entropy packets",
							BooleanValue (false), //additional 12 bytes added for header and timestamp, 1100 bytes of payload required
							MakeBooleanAccessor (&SpqApplicationClient::m_set_entropy),
							MakeBooleanChecker())

		;
		return tid;
	}
	 
	SpqApplicationClient::SpqApplicationClient ()
	{
		NS_LOG_FUNCTION (this);
		m_sent = 0;
		m_socket = 0;

		//note: if we stop while they both have that same Id, that'll probably cause an error
		m_sendTrain1 = EventId();
		m_sendTrain2 = EventId();
	}
	 
	SpqApplicationClient::~SpqApplicationClient ()
	{
		NS_LOG_FUNCTION (this);
	}
	 
	void
	SpqApplicationClient::SetRemote (Address ip, uint16_t port)
	{
		NS_LOG_FUNCTION (this << ip << port);
		m_peerAddress = ip;
		m_peerPort = port;
	}
	 
	void
	SpqApplicationClient::SetRemote (Address addr)
	{
		NS_LOG_FUNCTION (this << addr);
		m_peerAddress = addr;
	}
	 
	void
	SpqApplicationClient::DoDispose (void)
	{
		NS_LOG_FUNCTION (this);
		Application::DoDispose ();
	}
	 
	void
	SpqApplicationClient::StartApplication (void)
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
		m_sendEvent = Simulator::Schedule (Seconds (0.0), &SpqApplicationClient::Send, this);

		//where the functions to send the two trains are called
			//is a 5 sec delay good?
		// if(m_set_entropy){
		// 	NS_LOG_INFO ("Sending HIGH Entropy Packets...");
		// 	m_sendTrain2 = Simulator::Schedule (Seconds (0.0), &SpqApplicationClient::SendRandomTrain, this);
		// }else{
		// 	NS_LOG_INFO ("Sending LOW Entropy Packets...");
		// 	m_sendTrain1 = Simulator::Schedule (Seconds (0.0), &SpqApplicationClient::SendEmptyTrain, this);
		// }
		//m_sendTrain1 = Simulator::Schedule (Seconds (0.0), &SpqApplicationClient::SendEmptyTrain, this);
		//m_sendTrain2 = Simulator::Schedule (Seconds (25.0), &SpqApplicationClient::SendRandomTrain, this);
	}
	 
	void
	SpqApplicationClient::StopApplication (void)
	{
		NS_LOG_FUNCTION (this);
		Simulator::Cancel (m_sendTrain1);
		Simulator::Cancel (m_sendTrain2);
	}

void
 SpqApplicationClient::createLowEntropyPackets (uint8_t*  buffer, uint32_t m_size)
 {
  for (uint32_t i = 0; i < m_size; i++){
    buffer[i] = 0x00;
  }
 }
 //create high entropy
 void
 SpqApplicationClient::createHighEntropyPackets (uint8_t* buffer, uint32_t m_size)
 {
	int fd = open("/dev/urandom", O_RDONLY);
	read(fd, buffer, m_size);
	 //buffer now contains the random data
 }

	void
SpqApplicationClient::Send (void)
{
  NS_LOG_FUNCTION (this);
    SeqTsHeader seqTs;
  //cout<<m_sent;
  seqTs.SetSeq (m_sent);
  uint8_t* buffer = new uint8_t[m_size+8+4];
   Ptr<Packet> p;
   //set entropy high and low done  
  if (m_set_entropy == true) { //if true
   createHighEntropyPackets(buffer, m_size+8+4);
    p = Create<Packet> (buffer, m_size+8+4);
  }else{ //false 
    createLowEntropyPackets(buffer, m_size+8+4);
    p = Create<Packet> (buffer, m_size+8+4);
  }
  //Ptr<Packet> p = Create<Packet> ((uint8_t*) str.str().c_str(), str.str().length() + 1); // 8+4 : the size of the seqTs header
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

  if (m_sent < m_count)
    {
      m_sendEvent = Simulator::Schedule (m_interval, &SpqApplicationClient::Send, this);
    }
}

	// //all we need to do is change it so we send a train of 6000
	// 	//already send them empty, do we really need ro read fro /dev/nul?
	// void
	// SpqApplicationClient::SendEmptyTrain (void)
	// {
	// 	NS_LOG_FUNCTION (this);
	// 	NS_ASSERT (m_sendTrain1.IsExpired ());
	// 	SeqTsHeader seqTs;
	// 	seqTs.SetSeq (m_sent);
	// 	Ptr<Packet> p = Create<Packet> (m_size-(8+4)); // 8+4 : the size of the seqTs header
	// 	p->AddHeader (seqTs);
	 
	// 	std::stringstream peerAddressStringStream;
	// 	if (Ipv4Address::IsMatchingType (m_peerAddress))
	// 		{
	// 			peerAddressStringStream << Ipv4Address::ConvertFrom (m_peerAddress);
	// 		}
	// 	else if (Ipv6Address::IsMatchingType (m_peerAddress))
	// 		{
	// 			peerAddressStringStream << Ipv6Address::ConvertFrom (m_peerAddress);
	// 		}
	 
	// 	if ((m_socket->Send (p)) >= 0)
	// 		{
	// 			++m_sent;
	// 			NS_LOG_INFO ("  Sending empty packet: " << m_sent%6000);

	// 		}
	// 	else
	// 		{
	// 			NS_LOG_INFO ("Error while sending " << m_size << " bytes to "
	// 													<< peerAddressStringStream.str ());
	// 		}
	 
	//  	//since we send the empty packets first, we only send half of the max (only 1 train)
	// 	if (m_sent < m_count/2)
	// 		{
	// 			m_sendTrain1 = Simulator::Schedule (m_interval, &SpqApplicationClient::SendEmptyTrain, this);
	// 		}
	// }

	//all we need to do is change it so we send a train of 6000
		//and read in 1100 bytes from /dev/random
	// void
	// SpqApplicationClient::SendRandomTrain (void)
	// {
	// 	//first, read 1100 bytes into the buffer
	// 		//possible issues:
	// 			//unsigned char is not the same as uint8_t
	// 			//not const enough
  //       unsigned char buffer[1100];
	// 			std::fstream fs ("/dev/random", std::fstream::in | std::fstream::binary);
  //       fs.read( (char*)&buffer[0], 1100);
  //       fs.close();

	// 	//uint8_t buffer;
	// 	NS_LOG_FUNCTION (this);
	// 	NS_ASSERT (m_sendTrain2.IsExpired ());
	// 	SeqTsHeader seqTs;
	// 	seqTs.SetSeq (m_sent);
	// 	Ptr<Packet> p = Create<Packet> (&buffer[0], m_size-(8+4)); // 8+4 : the size of the seqTs header
	// 	p->AddHeader (seqTs);
	 
	// 	std::stringstream peerAddressStringStream;
	// 	if (Ipv4Address::IsMatchingType (m_peerAddress))
	// 		{
	// 			peerAddressStringStream << Ipv4Address::ConvertFrom (m_peerAddress);
	// 		}
	// 	else if (Ipv6Address::IsMatchingType (m_peerAddress))
	// 		{
	// 			peerAddressStringStream << Ipv6Address::ConvertFrom (m_peerAddress);
	// 		}
	 
	// 	if ((m_socket->Send (p)) >= 0)
	// 		{
	// 			++m_sent;
	// 			NS_LOG_INFO (" Sending random packet: " << m_sent%6000);
	 
	// 		}
	// 	else
	// 		{
	// 			NS_LOG_INFO ("Error while sending " << m_size << " bytes to "
	// 													<< peerAddressStringStream.str ());
	// 		}
	 
	// 	//don't divide by 2 this time, because we're sending the remaining half
	// 		//cleaner way might be:
	// 		//m_sent%6000 == 0
	// 	if (m_sent < m_count)
	// 		{
	// 			m_sendTrain2 = Simulator::Schedule (m_interval, &SpqApplicationClient::SendRandomTrain, this);
	// 		}
	// }
 
} // Namespace ns3
