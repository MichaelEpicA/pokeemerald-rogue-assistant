// dllmain.cpp : Defines the entry point for the DLL application.
//#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
//// Windows Header Files
//#include <windows.h>
//
//BOOL APIENTRY DllMain(HMODULE hModule,
//    DWORD  ul_reason_for_call,
//    LPVOID lpReserved
//)
//{
//    switch (ul_reason_for_call)
//    {
//    case DLL_PROCESS_ATTACH:
//    case DLL_THREAD_ATTACH:
//    case DLL_THREAD_DETACH:
//    case DLL_PROCESS_DETACH:
//        break;
//    }
//    return TRUE;
//}

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

    __declspec(dllexport) int luaopen_mGBATools(lua_State* lua)
    {
        lua_register(lua, "dotest_test", doThing);

        return 1;
    }

#ifdef __cplusplus
}
#endif
