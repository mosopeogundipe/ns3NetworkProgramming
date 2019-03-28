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

#ifndef SOURCE_ADDR_PACKET_FILTER_H
#define SOURCE_ADDR_PACKET_FILTER_H

#include "ns3/object.h"
#include "ns3/inet-socket-address.h"
#include "ns3/ipv4-address.h"
#include "ns3/packet.h"

namespace ns3 {

class QueueDiscItem;

/**
 * \ingroup traffic-control
 *
 */
class SourceAddrPacketFilter: public Object {
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);

  SourceAddrPacketFilter (Ipv4Address address);
  virtual ~SourceAddrPacketFilter ();

  /**
   * Standard value used by packet filters to indicate that no match was possible.
   */
  static const int PF_NO_MATCH = -1;

  /**
   * \brief Classify a packet.
   *
   * \param item the packet to classify.
   *
   * \return -1 if this filter is not able to classify packets of the same protocol
   * as item or the item does not match the filter conditions, the configured return
   * value otherwise.
   */
  int32_t Classify (Ptr<Packet> packet) const;

private:
	Ipv4Address address;

  /**
   * \brief Checks if the filter is able to classify a kind of items.
   *
   * \param item an example item to check.
   * \return true if this filter is able to classify packets.
   */
  bool CheckProtocol (Ptr<Packet> packet) const;

  /**
   * \brief Classify a packet.
   *
   * \param item the packet to classify.
   *
   * \return -1 if the item does not match the filter conditions, or the configured
   * return value otherwise.
   */
  int32_t DoClassify (Ptr<Packet> packet) const;
};

} // namespace ns3

#endif /* SOURCE_ADDR_PACKET_FILTER */
