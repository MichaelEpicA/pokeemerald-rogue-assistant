#include "Defines.h"
#include "Log.h"
#include <vector>
#include <string>

#define WIN32_LEAN_AND_MEAN

#include "GameConnectionManager.h"

int RogueAssistant_Main(bool isStub, std::vector<std::string> const& args);
void RogueAssistant_Frame();
void RogueAssistant_Shutdown();

#ifdef __cplusplus
extern "C"
{
#endif

#define LUA_LIB

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include <cstring>

    // Can't call lua directly from C for some reason, so work around that by having the lua call US
    // Example https://www.cs.usfca.edu/~galles/cs420/lecture/LuaLectures/LuaAndC.html
    //void do_test_print(lua_State* lua)
    //{
    //    // Push the fib function on the top of the lua stack
    //    lua_getglobal(lua, "onDoTest");
    //
    //    // Push the argument (the number 13) on the stack 
    //    //lua_pushnumber(lua, 234);
    //
    //    // call the function with 1 argument, returning a single result.  Note that the function actually
    //    // returns 2 results -- we just want one of them.  The second result will *not* be pushed on the
    //    // lua stack, so we don't need to clean up after it
    //    lua_call(lua, 0, 1);
    //
    //    // Get the result from the lua stack
    //    //int result = (int)lua_tointeger(lua, -1);
    //
    //    // Clean up.  If we don't do this last step, we'll leak stack memory.
    //    lua_pop(lua, 1);
    //}

    GameDataRequest s_RecentReq;
    std::vector<u8> s_Response;
    size_t s_WriteIndex = 0;

    int rogue_next_data_request(lua_State* lua)
    {
        if (GameConnectionManager::IsValid() && GameConnectionManager::Instance().TryPopDataRequest(s_RecentReq))
        {
            s_Response.clear();
            s_WriteIndex = 0;
            lua_pushboolean(lua, true);
            return 1;
        }
        else
        {
            lua_pushboolean(lua, false);
            return 1;
        }
    }

    int rogue_data_request_is_read(lua_State* lua)
    {
        lua_pushboolean(lua, s_RecentReq.m_Type == GameDataRequest::REQUEST_READ);
        return 1;
    }

    int rogue_data_request_get_read(lua_State* lua)
    {
        lua_pushnumber(lua, (int)s_RecentReq.m_Address);
        lua_pushnumber(lua, (int)s_RecentReq.m_Size);
        return 2;
    }

    int rogue_data_request_get_write(lua_State* lua)
    {
        if (s_WriteIndex < s_RecentReq.m_Data.size())
        {
            lua_pushnumber(lua, (int)(s_RecentReq.m_Address + s_WriteIndex));

            size_t remainingBytes = s_RecentReq.m_Data.size() - s_WriteIndex;

            if (remainingBytes >= 4)
            {
                lua_pushnumber(lua, (int)4);
                lua_pushnumber(lua, *((s32*)&s_RecentReq.m_Data[s_WriteIndex]));
                s_WriteIndex += 4;
            }
            else if(remainingBytes >= 2)
            {
                lua_pushnumber(lua, (int)2);
                lua_pushnumber(lua, *((s16*)&s_RecentReq.m_Data[s_WriteIndex]));
                s_WriteIndex += 2;
            }
            else
            {
                lua_pushnumber(lua, (int)1);
                lua_pushnumber(lua, s_RecentReq.m_Data[s_WriteIndex]);
                s_WriteIndex += 1;
            }
        }
        else
        {
            lua_pushnumber(lua, 0);
            lua_pushnumber(lua, 0);
            lua_pushnumber(lua, 0);
        }
        return 3;
    }

    int rogue_data_request_provide_result(lua_State* lua)
    {
        if (s_RecentReq.m_Type == GameDataRequest::REQUEST_READ)
        {
            u8 const* data = (u8 const* )lua_tostring(lua, 1);

            s_Response.resize(s_RecentReq.m_Size);

            memcpy(s_Response.data(), data, s_RecentReq.m_Size);
        }

        s_RecentReq.m_Callback(s_Response);
        return 0;
    }

    int rogue_attach(lua_State* lua)
    {
        // Not entirely sure why but not delayed registering these can case mGBA to crash???
        lua_register(lua, "rogue_next_data_request", rogue_next_data_request);
        lua_register(lua, "rogue_data_request_is_read", rogue_data_request_is_read);
        lua_register(lua, "rogue_data_request_get_read", rogue_data_request_get_read);
        lua_register(lua, "rogue_data_request_get_write", rogue_data_request_get_write);
        lua_register(lua, "rogue_data_request_provide_result", rogue_data_request_provide_result);

        std::vector<std::string> args;
        RogueAssistant_Main(false, args);
        return 0;
    }

    int rogue_frame(lua_State* lua)
    {
        RogueAssistant_Frame();
        return 0;
    }

    int rogue_shutdown(lua_State* lua)
    {
        RogueAssistant_Shutdown();
        return 0;
    }

    /*bool IsDebuggerPresent() {
        std::ifstream statusFile("/proc/self/status");
        std::string line;

        while (std::getline(statusFile, line)) {
            if (line.rfind("TracerPid:", 0) == 0) {
                // Extract the PID value
                int tracerPid = std::stoi(line.substr(10));
                return tracerPid != 0;
            }   
        }
        return false;
    }*/
#if defined(_WIN32)
#define EXPORT __declspec(dllexport)
#else
#define EXPORT __attribute__((visibility("default")))
#endif
    EXPORT int luaopen_RogueAssistant(lua_State* lua)
    {  
#if _DEBUG
        LOG_INFO("Awaiting debugger attach..");
        /*while (!IsDebuggerPresent())
        {
            //Sleep(10);
        }*/
#endif
        lua_register(lua, "rogue_attach", rogue_attach);
        lua_register(lua, "rogue_frame", rogue_frame);
        lua_register(lua, "rogue_shutdown", rogue_shutdown);

        return 1;
    }

#ifdef __cplusplus
}
#endif
