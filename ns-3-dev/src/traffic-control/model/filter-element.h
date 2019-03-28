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

#ifndef FILTER_ELEMENT_H
#define FILTER_ELEMENT_H

#include "ns3/object.h"

namespace ns3 {

class QueueDiscItem;

/**
 * \ingroup traffic-control
 *
 * FilterElement is the abstract base class for filters used by queue discs
 * to classify packets.
 */
class FilterElement: public Object {
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);

  FilterElement ();
  virtual ~FilterElement ();

  /**
   * \brief match a packet to given specifications
   *
   * \param packet the packet to classify.
   *
   * \return True if packet matches, False otherwise
   */
  virtual bool Match (Ptr<Packet> packet) const;

private:
};

} // namespace ns3

#endif /* FILTER_ELEMENT */
