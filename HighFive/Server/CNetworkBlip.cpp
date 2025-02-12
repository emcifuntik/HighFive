#include "stdafx.h"

std::vector<CNetworkBlip *> CNetworkBlip::GlobalBlips;
std::vector<CNetworkBlip *> CNetworkBlip::PlayerBlips;

CNetworkBlip::CNetworkBlip(float x, float y, float z, float scale, int color, int sprite, int playerid)
{
	RakNet::BitStream bsOut;

	rnGUID = RakNetGUID::RakNetGUID(rand());

	bsOut.Write(rnGUID);
	bsOut.Write(x);
	bsOut.Write(y);
	bsOut.Write(z);
	bsOut.Write(scale);
	bsOut.Write(color);
	bsOut.Write(sprite);

	vecPos = CVector3(x, y, z);
	this->scale = scale;
	this->color = color;
	this->sprite = sprite;
	this->playerid = playerid;

	if (playerid == -1)
	{
		CRPCPlugin::Get()->Signal("CreateBlip", &bsOut, HIGH_PRIORITY, RELIABLE_SEQUENCED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true, false);
		GlobalBlips.push_back(this);
	} else {
		RakNetGUID guid = CNetworkPlayer::GetByID(playerid)->GetGUID();
		CRPCPlugin::Get()->Signal("CreateBlip", &bsOut, HIGH_PRIORITY, RELIABLE_SEQUENCED, 0, guid, false, false);
		PlayerBlips.push_back(this);
	}
	
}

void CNetworkBlip::SetScale(float _scale)
{
	scale = _scale;
}

void CNetworkBlip::Delete()
{
}


CNetworkBlip::~CNetworkBlip()
{

}

void CNetworkBlip::SendGlobal(RakNet::Packet *packet)
{
	for each(auto *blip in GlobalBlips)
	{
		RakNet::BitStream bsOut;

		bsOut.Write(blip->rnGUID);
		bsOut.Write(blip->vecPos.fX);
		bsOut.Write(blip->vecPos.fY);
		bsOut.Write(blip->vecPos.fZ);
		bsOut.Write(blip->scale);
		bsOut.Write(blip->color);
		bsOut.Write(blip->sprite);

		CRPCPlugin::Get()->Signal("CreateBlip", &bsOut, HIGH_PRIORITY, RELIABLE_SEQUENCED, 0, packet->guid, false, false);
	}
}

CNetworkBlip * CNetworkBlip::GetByGUID(RakNetGUID guid)
{
	for each (CNetworkBlip *blip in GlobalBlips)
		if (blip && blip->rnGUID == guid)
			return blip;

	for each (CNetworkBlip *blip in PlayerBlips)
		if (blip && blip->rnGUID == guid)
			return blip;

	return nullptr;
}

std::vector<CNetworkBlip *> CNetworkBlip::AllGlobal()
{
	return GlobalBlips;
}

