/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright 2007 University of Washington
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "ns3/log.h"
#include "ns3/ipv4-address.h"
#include "ns3/address-utils.h"
#include "ns3/nstime.h"
#include "ns3/inet-socket-address.h"
#include "ns3/socket.h"
#include "ns3/udp-socket.h"
#include "ns3/simulator.h"
#include "ns3/socket-factory.h"
#include "ns3/packet.h"
#include "ns3/uinteger.h"

#include "p2p-server.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("P2PServerApplication");

NS_OBJECT_ENSURE_REGISTERED (P2PServer);

TypeId
P2PServer::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::P2PServer")
    .SetParent<Application> ()
    .SetGroupName("Applications")
    .AddConstructor<P2PServer> ()
    .AddAttribute ("Port", "Port on which we listen for incoming packets.",
                   UintegerValue (9),
                   MakeUintegerAccessor (&P2PServer::m_port),
                   MakeUintegerChecker<uint16_t> ())
    .AddTraceSource ("Rx", "A packet has been received",
                     MakeTraceSourceAccessor (&P2PServer::m_rxTrace),
                     "ns3::Packet::TracedCallback")
    .AddTraceSource ("RxWithAddresses", "A packet has been received",
                     MakeTraceSourceAccessor (&P2PServer::m_rxTraceWithAddresses),
                     "ns3::Packet::TwoAddressTracedCallback")
  ;
  return tid;
}

P2PServer::P2PServer ()
{
  NS_LOG_FUNCTION (this);
}

P2PServer::~P2PServer()
{
  NS_LOG_FUNCTION (this);
  m_socket = 0;
}

void
P2PServer::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  Application::DoDispose ();
}

void 
P2PServer::StartApplication (void)
{
  NS_LOG_FUNCTION (this);

  if (m_socket == 0)
    {
      TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
      m_socket = Socket::CreateSocket (GetNode (), tid);
      InetSocketAddress local = InetSocketAddress (Ipv4Address::GetAny (), m_port);
      if (m_socket->Bind (local) == -1)
        {
          NS_FATAL_ERROR ("Failed to bind socket");
        }
      if (addressUtils::IsMulticast (m_local))
        {
          Ptr<UdpSocket> udpSocket = DynamicCast<UdpSocket> (m_socket);
          if (udpSocket)
            {
              // equivalent to setsockopt (MCAST_JOIN_GROUP)
              udpSocket->MulticastJoinGroup (0, m_local);
            }
          else
            {
              NS_FATAL_ERROR ("Error: Failed to join multicast group");
            }
        }
    }


  m_socket->SetRecvCallback (MakeCallback (&P2PServer::HandleRead, this));
}

void 
P2PServer::StopApplication ()
{
  NS_LOG_FUNCTION (this);

  if (m_socket != 0) 
    {
      m_socket->Close ();
      m_socket->SetRecvCallback (MakeNullCallback<void, Ptr<Socket> > ());
    }
}

void 
P2PServer::HandleRead (Ptr<Socket> socket)
{
  NS_LOG_FUNCTION (this << socket);

  Ptr<Packet> packet;
  Address from;
  Address localAddress;
	uint32_t size;
	uint8_t *buf;
  while ((packet = socket->RecvFrom (from)))
    {
			size = packet->GetSize ();
			buf = new uint8_t[size];
			packet->CopyData (buf, size);
			std::cout << "server read packet with message: " << buf << std::endl;

      socket->GetSockName (localAddress);
      m_rxTrace (packet);
      m_rxTraceWithAddresses (packet, from, localAddress);

      packet->RemoveAllPacketTags ();
      packet->RemoveAllByteTags ();
    }
}

} // Namespace ns3
