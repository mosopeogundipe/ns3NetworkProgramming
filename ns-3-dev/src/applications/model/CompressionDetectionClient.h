/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

 /*
 	Authors:
 		Ian Granger

	Purpose:

	Date:
		2/25/19

	Todo:
	
*/

#ifndef COMPRESSIONDETECTIONCLIENT_H
#define COMPRESSIONDETECTIONCLIENT_H


#include "ns3/application.h"
#include "ns3/event-id.h"
#include "ns3/ptr.h"
#include "ns3/ipv4-address.h"

//Do I need to include UdpClient

namespace ns3 {
 
	class Socket;
	class Packet;

	class CompressionDetectionClient : public Application
	{
	public:
		static TypeId GetTypeId(void);

		CompressionDetectionClient();

		virtual ~CompressionDetectionClient();

		void SetRemote(Address ip, uint16_t port);
		void SetRemote(Address addr);

	protected:
		virtual void DoDispose(void);

	private:

		virtual void StartApplication(void);
		virtual void StopApplication(void);

		void SendEmptyTrain(void);
		void SendRandomTrain(void);

		uint32_t m_count; 
		Time m_interval; 
		uint32_t m_size; 

		uint32_t m_sent; 
		Ptr<Socket> m_socket; 
		Address m_peerAddress; 
		uint16_t m_peerPort; 

		//probably seed two sendEvents, one for each train
		EventId m_sendTrain1;
		EventId m_sendTrain2;  

	};

}

#endif /*  COMPRESSIONDETECTIONCLIENT_H */
 