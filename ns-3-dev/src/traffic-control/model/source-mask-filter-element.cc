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
#include "source-mask-filter-element.h"
#include "ns3/inet-socket-address.h"
#include "ns3/ipv4-address.h"
#include "ns3/ipv4-header.h"
#include "ns3/packet.h"
#include "ns3/filter-element.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("SourceMaskFilterElement");

NS_OBJECT_ENSURE_REGISTERED (SourceMaskFilterElement);

TypeId 
SourceMaskFilterElement::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::SourceMaskFilterElement")
    .SetParent<FilterElement> ()
    .SetGroupName ("TrafficControl")
  ;
  return tid;
}

SourceMaskFilterElement::SourceMaskFilterElement (Ipv4Address startAddr, Ipv4Address endAddr)
{
  NS_LOG_FUNCTION (this);
	startAddress = startAddr;
	startBits = startAddr.Get ();
	endAddress = endAddr;
	endBits = endAddr.Get ();
}

SourceMaskFilterElement::~SourceMaskFilterElement()
{
  NS_LOG_FUNCTION (this);
}

bool
SourceMaskFilterElement::Match (Ptr<Packet> packet) const
{
  NS_LOG_FUNCTION (this);

	uint32_t compareBits;
	ns3::Ipv4Header ipHeader;
	Ipv4Address compareAddr;

	packet->PeekHeader (ipHeader);
	compareAddr = ipHeader.GetSource ();
	compareBits = compareAddr.Get ();

	return (compareBits >= startBits && compareBits < endBits);
}

} // namespace ns3
