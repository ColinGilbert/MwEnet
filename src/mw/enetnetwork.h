#ifndef MW_ENETNETWORK_H
#define MW_ENETNETWORK_H

#include "network.h"
#include "packet.h"

#include <enet/enet.h>

#include <queue>

namespace mw {

	// An abstract class.
	class EnetNetwork : public Network {
	public:
		EnetNetwork();
		virtual ~EnetNetwork();

		void pushToSendBuffer(const Packet& packet, PacketType type, int toId) override;

		void pushToSendBuffer(const Packet& packet, PacketType type) override;

		int pullFromReceiveBuffer(Packet& data) override;

	protected:
		enum EnetConnectionType {
			CONNECT_INFO = 0, PACKET = 1
		};

		class InternalPacket {
		public:
			InternalPacket(Packet packet, int id, PacketType type) {
				data_ = packet;
				fromId_ = id;
				type_ = type;
				toId_ = 0;
			}

			InternalPacket(Packet packet, int id, PacketType type, int toId) {
				data_ = packet;
				fromId_ = id;
				type_ = type;
				toId_ = toId;
			}

			Packet data_;
			int fromId_;
			PacketType type_;
			int toId_;
		};

		virtual InternalPacket receive(ENetEvent eNetEvent) = 0;

		// Sends data packet from client width the corresponding id.
		// 0 char type    |	EnetNetwork type.
		// 1 char id      |
		// 2 char data[N] |
		ENetPacket* createEnetPacket(const Packet& dataPacket, char fromId, PacketType type) const;

		std::queue<InternalPacket> sendPackets_;
		std::queue<InternalPacket> receivePackets_;

		bool acceptNewNetworkConnections_;

		static int nbrOfInstances;
	};

} // Namespace mw.

#endif // MW_ENETNETWORK_H