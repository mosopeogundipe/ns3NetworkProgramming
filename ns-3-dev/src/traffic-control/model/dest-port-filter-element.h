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

#ifndef DEST_PORT_FILTER_ELEMENT_H
#define DEST_PORT_FILTER_ELEMENT_H

#include "ns3/object.h"
#include "ns3/inet-socket-address.h"
#include "ns3/ipv4-address.h"
#include "ns3/packet.h"
#include "ns3/filter-element.h"

namespace ns3 {

/**
 * \ingroup traffic-control
 *
 */
class DestPortFilterElement: public FilterElement {
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);

  DestPortFilterElement (uint32_t portNumber);
  virtual ~DestPortFilterElement ();

  /**
   * \brief Classify a packet.
   *
   * \param item the packet to classify.
   *
   * \return -1 if this filter is not able to classify packets of the same protocol
   * as item or the item does not match the filter conditions, the configured return
   * value otherwise.
   */
  bool Match (Ptr<Packet> packet) const;

private:
	uint32_t port;
};

} // namespace ns3

#endif /* DEST_PORT_FILTER_ELEMENT */
