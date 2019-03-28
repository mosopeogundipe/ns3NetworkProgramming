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
#include "dest-addr-packet-filter.h"
#include "ns3/inet-socket-address.h"
#include "ns3/ipv4-address.h"
#include "ns3/ipv4-header.h"
#include "ns3/packet.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("DestAddrPacketFilter");

NS_OBJECT_ENSURE_REGISTERED (DestAddrPacketFilter);

TypeId 
DestAddrPacketFilter::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::DestAddrPacketFilter")
    .SetParent<Object> ()
    .SetGroupName ("TrafficControl")
  ;
  return tid;
}

DestAddrPacketFilter::DestAddrPacketFilter (Ipv4Address addr)
{
  NS_LOG_FUNCTION (this);
	address = addr;
}

DestAddrPacketFilter::~DestAddrPacketFilter()
{
  NS_LOG_FUNCTION (this);
}

int32_t
DestAddrPacketFilter::Classify (Ptr<Packet> packet) const
{
  NS_LOG_FUNCTION (this << packet);

  if (!CheckProtocol (packet))
  {
    NS_LOG_LOGIC ("Unable to classify packets of this protocol");
    return PF_NO_MATCH;
  }

  return DoClassify (packet);
}

bool
DestAddrPacketFilter::CheckProtocol (Ptr<Packet> packet) const
{
	// Gonna go ahead and assume true because idk how to check if
	// a packet has a ipv4 address
	return true;
}

int32_t
DestAddrPacketFilter::DoClassify (Ptr<Packet> packet) const
{
	ns3::Ipv4Header ipHeader;
	Ipv4Address compareAddr;

	packet->PeekHeader (ipHeader);
	compareAddr = ipHeader.GetDestination ();

	if (address.IsEqual (compareAddr))
		{
			return 1;
		}

	return -1;
}

} // namespace ns3
