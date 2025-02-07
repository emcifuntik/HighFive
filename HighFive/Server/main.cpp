#include "stdafx.h"

struct pass
{
	template<typename ...T> pass(T...) {}
};

int main(void)
{
	log << "Starting the server..." << std::endl;
	log << "Hostname: " << color::lred << CConfig::Get()->Hostname << std::endl;
	log << "Port: " << color::lred << CConfig::Get()->Port << std::endl;
	log << "Maximum players: " << color::lred << CConfig::Get()->MaxPlayers << std::endl;

	Plugin::LoadPlugins();	

	CHTTPServer::Get()->Start(88);
	CHTTPHandler h_a;
	CHTTPServer::g_server.addHandler("", h_a);

	new CNetworkVehicle(API::Get()->Hash("t20"), 5, 10, 72, 0);
	new CNetworkVehicle(API::Get()->Hash("shotaro"), 10, 10, 72, 0);
	new CNetworkVehicle(API::Get()->Hash("adder"), 15, 10, 72, 0);
	new CNetworkVehicle(API::Get()->Hash("besra"), 35, 10, 72, 0);
	new CNetworkVehicle(API::Get()->Hash("swift"), 45, 5, 72, 0);
	new CNetworkVehicle(API::Get()->Hash("jetmax"), -810, -1500, 5, 0);

	new CNetworkVehicle(API::Get()->Hash("lazer"), -966, -2987, 14, 0);
	new CNetworkVehicle(API::Get()->Hash("lazer"), -986, -2987, 14, 0);
	new CNetworkVehicle(API::Get()->Hash("lazer"), -946, -2987, 14, 0);
	
	auto netLoop = [=]()
	{
		CNetworkConnection::Get()->Start(CConfig::Get()->MaxPlayers, CConfig::Get()->Port);
		CRPCPlugin::Get();
		DWORD lastTick = 0;
		RakNet::RakNetStatistics stat;
		for (;;)
		{
			RakSleep(5);
			CNetworkConnection::Get()->Tick();
			CNetworkPlayer::Tick();
			Plugin::Tick();

			if ((GetTickCount() - lastTick) > 100)
			{
				CNetworkConnection::Get()->server->GetStatistics(0, &stat);
				std::stringstream ss;
				ss << CConfig::Get()->Hostname << ". Players online: " << CNetworkPlayer::Count() << ", "
					<< "Packet loss: " << std::setprecision(2) << std::fixed << stat.packetlossTotal * 100 << "%";
				SetConsoleTitle(ss.str().c_str());
				lastTick = GetTickCount();
			}
		}
	};
	std::thread netThread(netLoop);
	netThread.detach();

	for (;;)
	{
		std::string msg;
		std::getline(std::cin, msg);
		if (!msg.compare("exit") || !msg.compare("kill"))
		{
			log << "Terminating server..." << std::endl;
			break;
		}
		else
		{
			Plugin::ServerCommand(msg);
		}
	}
	netThread.~thread();
	//delete Python::Get();
	return 0;
}