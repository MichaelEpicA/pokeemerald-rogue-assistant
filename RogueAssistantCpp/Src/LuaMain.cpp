
#include <vector>
#include <string>

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

    int doThing(lua_State* lua)
    {
        // Example https://www.cs.usfca.edu/~galles/cs420/lecture/LuaLectures/LuaAndC.html

        //lua_getglobal(lua, "hookLog");
        //lua_pushstring(lua, "123aweawea");
        //lua_call(lua, 1, 0);

        //luaL_dostring(lua, "hookLog()");



        //a = f("how", t.x, 14)
        //
        //
        //lua_getfield(L, LUA_GLOBALSINDEX, "f");          /* function to be called */
        //lua_pushstring(L, "how");                                 /* 1st argument */
        //lua_getfield(L, LUA_GLOBALSINDEX, "t");            /* table to be indexed */
        //lua_getfield(L, -1, "x");                 /* push result of t.x (2nd arg) */
        //lua_remove(L, -2);                           /* remove 't' from the stack */
        //lua_pushinteger(L, 14);                                   /* 3rd argument */
        //lua_call(L, 3, 1);         /* call function with 3 arguments and 1 result */
        //lua_setfield(L, LUA_GLOBALSINDEX, "a");        /* set global variable 'a' */

        return 1;
    }

    int rogue_attach(lua_State* lua)
    {
        std::vector<std::string> args;
        return RogueAssistant_Main(false, args);
    }

    int rogue_frame(lua_State* lua)
    {
        RogueAssistant_Frame();
        return 1;
    }

    int rogue_shutdown(lua_State* lua)
    {
        RogueAssistant_Shutdown();
        return 1;
    }

    __declspec(dllexport) int luaopen_RogueAssistant(lua_State* lua)
    {
        lua_register(lua, "rogue_attach", rogue_attach);
        lua_register(lua, "rogue_frame", rogue_frame);
        lua_register(lua, "rogue_shutdown", rogue_shutdown);

        return 1;
    }

#ifdef __cplusplus
}
#endif
