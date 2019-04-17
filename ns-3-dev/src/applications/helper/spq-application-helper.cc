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
#include "compression-detection-client-server-helper.h"
#include "ns3/compression-detection-server.h"
#include "ns3/compression-detection-client.h"
#include "ns3/uinteger.h"
#include "ns3/string.h"
 
namespace ns3 {
 
SpqServerHelper::SpqServerHelper ()
{
	m_factory.SetTypeId (CompressionDetectionServer::GetTypeId ());
}
 
SpqServerHelper::SpqServerHelper (uint16_t port)
{
	m_factory.SetTypeId (CompressionDetectionServer::GetTypeId ());
	SetAttribute ("Port", UintegerValue (port));
}
 
void
SpqServerHelper::SetAttribute (std::string name, const AttributeValue &value)
{
	m_factory.Set (name, value);
}
 
ApplicationContainer
SpqServerHelper::Install (NodeContainer c)
{
	ApplicationContainer apps;
	for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
		{
			Ptr<Node> node = *i;
 
			m_server = m_factory.Create<CompressionDetectionServer> ();
			node->AddApplication (m_server);
			apps.Add (m_server);
 
		}
	return apps;
}
 
Ptr<CompressionDetectionServer>
SpqServerHelper::GetServer (void)
{
	return m_server;
}
 
SpqClientHelper::SpqClientHelper ()
{
	m_factory.SetTypeId (CompressionDetectionClient::GetTypeId ());
}
 
SpqClientHelper::SpqClientHelper (Address address, uint16_t port)
{
	m_factory.SetTypeId (CompressionDetectionClient::GetTypeId ());
	SetAttribute ("RemoteAddress", AddressValue (address));
	SetAttribute ("RemotePort", UintegerValue (port));
}
 
SpqClientHelper::SpqClientHelper (Address address)
{
	m_factory.SetTypeId (CompressionDetectionClient::GetTypeId ());
	SetAttribute ("RemoteAddress", AddressValue (address));
}
 
void
SpqClientHelper::SetAttribute (std::string name, const AttributeValue &value)
{
	m_factory.Set (name, value);
}
 
ApplicationContainer
SpqClientHelper::Install (NodeContainer c)
{
	ApplicationContainer apps;
	for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
		{
			Ptr<Node> node = *i;
			Ptr<CompressionDetectionClient> client = m_factory.Create<CompressionDetectionClient> ();
			node->AddApplication (client);
			apps.Add (client);
		}
	return apps;
}
 

 } // namespace ns3
