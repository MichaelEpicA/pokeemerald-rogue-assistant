#include "UI/PrimaryUI.h"
#include "UI/Window.h"
#include "Assets.h"
#include "GameConnectionManager.h"
#include "Log.h"
#include "UserData.h"

#include <fstream>
#include <stdlib.h>
#include <string>
#include <thread>
#include <vector>

#include <Windows.h>

#pragma warning(disable: 4244)

bool RogueAssistant_MainLoop(Window* window, void* userData);
void RogueAssistant_StubFunc();
void RogueAssistant_ThreadFunc();

static void DumpScriptsNextToExe()
{
    {
        std::ofstream fileStream;
        fileStream.open("RogueAssistant_mGBA.lua", std::ios::out);

        LOG_INFO("Dumping RogueAssistant_mGBA.lua next to exe");

        auto const& data = bin2cpp::getRogueAssistant_mGBALuaFile();
        char const* ptr = data.getBuffer();

        for (; *ptr != 0; ++ptr)
        {
            // Ignore carriage return
            if (*ptr != '\r')
                fileStream << *ptr;
        }

        fileStream.close();
    }

    // TODO - Move assets around and only ship exe file
}

static std::unique_ptr<std::thread> s_BackgroundThread;
static bool s_CloseRequested = false;

#if defined(_WIN32)
#define EXPORT __declspec(dllexport)
#else
#define EXPORT __attribute__((visibility("default")))
#endif
EXPORT int RogueAssistant_Main(bool isStub, std::vector<std::string> const& args)
{
    if (isStub)
    {
        DumpScriptsNextToExe();
        UserData::Init();

        RogueAssistant_StubFunc();
        //RogueAssistant_ThreadFunc()
    }
    else
    {
        UserData::Init();

        s_BackgroundThread = std::make_unique<std::thread>(RogueAssistant_ThreadFunc);
    }

    return 0;
}

void RogueAssistant_Frame()
{
}

void RogueAssistant_Shutdown()
{
    if (s_BackgroundThread)
    {
        s_CloseRequested = true;
        s_BackgroundThread->join();
        s_BackgroundThread = nullptr;
    }
}

struct WindowData
{
    Window m_Window;
    PrimaryUI m_UI;
};

void RogueAssistant_StubFunc()
{
    WindowConfig config;
    config.title = "Rogue Assistant";
    config.canBeDestroyed = true;

    WindowData data =
    {
        Window(config),
        PrimaryUI()
    };
    data.m_UI.SetToStubTheme();

    if (data.m_Window.Create())
    {
        data.m_Window.EnterMainLoop(RogueAssistant_MainLoop, &data);
        data.m_Window.Destroy();
    }
}

void RogueAssistant_ThreadFunc()
{
    #if defined(_WIN32)
    SetThreadDescription(GetCurrentThread(), L"RogueAssistant");
    #endif
    WindowConfig config;
    config.title = "Rogue Assistant";
    config.imGuiEnabled = false;
    config.canBeDestroyed = false;

    WindowData data =
    {
        Window(config),
        PrimaryUI()
    };

    if (data.m_Window.Create())
    {
        GameConnectionManager::Instance().OpenListener();

        data.m_Window.EnterMainLoop(RogueAssistant_MainLoop, &data);

        if (data.m_Window.Destroy())
        {
            GameConnectionManager::Instance().CloseListener();
        }
    }
}

bool RogueAssistant_MainLoop(Window* window, void* userData)
{
    WindowData* data = (WindowData*)userData;
    UserData::Update();
    GameConnectionManager::Instance().UpdateConnections();
    data->m_UI.Render(*window);

    if (s_CloseRequested)
        return false;

    return true;
}