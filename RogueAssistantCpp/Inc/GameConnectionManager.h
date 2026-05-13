#pragma once
#include "Defines.h"
#include "SFML/Network.hpp"
#include "GameDataRequest.h"
#include <memory>
#include <thread>
#include <queue>

class GameConnection;
class GameConnectionManager;

struct GameDataRequest;

typedef std::shared_ptr<GameConnection> GameConnectionRef;

struct ActiveGameConnection
{
	GameConnectionRef m_Game;
	std::thread m_UpdateThread;
};

class GameConnectionManager
{
public:
	GameConnectionManager() {};

	static GameConnectionManager& Instance();
	static bool IsValid();

	void OpenListener();
	void CloseListener();

	void UpdateConnections();
	void EnqueueGameDataRequest(GameDataRequest const& request);
	bool TryPopDataRequest(GameDataRequest& target);

	inline bool AnyConnectionsActive() const { return !m_ActiveConnections.empty(); }
	inline int ActiveConnectionCount() const { return (int)m_ActiveConnections.size(); }

	inline ActiveGameConnection& GetGameConnectionAt(int index) { return m_ActiveConnections[index]; }

	inline void PushError(std::string const& error) { m_RecentError = error; }
	inline void ClearRecentError() { m_RecentError = ""; }
	inline std::string const& GetRecentError() const { return m_RecentError; }

private:
	void BackgroundUpdate(GameConnectionRef game);

	std::string m_RecentError;
	std::queue<GameDataRequest> m_PendingDataRequests;
	bool m_ListeningForConnections = false;

	std::vector<ActiveGameConnection> m_ActiveConnections;
	GameConnectionRef m_AcceptingConnection;
};