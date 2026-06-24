#include "GameConnectionManager.h"
#include "GameConnection.h"
#include "GameDataRequest.h"

#include "Log.h"

#if defined(_WIN32)
#include <WinSock2.h>
#endif

#include <SFML/Network.hpp>

static std::unique_ptr<GameConnectionManager> s_Manager;

GameConnectionManager& GameConnectionManager::Instance()
{
	if (s_Manager == nullptr)
	{
		s_Manager = std::make_unique<GameConnectionManager>();
	}
	return *s_Manager;
}

bool GameConnectionManager::IsValid()
{
	return s_Manager != nullptr;
}

void GameConnectionManager::OpenListener()
{
	LOG_INFO("Game: Opening connection listener");
	m_ListeningForConnections = true;
}

void GameConnectionManager::CloseListener()
{
	LOG_INFO("Game: Closing connection listener");
	m_ListeningForConnections = false;
}

void GameConnectionManager::UpdateConnections()
{
	// Accept any incoming connections
	if (m_AcceptingConnection == nullptr)
		m_AcceptingConnection = std::make_shared<GameConnection>();

	if (m_ListeningForConnections && m_ActiveConnections.empty())
	{
		LOG_INFO("Game: Incoming connection...");
		ClearRecentError();
		m_ListeningForConnections = false; // only 1 connection per emulator now

		GameConnectionRef gameConn = m_AcceptingConnection;
		m_AcceptingConnection = nullptr;

		ActiveGameConnection newConnection;
		newConnection.m_Game = gameConn;
		newConnection.m_UpdateThread = std::thread([this, gameConn]() { BackgroundUpdate(gameConn); });
		m_ActiveConnections.push_back(std::move(newConnection));
	}

	// Handle disconnections
	for (int i = 0; i < (int)m_ActiveConnections.size();)
	{
		if (m_ActiveConnections[i].m_Game->HasDisconnected())
		{
			LOG_INFO("Game: Connection disconnected");
			m_ActiveConnections[i].m_UpdateThread.join();
			m_ActiveConnections.erase(m_ActiveConnections.begin() + i);
		}
		else
			++i;
	}
}

void GameConnectionManager::EnqueueGameDataRequest(GameDataRequest const& request)
{
	m_PendingDataRequests.push(request);
}

bool GameConnectionManager::TryPopDataRequest(GameDataRequest& target)
{
	if (!m_PendingDataRequests.empty())
	{
		target = m_PendingDataRequests.front();
		m_PendingDataRequests.pop();
		return true;
	}

	return false;
}

void GameConnectionManager::BackgroundUpdate(GameConnectionRef game)
{
	// At max run at 30UPS for now
	while (!game->HasDisconnected())
	{
		game->Update();
		std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 30));
	}
}