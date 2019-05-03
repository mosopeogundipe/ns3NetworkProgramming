/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2016 Universita' degli Studi di Napoli Federico II
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
#include "ns3/integer.h"
#include "queue-disc.h"
#include "dest-port-filter-element.h"
#include "ns3/inet-socket-address.h"
#include "ns3/ipv4-address.h"
#include "ns3/ppp-header.h"
#include "ns3/ipv4-header.h"
#include "ns3/tcp-header.h"
#include "ns3/packet.h"
#include "ns3/filter-element.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("DestPortFilterElement");

NS_OBJECT_ENSURE_REGISTERED (DestPortFilterElement);

TypeId 
DestPortFilterElement::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::DestPortFilterElement")
    .SetParent<FilterElement> ()
    .SetGroupName ("TrafficControl")
  ;
  return tid;
}

DestPortFilterElement::DestPortFilterElement (uint32_t portNumber)
{
  NS_LOG_FUNCTION (this);
	port = portNumber;
}

DestPortFilterElement::~DestPortFilterElement()
{
  NS_LOG_FUNCTION (this);
}

bool
DestPortFilterElement::Match (Ptr<Packet> packet) const
{
  NS_LOG_FUNCTION (this);

	ns3::PppHeader pppHeader;
	ns3::Ipv4Header ipHeader;
	ns3::TcpHeader tcpHeader;
  bool value = false;

	packet->RemoveHeader (pppHeader);
	packet->RemoveHeader (ipHeader);
	packet->RemoveHeader (tcpHeader);
  //std::cout << "Packet Port is: " << tcpHeader.GetDestinationPort () << std::endl;
  //std::cout << "Filter element Port is: " << port << std::endl;
  if(tcpHeader.GetDestinationPort () == port){
    value = true;
  }
  packet->AddHeader (tcpHeader);
  packet->AddHeader (ipHeader);
  packet->AddHeader (pppHeader);
  return value;
}

} // namespace ns3
