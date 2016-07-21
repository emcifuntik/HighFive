#include "stdafx.h"

CNetworkConnection *CNetworkConnection::singleInstance = nullptr;

CNetworkConnection::CNetworkConnection()
{
	client = RakNet::RakPeerInterface::GetInstance();
}

CNetworkConnection::~CNetworkConnection()
{
	CLocalPlayer::Get()->SetMoney(0);
}

CNetworkConnection * CNetworkConnection::Get()
{
	if (!singleInstance)
		singleInstance = new CNetworkConnection();
	return singleInstance;
}

bool CNetworkConnection::Connect(std::string host, unsigned short port)
{
	if (!host.empty() && port)
	{
		RakNet::SocketDescriptor socketDescriptor(0, 0);

		socketDescriptor.socketFamily = AF_INET;
		client->Startup(8, &socketDescriptor, 1);
		client->SetOccasionalPing(true);

		connection = client->Connect(host.c_str(), port, "", 0);
		RakAssert(connection == RakNet::CONNECTION_ATTEMPT_STARTED);
		bConnected = true;
		return true;
	}

	return false;
}

void CNetworkConnection::Disconnect()
{
	client->Shutdown(300);
	CChat::Get()->Clear();
	CLocalPlayer::Get()->ShowNotification("~b~Disconnected");
}

void CNetworkConnection::Tick()
{
	for (packet = client->Receive(); packet; client->DeallocatePacket(packet), packet = client->Receive()) {
		unsigned char packetID = packet->data[0];
		RakNet::BitStream bsIn(packet->data, packet->length, false);
		RakNet::BitStream bsOut;
		bsIn.IgnoreBytes(sizeof(unsigned char));

		switch (packetID) {
			case ID_CONNECTION_REQUEST_ACCEPTED:
			{
				CLocalPlayer::Get()->FreezePosition(false);
				CLocalPlayer::Get()->SetVisible(true);
				RakString playerName("Funtik");
				bsOut.Write((unsigned char)ID_CONNECT_TO_SERVER);
				bsOut.Write(playerName);
				CLocalPlayer::Get()->SetMoney(0);
				CLocalPlayer::Get()->SetCoordsKeepVehicle(0.0, 0.0, 73.5);
				client->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
				break;
			}
			case ID_CONNECTION_ATTEMPT_FAILED:
			{
				CLocalPlayer::Get()->SetMoney(0);
				CLocalPlayer::Get()->ShowNotification("~r~Not connected");
				break;
			}
			case ID_NO_FREE_INCOMING_CONNECTIONS:
			{
				CLocalPlayer::Get()->SetMoney(0);
				CLocalPlayer::Get()->ShowNotification("~r~Server is full!");
				break;
			}
			case ID_DISCONNECTION_NOTIFICATION:
			{
				CLocalPlayer::Get()->SetMoney(0);

				CLocalPlayer::Get()->ShowNotification("~r~Connection closed!");
				break;
			}
			case ID_CONNECTION_LOST:
			{
				CLocalPlayer::Get()->SetMoney(0);
				CLocalPlayer::Get()->ShowNotification("~r~Connection Lost!");
				break;
			}
			case ID_CONNECTION_BANNED:
			{
				CLocalPlayer::Get()->SetMoney(0);
				CLocalPlayer::Get()->ShowNotification("~r~You are banned!");
				break;
			}
			case ID_CONNECT_TO_SERVER:
			{
				bEstablished = true;
				break;
			}
			case ID_SEND_PLAYER_DATA:
			{
				RakNet::BitStream bsIn(packet->data, packet->length, false);
				bsIn.IgnoreBytes(sizeof(unsigned char));
				OnFootSyncData data;
				RakNet::RakNetGUID playerGUID;
				bsIn.Read(playerGUID);
				bsIn.Read(data);
				CNetworkPlayer *remotePlayer = CNetworkPlayer::GetByGUID(playerGUID);
				remotePlayer->SetOnFootData(data, 100);
				break;
			}
			case ID_SEND_VEHICLE_DATA:
			{
				break;
			}
			case ID_PLAYER_LEFT:
			{
				break;
			}
			default:
			{
				std::stringstream ss;
				ss << "[RakNet] Unknown message id: " << (int)packet->data[0] << ", message: " << packet->data;
				CChat::Get()->AddChatMessage(ss.str(), { 255, 100, 100, 255 });
				break;
			}
		}
	}
}
