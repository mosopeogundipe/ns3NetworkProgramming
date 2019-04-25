/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

 /*
	Authors:
		Ian Granger

	Purpose:
		Helper class to create the compression detection client 
		and server. Note, the name really should be changed.

	Date:
		2/25/19

	Todo:
	
*/
#include "drr-application-helper.h"
#include "ns3/compression-detection-server.h"
#include "ns3/compression-detection-client.h"
#include "ns3/uinteger.h"
#include "ns3/string.h"
 
namespace ns3 {
 
DrrServerHelper::DrrServerHelper ()
{
	m_factory.SetTypeId (DrrApplicationServer::GetTypeId ());
}
 
DrrServerHelper::DrrServerHelper (uint16_t port)
{
	m_factory.SetTypeId (DrrApplicationServer::GetTypeId ());
	SetAttribute ("Port", UintegerValue (port));
}
 
void
DrrServerHelper::SetAttribute (std::string name, const AttributeValue &value)
{
	m_factory.Set (name, value);
}
 
ApplicationContainer
DrrServerHelper::Install (NodeContainer c)
{
	ApplicationContainer apps;
	for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
		{
			Ptr<Node> node = *i;
 
			m_server = m_factory.Create<DrrApplicationServer> ();
			node->AddApplication (m_server);
			apps.Add (m_server);
 
		}
	return apps;
}
 
Ptr<DrrApplicationServer>
DrrServerHelper::GetServer (void)
{
	return m_server;
}
 
DrrClientHelper::DrrClientHelper ()
{
	m_factory.SetTypeId (DrrApplicationClient::GetTypeId ());
}
 
DrrClientHelper::DrrClientHelper (Address address, uint16_t port)
{
	m_factory.SetTypeId (DrrApplicationClient::GetTypeId ());
	SetAttribute ("RemoteAddress", AddressValue (address));
	SetAttribute ("RemotePort", UintegerValue (port));
}
 
DrrClientHelper::DrrClientHelper (Address address)
{
	m_factory.SetTypeId (DrrApplicationClient::GetTypeId ());
	SetAttribute ("RemoteAddress", AddressValue (address));
}
 
void
DrrClientHelper::SetAttribute (std::string name, const AttributeValue &value)
{
	m_factory.Set (name, value);
}
 
ApplicationContainer
DrrClientHelper::Install (NodeContainer c)
{
	ApplicationContainer apps;
	for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
		{
			Ptr<Node> node = *i;
			Ptr<DrrApplicationClient> client = m_factory.Create<DrrApplicationClient> ();
			node->AddApplication (client);
			apps.Add (client);
		}
	return apps;
}
 

 } // namespace ns3
