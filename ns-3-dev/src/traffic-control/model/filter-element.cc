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
#include "filter-element.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("FilterElement");

NS_OBJECT_ENSURE_REGISTERED (FilterElement);

TypeId 
FilterElement::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::FilterElement")
    .SetParent<Object> ()
    .SetGroupName ("TrafficControl")
		.AddConstructor<FilterElement> ()
  ;
  return tid;
}

FilterElement::FilterElement ()
{
  NS_LOG_FUNCTION (this);
}

FilterElement::~FilterElement()
{
  NS_LOG_FUNCTION (this);
}

bool
FilterElement::Match (Ptr<Packet> packet) const
{
	return false;
}

} // namespace ns3
