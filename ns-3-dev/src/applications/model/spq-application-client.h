/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

 /*
 	Authors:
 		Ian Granger

	Purpose:


	Date:
		4/16/19

	Todo:
	
*/

#ifndef SPQ_APPLICATION_CLIENT_H
#define SPQ_APPLICATION_CLIENT_H


#include "ns3/application.h"
#include "ns3/event-id.h"
#include "ns3/ptr.h"
#include "ns3/ipv4-address.h"

//Do I need to include UdpClient

namespace ns3 {
 
	class Socket;
	class Packet;

	class SpqApplicationClient : public Application
	{
	public:
		static TypeId GetTypeId(void);

		SpqApplicationClient();

		virtual ~SpqApplicationClient();

		void SetRemote(Address ip, uint16_t port);
		void SetRemote(Address addr);

	protected:
		virtual void DoDispose(void);

	private:

		virtual void StartApplication(void);
		virtual void StopApplication(void);

		void SendEmptyTrain(void);
		void SendRandomTrain(void);
		void Send(void);
		void createLowEntropyPackets (uint8_t*  buffer, uint32_t m_size);
		void createHighEntropyPackets (uint8_t* buffer, uint32_t m_size);

		uint32_t m_count; 
		Time m_interval; 
		uint32_t m_size;
		bool m_set_entropy; 

		uint32_t m_sent; 
		Ptr<Socket> m_socket; 
		Address m_peerAddress; 
		uint16_t m_peerPort; 

		//probably seed two sendEvents, one for each train
		EventId m_sendTrain1;
		EventId m_sendTrain2; 
		EventId m_sendEvent;  

	};

}

#endif /*  SpqApplicationClient_H */
 