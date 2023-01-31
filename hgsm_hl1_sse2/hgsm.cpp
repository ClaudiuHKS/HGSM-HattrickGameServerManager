
#include "hgsm.h"

::plugin_info_t Plugin_info
{
    META_INTERFACE_VERSION,
    xName,
    __DATE__,
    __DATE__,
    "Hattrick HKS",
    "https://hattrick.go.ro/",
    "HGSM",
    ::PT_ANYPAUSE,
    ::PT_ANYPAUSE,
};

::globalvars_t* gpGlobals{ };
::enginefuncs_t g_engfuncs{ };

::meta_globals_t* gpMetaGlobals{ };
::mutil_funcs_t* gpMetaUtilFuncs{ };
::gamedll_funcs_t* gpGamedllFuncs{ };

::DLL_FUNCTIONS gFunctionTable{ };
::DLL_FUNCTIONS* gpFunctionTable{ };

::DLL_FUNCTIONS gFunctionTable_Post{ };
::DLL_FUNCTIONS* gpFunctionTable_Post{ };

::enginefuncs_t gEngFuncTable{ };
::enginefuncs_t* gpEngFuncTable{ };

::enginefuncs_t gEngFuncTable_Post{ };
::enginefuncs_t* gpEngFuncTable_Post{ };

::META_FUNCTIONS gMetaFunctionTable{ };
::META_FUNCTIONS* gpMetaFunctionTable{ };

::cvar_t g_cvVersion{ "hgsm_version", __DATE__, FCVAR_SERVER | FCVAR_SPONLY, { }, };

::std::unordered_map < ::std::string, ::std::string > g_mapKeyValues[16384]{ };

::std::string g_strGameDir{ };

::std::string g_strWelcome_1{ };
::std::string g_strWelcome_2{ };
::std::string g_strWelcome_3{ };
::std::string g_strWelcome_4{ };
::std::string g_strWelcome_5{ };
::std::string g_strWelcome_6{ };

::std::string g_strNotify_1{ };
::std::string g_strNotify_2{ };

#ifdef WIN32

::HWND__* g_pWnd{ };

#endif

bool g_bShow[48]{ };

bool g_bHide[48]{ };
bool g_bArmor[48]{ };
bool g_bRadar[48]{ };

bool g_bAttached{ };

bool g_bLateLoaded{ };
bool g_bNoReturn{ };

bool g_bResetHud{ };
bool g_bDeathMsg{ };

bool g_bNoWeapons{ };

bool g_bStripper{ };

bool g_bKvEditor{ };
bool g_bKvAdder{ };

bool g_bHideRadar{ };
bool g_bHideMoney{ };
bool g_bHideTimer{ };
bool g_bHideFlash{ };

bool g_bRealScoreFriendly{ };
bool g_bRealScore{ };
bool g_bZeroMoney{ };
bool g_bFixScore{ };
bool g_bFixKills{ };
bool g_bFixDeaths{ };
bool g_bKeepHelmet{ };
bool g_bHourlyRestart{ };

bool g_bOneReliable{ };
bool g_bAllReliable{ };

int g_nDeaths[48]{ };
int g_nKills[48]{ };

int g_nDeathMsg{ };
int g_nTextMsg{ };
int g_nScoreShort{ };
int g_nHideWeapon{ };
int g_nScoreInfo{ };
int g_nCrosshair{ };
int g_nArmorType{ };
int g_nResetHud{ };

int g_nArmor{ };

int g_nTicks{ 200, };

int g_nResetHudId{ };

int g_nDeathMsgByte{ };

int g_nOsOffs{ };

int g_nKiller{ };
int g_nVictim{ };

int m_iWindows{ };
int m_iLinux{ 5, };
int m_iDodTeam{ 1400, };
int m_iDodScore{ 476, };
int m_iDodDeaths{ 477, };
int m_iKevlar{ 112, };
int m_iTeam{ 114, };
int m_iMoney{ 115, };
int m_iFov{ 363, };
int m_iDeaths{ 444, };

#ifdef WIN32

unsigned long g_ulProc{ };

#endif

C_DLLEXPORT void WINAPI GiveFnptrsToDll(::enginefuncs_t* pengfuncsFromEngine, ::globalvars_t* pGlobals) noexcept
{
    if (pGlobals)
    {
        if (::gpGlobals = pGlobals)
        {
            if (pengfuncsFromEngine)
            {
                ::std::memcpy(&::g_engfuncs, pengfuncsFromEngine, sizeof(::enginefuncs_t));
                {
                    ::g_bAttached = true;
                    {
                        return;
                    }
                }
            }

            else
            {
                ::std::printf("'enginefuncs_t' Failed\n");
            }
        }

        else
        {
            ::std::printf("'globalvars_t' Failed\n");
        }
    }

    else
    {
        ::std::printf("'globalvars_t' Failed\n");
    }

    ::std::printf("'GiveFnptrsToDll' Failed\n");
}

C_DLLEXPORT int GetEntityAPI2(::DLL_FUNCTIONS* pFunctionTable, int*)
{
    if (::g_bAttached)
    {
        if (pFunctionTable)
        {
            if (::gpFunctionTable = pFunctionTable)
            {
                ::std::memset(&::gFunctionTable, { }, sizeof(::DLL_FUNCTIONS));
                {
                    ::gFunctionTable.pfnKeyValue = ::Hook_KeyValue;
                    {
                        ::gFunctionTable.pfnSpawn = ::Hook_Spawn;
                        {
                            ::gFunctionTable.pfnServerActivate = ::Hook_ServerActivate;
                        }
                    }

                    ::gFunctionTable.pfnClientDisconnect = ::Hook_ClientDisconnect;
                }
                ::std::memcpy(pFunctionTable, &::gFunctionTable, sizeof(::DLL_FUNCTIONS));

                return xTo(true, int);
            }

            else
            {
                ::std::printf("'DLL_FUNCTIONS' Failed\n");
            }
        }

        else
        {
            ::std::printf("'DLL_FUNCTIONS' Failed\n");
        }
    }

    ::std::printf("'GetEntityAPI2' Failed\n");
    {
        return xTo(false, int);
    }
}

C_DLLEXPORT int GetEntityAPI2_Post(::DLL_FUNCTIONS* pFunctionTable, int*)
{
    if (::g_bAttached)
    {
        if (pFunctionTable)
        {
            if (::gpFunctionTable_Post = pFunctionTable)
            {
                ::std::memset(&::gFunctionTable_Post, { }, sizeof(::DLL_FUNCTIONS));
                {
                    ::gFunctionTable_Post.pfnServerActivate = ::Hook_ServerActivate_Post;
                    {
                        ::gFunctionTable_Post.pfnServerDeactivate = ::Hook_ServerDeactivate_Post;
                    }

                    ::gFunctionTable_Post.pfnStartFrame = ::Hook_StartFrame_Post;

                    ::gFunctionTable_Post.pfnClientPutInServer = ::Hook_ClientPutInServer_Post;
                    {
                        ::gFunctionTable_Post.pfnClientDisconnect = ::Hook_ClientDisconnect_Post;
                    }
                }
                ::std::memcpy(pFunctionTable, &::gFunctionTable_Post, sizeof(::DLL_FUNCTIONS));

                return xTo(true, int);
            }

            else
            {
                ::std::printf("'DLL_FUNCTIONS' Failed\n");
            }
        }

        else
        {
            ::std::printf("'DLL_FUNCTIONS' Failed\n");
        }
    }

    ::std::printf("'GetEntityAPI2_Post' Failed\n");
    {
        return xTo(false, int);
    }
}

C_DLLEXPORT int GetEngineFunctions(::enginefuncs_t* pengfuncsFromEngine, int*)
{
    if (::g_bAttached)
    {
        if (pengfuncsFromEngine)
        {
            if (::gpEngFuncTable = pengfuncsFromEngine)
            {
                ::std::memset(&::gEngFuncTable, { }, sizeof(::enginefuncs_t));
                {

                }
                ::std::memcpy(pengfuncsFromEngine, &::gEngFuncTable, sizeof(::enginefuncs_t));

                return xTo(true, int);
            }

            else
            {
                ::std::printf("'enginefuncs_t' Failed\n");
            }
        }

        else
        {
            ::std::printf("'enginefuncs_t' Failed\n");
        }
    }

    ::std::printf("'GetEngineFunctions' Failed\n");
    {
        return xTo(false, int);
    }
}

C_DLLEXPORT int GetEngineFunctions_Post(::enginefuncs_t* pengfuncsFromEngine, int*)
{
    if (::g_bAttached)
    {
        if (pengfuncsFromEngine)
        {
            if (::gpEngFuncTable_Post = pengfuncsFromEngine)
            {
                ::std::memset(&::gEngFuncTable_Post, { }, sizeof(::enginefuncs_t));
                {
                    ::gEngFuncTable_Post.pfnMessageBegin = ::Hook_MessageBegin_Post;
                    {
                        ::gEngFuncTable_Post.pfnWriteByte = ::Hook_WriteByte_Post;
                    }
                    ::gEngFuncTable_Post.pfnMessageEnd = ::Hook_MessageEnd_Post;
                }
                ::std::memcpy(pengfuncsFromEngine, &::gEngFuncTable_Post, sizeof(::enginefuncs_t));

                return xTo(true, int);
            }

            else
            {
                ::std::printf("'enginefuncs_t' Failed\n");
            }
        }

        else
        {
            ::std::printf("'enginefuncs_t' Failed\n");
        }
    }

    ::std::printf("'GetEngineFunctions_Post' Failed\n");
    {
        return xTo(false, int);
    }
}

C_DLLEXPORT void Meta_Init()
{
    if (::g_bAttached)
    {

    }

    else
    {

    }
}

C_DLLEXPORT int Meta_Query(const char* pszInterfaceVersion, ::plugin_info_t** pPlugInfo, ::mutil_funcs_t* pMetaUtilFuncs)
{
    if (::g_bAttached)
    {
        if (!::std::strcmp(pszInterfaceVersion, META_INTERFACE_VERSION))
        {
            if (pPlugInfo)
            {
                if (!(*pPlugInfo))
                {
                    if (*pPlugInfo = &::Plugin_info)
                    {
                        if (pMetaUtilFuncs)
                        {
                            if (::gpMetaUtilFuncs = pMetaUtilFuncs)
                            {
                                return xTo(true, int);
                            }

                            else
                            {
                                ::std::printf("'mutil_funcs_t' Failed\n");
                            }
                        }

                        else
                        {
                            ::std::printf("'mutil_funcs_t' Failed\n");
                        }
                    }

                    else
                    {
                        ::std::printf("'plugin_info_t' Failed\n");
                    }
                }

                else
                {
                    ::std::printf("'plugin_info_t' Failed\n");
                }
            }

            else
            {
                ::std::printf("'plugin_info_t' Failed\n");
            }
        }

        else
        {
            ::std::printf("Bad Server 'Meta Mod' Interface Version '%s' Required '%s'\n", pszInterfaceVersion, META_INTERFACE_VERSION);
        }
    }

    ::std::printf("'Meta_Query' Failed\n");
    {
        return xTo(false, int);
    }
}

C_DLLEXPORT int Meta_Attach(::PLUG_LOADTIME plugLoadTime, ::META_FUNCTIONS* pFunctionTable, ::meta_globals_t* pMGlobals, ::gamedll_funcs_t* pGamedllFuncs)
{
    static int nEntity{ };
    static char szGameDir[256]{ };

    static ::edict_t* pEntity{ };

#ifdef WIN32

    static unsigned long ulProc{ };

    static ::HWND__* pWnd{ };

#endif

    if (!::g_bAttached)
    {
        ::std::printf("'Meta_Attach' Failed\n");
        {
            return xTo(false, int);
        }
    }

    if (pMGlobals)
    {
        if (::gpMetaGlobals = pMGlobals)
        {
            if (pGamedllFuncs)
            {
                if (::gpGamedllFuncs = pGamedllFuncs)
                {

                }

                else
                {
                    ::std::printf("'Meta_Attach' Failed\n");
                    {
                        ::std::printf("'gamedll_funcs_t' Failed\n");
                        {
                            return xTo(false, int);
                        }
                    }
                }
            }

            else
            {
                ::std::printf("'Meta_Attach' Failed\n");
                {
                    ::std::printf("'gamedll_funcs_t' Failed\n");
                    {
                        return xTo(false, int);
                    }
                }
            }
        }

        else
        {
            ::std::printf("'Meta_Attach' Failed\n");
            {
                ::std::printf("'meta_globals_t' Failed\n");
                {
                    return xTo(false, int);
                }
            }
        }
    }

    else
    {
        ::std::printf("'Meta_Attach' Failed\n");
        {
            ::std::printf("'meta_globals_t' Failed\n");
            {
                return xTo(false, int);
            }
        }
    }

    (*::g_engfuncs.pfnGetGameDir) (szGameDir);
    {
        ::g_strGameDir.assign(szGameDir);
        {
            if (::g_strGameDir.empty())
            {
                ::std::printf("'Meta_Attach' Failed\n");
                {
                    ::std::printf("'pfnGetGameDir' Failed\n");
                    {
                        return xTo(false, int);
                    }
                }
            }
        }
    }

    if (pFunctionTable)
    {
        if (::gpMetaFunctionTable = pFunctionTable)
        {
            ::std::memset(&::gMetaFunctionTable, { }, sizeof(::META_FUNCTIONS));
            {
                {
                    ::gMetaFunctionTable.pfnGetEntityAPI2 = ::GetEntityAPI2;
                    ::gMetaFunctionTable.pfnGetEntityAPI2_Post = ::GetEntityAPI2_Post;
                }

                {
                    ::gMetaFunctionTable.pfnGetEngineFunctions = ::GetEngineFunctions;
                    ::gMetaFunctionTable.pfnGetEngineFunctions_Post = ::GetEngineFunctions_Post;
                }
            }
            ::std::memcpy(pFunctionTable, &::gMetaFunctionTable, sizeof(::META_FUNCTIONS));
        }

        else
        {
            ::std::printf("'Meta_Attach' Failed\n");
            {
                ::std::printf("'META_FUNCTIONS' Failed\n");
                {
                    return xTo(false, int);
                }
            }
        }
    }

    else
    {
        ::std::printf("'Meta_Attach' Failed\n");
        {
            ::std::printf("'META_FUNCTIONS' Failed\n");
            {
                return xTo(false, int);
            }
        }
    }

    if (::FileExists(::g_strGameDir + "/addons/hgsm/delete_bot_profile_files"))
    {
        if (::FileExists(::g_strGameDir + "/botchatter.db"))
        {
            ::FileRemove(::g_strGameDir + "/botchatter.db");
        }

        if (::FileExists(::g_strGameDir + "/BotChatter.db"))
        {
            ::FileRemove(::g_strGameDir + "/BotChatter.db");
        }

        if (::FileExists(::g_strGameDir + "/botcampaignprofile.db"))
        {
            ::FileRemove(::g_strGameDir + "/botcampaignprofile.db");
        }

        if (::FileExists(::g_strGameDir + "/BotCampaignProfile.db"))
        {
            ::FileRemove(::g_strGameDir + "/BotCampaignProfile.db");
        }

        if (::FileExists(::g_strGameDir + "/botprofile.db"))
        {
            ::FileRemove(::g_strGameDir + "/botprofile.db");
        }

        if (::FileExists(::g_strGameDir + "/BotProfile.db"))
        {
            ::FileRemove(::g_strGameDir + "/BotProfile.db");
        }

        if (::FileExists(::g_strGameDir + "/botprofilecoop.db"))
        {
            ::FileRemove(::g_strGameDir + "/botprofilecoop.db");
        }

        if (::FileExists(::g_strGameDir + "/BotProfileCoop.db"))
        {
            ::FileRemove(::g_strGameDir + "/BotProfileCoop.db");
        }

        if (::FileExists(::g_strGameDir + "/navplace.db"))
        {
            ::FileRemove(::g_strGameDir + "/navplace.db");
        }

        if (::FileExists(::g_strGameDir + "/NavPlace.db"))
        {
            ::FileRemove(::g_strGameDir + "/NavPlace.db");
        }
    }

    if (::FileExists(::g_strGameDir + "/addons/hgsm/gamemode_competitive.cfg"))
    {
        if (::FileExists(::g_strGameDir + "/gamemode_competitive.cfg"))
        {
            ::FileRemove(::g_strGameDir + "/gamemode_competitive.cfg");
        }

        ::FileCopy(::g_strGameDir + "/addons/hgsm/gamemode_competitive.cfg", ::g_strGameDir + "/gamemode_competitive.cfg");
    }

    if (::FileExists(::g_strGameDir + "/addons/hgsm/skill.cfg"))
    {
        if (::FileExists(::g_strGameDir + "/skill.cfg"))
        {
            ::FileRemove(::g_strGameDir + "/skill.cfg");
        }

        ::FileCopy(::g_strGameDir + "/addons/hgsm/skill.cfg", ::g_strGameDir + "/skill.cfg");
    }

    if (::FileExists(::g_strGameDir + "/addons/hgsm/game.cfg"))
    {
        if (::FileExists(::g_strGameDir + "/game.cfg"))
        {
            ::FileRemove(::g_strGameDir + "/game.cfg");
        }

        ::FileCopy(::g_strGameDir + "/addons/hgsm/game.cfg", ::g_strGameDir + "/game.cfg");
    }

    if (::FileExists(::g_strGameDir + "/addons/hgsm/skill1.cfg"))
    {
        if (::FileExists(::g_strGameDir + "/skill1.cfg"))
        {
            ::FileRemove(::g_strGameDir + "/skill1.cfg");
        }

        ::FileCopy(::g_strGameDir + "/addons/hgsm/skill1.cfg", ::g_strGameDir + "/skill1.cfg");
    }

    if (::FileExists(::g_strGameDir + "/addons/hgsm/config.cfg"))
    {
        if (::FileExists(::g_strGameDir + "/config.cfg"))
        {
            ::FileRemove(::g_strGameDir + "/config.cfg");
        }

        ::FileCopy(::g_strGameDir + "/addons/hgsm/config.cfg", ::g_strGameDir + "/config.cfg");
    }

    if (::FileExists(::g_strGameDir + "/addons/hgsm/listenserver.cfg"))
    {
        if (::FileExists(::g_strGameDir + "/listenserver.cfg"))
        {
            ::FileRemove(::g_strGameDir + "/listenserver.cfg");
        }

        ::FileCopy(::g_strGameDir + "/addons/hgsm/listenserver.cfg", ::g_strGameDir + "/listenserver.cfg");
    }

    if (::FileExists(::g_strGameDir + "/addons/hgsm/autoexec.cfg"))
    {
        if (::FileExists(::g_strGameDir + "/autoexec.cfg"))
        {
            ::FileRemove(::g_strGameDir + "/autoexec.cfg");
        }

        ::FileCopy(::g_strGameDir + "/addons/hgsm/autoexec.cfg", ::g_strGameDir + "/autoexec.cfg");
    }

    if (::FileExists(::g_strGameDir + "/addons/hgsm/bspconvar_whitelist.txt"))
    {
        if (::FileExists(::g_strGameDir + "/bspconvar_whitelist.txt"))
        {
            ::FileRemove(::g_strGameDir + "/bspconvar_whitelist.txt");
        }

        ::FileCopy(::g_strGameDir + "/addons/hgsm/bspconvar_whitelist.txt", ::g_strGameDir + "/bspconvar_whitelist.txt");
    }

    if (::FileExists(::g_strGameDir + "/addons/hgsm/valve.rc"))
    {
        if (::FileExists("valve/valve.rc"))
        {
            ::FileRemove("valve/valve.rc");
        }

        if (::FileExists("valve"))
        {
            if (::FileDirectory("valve"))
            {
                ::FileCopy(::g_strGameDir + "/addons/hgsm/valve.rc", "valve/valve.rc");
            }
        }

        else if (::FileDirectory("valve"))
        {
            ::FileCopy(::g_strGameDir + "/addons/hgsm/valve.rc", "valve/valve.rc");
        }
    }

    if (::FileExists(::g_strGameDir + "/addons/hgsm/delete_motd_file"))
    {
        if (::FileExists(::g_strGameDir + "/motd.txt"))
        {
            ::FileRemove(::g_strGameDir + "/motd.txt");
        }

        if (::FileExists(::g_strGameDir + "/motd_text.txt"))
        {
            ::FileRemove(::g_strGameDir + "/motd_text.txt");
        }

        if (::FileExists(::g_strGameDir + "/motd_default.txt"))
        {
            ::FileRemove(::g_strGameDir + "/motd_default.txt");
        }

        if (::FileExists(::g_strGameDir + "/cfg/motd_default.txt"))
        {
            ::FileRemove(::g_strGameDir + "/cfg/motd_default.txt");
        }
    }

    else
    {
        if (::FileExists(::g_strGameDir + "/addons/hgsm/motd.txt"))
        {
            if (::FileExists(::g_strGameDir + "/motd.txt"))
            {
                ::FileRemove(::g_strGameDir + "/motd.txt");
            }

            ::FileCopy(::g_strGameDir + "/addons/hgsm/motd.txt", ::g_strGameDir + "/motd.txt");
        }

        if (::FileExists(::g_strGameDir + "/addons/hgsm/motd_text.txt"))
        {
            if (::FileExists(::g_strGameDir + "/motd_text.txt"))
            {
                ::FileRemove(::g_strGameDir + "/motd_text.txt");
            }

            ::FileCopy(::g_strGameDir + "/addons/hgsm/motd_text.txt", ::g_strGameDir + "/motd_text.txt");
        }

        if (::FileExists(::g_strGameDir + "/addons/hgsm/motd_default.txt"))
        {
            if (::FileExists(::g_strGameDir + "/motd_default.txt"))
            {
                ::FileRemove(::g_strGameDir + "/motd_default.txt");
            }

            ::FileCopy(::g_strGameDir + "/addons/hgsm/motd_default.txt", ::g_strGameDir + "/motd_default.txt");
        }

        if (::FileExists(::g_strGameDir + "/addons/hgsm/motd_default.txt"))
        {
            if (::FileExists(::g_strGameDir + "/cfg/motd_default.txt"))
            {
                ::FileRemove(::g_strGameDir + "/cfg/motd_default.txt");
            }

            ::FileCopy(::g_strGameDir + "/addons/hgsm/motd_default.txt", ::g_strGameDir + "/cfg/motd_default.txt");
        }
    }

    if (::FileExists(::g_strGameDir + "/addons/hgsm/mapcycle.txt"))
    {
        if (::FileExists(::g_strGameDir + "/mapcycle.txt"))
        {
            ::FileRemove(::g_strGameDir + "/mapcycle.txt");
        }

        ::FileCopy(::g_strGameDir + "/addons/hgsm/mapcycle.txt", ::g_strGameDir + "/mapcycle.txt");
    }

    if (::FileExists(::g_strGameDir + "/addons/hgsm/mapcycle_default.txt"))
    {
        if (::FileExists(::g_strGameDir + "/mapcycle_default.txt"))
        {
            ::FileRemove(::g_strGameDir + "/mapcycle_default.txt");
        }

        ::FileCopy(::g_strGameDir + "/addons/hgsm/mapcycle_default.txt", ::g_strGameDir + "/mapcycle_default.txt");
    }

    if (::FileExists(::g_strGameDir + "/addons/hgsm/mapcycle_default.txt"))
    {
        if (::FileExists(::g_strGameDir + "/cfg/mapcycle_default.txt"))
        {
            ::FileRemove(::g_strGameDir + "/cfg/mapcycle_default.txt");
        }

        ::FileCopy(::g_strGameDir + "/addons/hgsm/mapcycle_default.txt", ::g_strGameDir + "/cfg/mapcycle_default.txt");
    }

    if (::FileExists(::g_strGameDir + "/addons/hgsm/maplist.txt"))
    {
        if (::FileExists(::g_strGameDir + "/maplist.txt"))
        {
            ::FileRemove(::g_strGameDir + "/maplist.txt");
        }

        ::FileCopy(::g_strGameDir + "/addons/hgsm/maplist.txt", ::g_strGameDir + "/maplist.txt");
    }

    if (::FileExists(::g_strGameDir + "/addons/hgsm/missioncycle.txt"))
    {
        if (::FileExists(::g_strGameDir + "/missioncycle.txt"))
        {
            ::FileRemove(::g_strGameDir + "/missioncycle.txt");
        }

        ::FileCopy(::g_strGameDir + "/addons/hgsm/missioncycle.txt", ::g_strGameDir + "/missioncycle.txt");
    }

    if (::FileExists(::g_strGameDir + "/addons/hgsm/gamemodes_server.txt"))
    {
        if (::FileExists(::g_strGameDir + "/gamemodes_server.txt"))
        {
            ::FileRemove(::g_strGameDir + "/gamemodes_server.txt");
        }

        ::FileCopy(::g_strGameDir + "/addons/hgsm/gamemodes_server.txt", ::g_strGameDir + "/gamemodes_server.txt");
    }

    if (::FileExists(::g_strGameDir + "/addons/hgsm/gamerulescvars.txt"))
    {
        if (::FileExists(::g_strGameDir + "/gamerulescvars.txt"))
        {
            ::FileRemove(::g_strGameDir + "/gamerulescvars.txt");
        }

        ::FileCopy(::g_strGameDir + "/addons/hgsm/gamerulescvars.txt", ::g_strGameDir + "/gamerulescvars.txt");
    }

#ifdef WIN32

    if (::g_ulProc = ::GetCurrentProcessId())
    {
        if (pWnd = ::GetActiveWindow())
        {
            ::GetWindowThreadProcessId(pWnd, &ulProc);

            if (ulProc == ::g_ulProc)
            {
                if (!::FileExists("nohideme"))
                {
                    ::ShowWindow(pWnd, { });
                }

                ::g_pWnd = pWnd;
            }

            else if (pWnd = ::GetForegroundWindow())
            {
                ::GetWindowThreadProcessId(pWnd, &ulProc);

                if (ulProc == ::g_ulProc)
                {
                    if (!::FileExists("nohideme"))
                    {
                        ::ShowWindow(pWnd, { });
                    }

                    ::g_pWnd = pWnd;
                }
            }
        }

        else if (pWnd = ::GetForegroundWindow())
        {
            ::GetWindowThreadProcessId(pWnd, &ulProc);

            if (ulProc == ::g_ulProc)
            {
                if (!::FileExists("nohideme"))
                {
                    ::ShowWindow(pWnd, { });
                }

                ::g_pWnd = pWnd;
            }

            else if (pWnd = ::GetActiveWindow())
            {
                ::GetWindowThreadProcessId(pWnd, &ulProc);

                if (ulProc == ::g_ulProc)
                {
                    if (!::FileExists("nohideme"))
                    {
                        ::ShowWindow(pWnd, { });
                    }

                    ::g_pWnd = pWnd;
                }
            }
        }
    }

#endif

    if (!((*::g_engfuncs.pfnCVarGetPointer) (::g_cvVersion.name)))
    {
        (*::g_engfuncs.pfnCVarRegister) (&::g_cvVersion);
    }

    if (plugLoadTime)
    {
        if (plugLoadTime != ::PT_STARTUP)
        {
            if (plugLoadTime != ::PT_CHANGELEVEL)
            {
                ::g_bLateLoaded = true;
                {
                    ::g_bNoReturn = true;

                    {
                        ::Hook_ServerActivate(nullptr, { }, { });
                        {
                            ::Hook_ServerActivate_Post(nullptr, { }, { });
                        }

                        for (nEntity = xTo(true, int); nEntity <= ::gpGlobals->maxClients; nEntity++)
                        {
                            if (pEntity = ::INDEXENT(nEntity))
                            {
                                if (!pEntity->free)
                                {
                                    if (pEntity->pvPrivateData)
                                    {
                                        if (!pEntity->serialnumber)
                                        {
                                            if (!(pEntity->v.flags & FL_DORMANT))
                                            {
                                                if (pEntity->v.flags & FL_CLIENT)
                                                {
                                                    ::Hook_ClientPutInServer_Post(pEntity);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }

                    ::g_bNoReturn = { };
                }
            }
        }
    }

#ifdef WIN32

    ::MultiMediaFeatureTryAttach();

#endif

    return xTo(true, int);
}

C_DLLEXPORT int Meta_Detach(::PLUG_LOADTIME, ::PL_UNLOAD_REASON)
{
    {
        if (::gpFunctionTable)
        {
            ::std::memset(&::gFunctionTable, { }, sizeof(::DLL_FUNCTIONS));
            {
                ::std::memcpy(::gpFunctionTable, &::gFunctionTable, sizeof(::DLL_FUNCTIONS));
            }
        }

        if (::gpFunctionTable_Post)
        {
            ::std::memset(&::gFunctionTable_Post, { }, sizeof(::DLL_FUNCTIONS));
            {
                ::std::memcpy(::gpFunctionTable_Post, &::gFunctionTable_Post, sizeof(::DLL_FUNCTIONS));
            }
        }
    }

    {
        if (::gpEngFuncTable)
        {
            ::std::memset(&::gEngFuncTable, { }, sizeof(::enginefuncs_t));
            {
                ::std::memcpy(::gpEngFuncTable, &::gEngFuncTable, sizeof(::enginefuncs_t));
            }
        }

        if (::gpEngFuncTable_Post)
        {
            ::std::memset(&::gEngFuncTable_Post, { }, sizeof(::enginefuncs_t));
            {
                ::std::memcpy(::gpEngFuncTable_Post, &::gEngFuncTable_Post, sizeof(::enginefuncs_t));
            }
        }
    }

    if (::gpMetaFunctionTable)
    {
        ::std::memset(&::gMetaFunctionTable, { }, sizeof(::META_FUNCTIONS));
        {
            ::std::memcpy(::gpMetaFunctionTable, &::gMetaFunctionTable, sizeof(::META_FUNCTIONS));
        }
    }

#ifdef WIN32

    ::MultiMediaFeatureTryDetach();

#endif

    return xTo(true, int);
}

int Hook_Spawn(::edict_t* pEntity) noexcept
{
    static ::std::string strClass{ }, strMap{ }, strPath{ };
    static ::std::ifstream inFile{ };
    static ::nlohmann::json jsTreeStripper{ }, jsTreeKvAdder{ };
    static ::KeyValueData kvData{ };

    static int nIter{ }, nEntity{ };
    static bool bErrorStripper{ }, bErrorKvAdder{ }, bKeep{ };
    static char szClassName[256]{ }, szKeyName[256]{ }, szValue[256]{ };

    if (!::g_bStripper)
    {
        strPath.assign(::g_strGameDir + "/addons/hgsm/stripper.cfg");
        {
            bErrorStripper = true;
        }

        if (::FileExists(strPath))
        {
            inFile.open(strPath);

            if (inFile.is_open())
            {
                jsTreeStripper.clear();
                {
                    jsTreeStripper = ::nlohmann::json::parse(inFile, nullptr, { }, true);
                    {
                        inFile.close();

                        inFile.clear();
                    }
                }

                strMap.assign(STRING(::gpGlobals->mapname));

                if (jsTreeStripper.is_discarded())
                {
                    jsTreeStripper.clear();
                    {
                        ::std::printf("'%s' Has INVALID 'JSON'\n", strPath.c_str());
                    }
                }

                else if (jsTreeStripper.empty())
                {
                    jsTreeStripper.clear();
                    {
                        ::std::printf("Loaded '%s'\n", strPath.c_str());
                    }
                }

                else if (!jsTreeStripper.is_object())
                {
                    jsTreeStripper.clear();
                    {
                        ::std::printf("Loaded '%s'\n", strPath.c_str());
                    }
                }

                else if (!strMap.empty())
                {
                    bErrorStripper = false;
                    {
                        ::std::printf("Loaded '%s'\n", strPath.c_str());
                    }
                }

                else
                {
                    jsTreeStripper.clear();
                    {
                        ::std::printf("Loaded '%s'\n", strPath.c_str());
                    }
                }
            }

            else
            {
                inFile.clear();
                {
                    ::std::printf("Failed To Open '%s'\n", strPath.c_str());
                }
            }
        }

        else
        {
            ::std::printf("Failed To Find '%s'\n", strPath.c_str());
        }

        ::g_bStripper = true;
    }

    if (!::g_bKvAdder)
    {
        strPath.assign(::g_strGameDir + "/addons/hgsm/kv_map_adder.cfg");
        {
            bErrorKvAdder = true;
        }

        if (::FileExists(strPath))
        {
            inFile.open(strPath);

            if (inFile.is_open())
            {
                jsTreeKvAdder.clear();
                {
                    jsTreeKvAdder = ::nlohmann::json::parse(inFile, nullptr, { }, true);
                    {
                        inFile.close();

                        inFile.clear();
                    }
                }

                strMap.assign(STRING(::gpGlobals->mapname));

                if (jsTreeKvAdder.is_discarded())
                {
                    jsTreeKvAdder.clear();
                    {
                        ::std::printf("'%s' Has INVALID 'JSON'\n", strPath.c_str());
                    }
                }

                else if (jsTreeKvAdder.empty())
                {
                    jsTreeKvAdder.clear();
                    {
                        ::std::printf("Loaded '%s'\n", strPath.c_str());
                    }
                }

                else if (!jsTreeKvAdder.is_object())
                {
                    jsTreeKvAdder.clear();
                    {
                        ::std::printf("Loaded '%s'\n", strPath.c_str());
                    }
                }

                else if (!strMap.empty())
                {
                    bErrorKvAdder = false;
                    {
                        ::std::printf("Loaded '%s'\n", strPath.c_str());
                    }
                }

                else
                {
                    jsTreeKvAdder.clear();
                    {
                        ::std::printf("Loaded '%s'\n", strPath.c_str());
                    }
                }
            }

            else
            {
                inFile.clear();
                {
                    ::std::printf("Failed To Open '%s'\n", strPath.c_str());
                }
            }
        }

        else
        {
            ::std::printf("Failed To Find '%s'\n", strPath.c_str());
        }

        ::g_bKvAdder = true;
    }

    if (!bErrorStripper)
    {
        if (pEntity)
        {
            strClass.assign(STRING(pEntity->v.classname));
            {
                if (!strClass.empty())
                {
                    bKeep = { };
                    {
                        for (auto& jsItem : jsTreeStripper)
                        {
                            if (!jsItem.empty())
                            {
                                if (jsItem.is_object())
                                {
                                    if (!jsItem["map"].empty())
                                    {
                                        if (jsItem["map"].is_string())
                                        {
                                            if (!strMap.compare(jsItem["map"].get < ::std::string >()) || (jsItem["map"].get < ::std::string >()).empty())
                                            {
                                                if (!jsItem["classname"].empty())
                                                {
                                                    if (jsItem["classname"].is_string())
                                                    {
                                                        if (!strClass.compare(jsItem["classname"].get < ::std::string >()))
                                                        {
                                                            if (!jsItem["origin_x"].empty())
                                                            {
                                                                if (jsItem["origin_x"].is_number())
                                                                {
                                                                    if (pEntity->v.origin.x == jsItem["origin_x"].get < float >() || jsItem["origin_x"].get < float >() == 2147483647.f)
                                                                    {
                                                                        if (!jsItem["origin_y"].empty())
                                                                        {
                                                                            if (jsItem["origin_y"].is_number())
                                                                            {
                                                                                if (pEntity->v.origin.y == jsItem["origin_y"].get < float >() || jsItem["origin_y"].get < float >() == 2147483647.f)
                                                                                {
                                                                                    if (!jsItem["origin_z"].empty())
                                                                                    {
                                                                                        if (jsItem["origin_z"].is_number())
                                                                                        {
                                                                                            if (pEntity->v.origin.z == jsItem["origin_z"].get < float >() || jsItem["origin_z"].get < float >() == 2147483647.f)
                                                                                            {
                                                                                                if ((nEntity = ::ENTINDEX(pEntity)) > 0)
                                                                                                {
                                                                                                    for (nIter = { }; nIter < 10; nIter++)
                                                                                                    {
                                                                                                        ::std::snprintf(szKeyName, sizeof szKeyName, "ifn_key_%d", nIter);
                                                                                                        {
                                                                                                            if (!jsItem[szKeyName].empty())
                                                                                                            {
                                                                                                                if (jsItem[szKeyName].is_string())
                                                                                                                {
                                                                                                                    if (!((jsItem[szKeyName].get < ::std::string >()).empty()))
                                                                                                                    {
                                                                                                                        ::std::snprintf(szValue, sizeof szValue, "ifn_value_%d", nIter);
                                                                                                                        {
                                                                                                                            if (!jsItem[szValue].empty())
                                                                                                                            {
                                                                                                                                if (jsItem[szValue].is_string())
                                                                                                                                {
                                                                                                                                    if ((jsItem[szValue].get < ::std::string >()).empty())
                                                                                                                                    {
                                                                                                                                        if (::g_mapKeyValues[nEntity][(jsItem[szKeyName].get < ::std::string >())].empty())
                                                                                                                                        {
                                                                                                                                            bKeep = true;
                                                                                                                                        }
                                                                                                                                    }

                                                                                                                                    else
                                                                                                                                    {
                                                                                                                                        if (!((jsItem[szValue].get < ::std::string >()).compare(::g_mapKeyValues[nEntity][(jsItem[szKeyName].get < ::std::string >())])))
                                                                                                                                        {
                                                                                                                                            bKeep = true;
                                                                                                                                        }
                                                                                                                                    }
                                                                                                                                }
                                                                                                                            }
                                                                                                                        }
                                                                                                                    }
                                                                                                                }
                                                                                                            }
                                                                                                        }
                                                                                                    }

                                                                                                    for (nIter = { }; nIter < 10; nIter++)
                                                                                                    {
                                                                                                        ::std::snprintf(szKeyName, sizeof szKeyName, "if_key_%d", nIter);
                                                                                                        {
                                                                                                            if (!jsItem[szKeyName].empty())
                                                                                                            {
                                                                                                                if (jsItem[szKeyName].is_string())
                                                                                                                {
                                                                                                                    if (!((jsItem[szKeyName].get < ::std::string >()).empty()))
                                                                                                                    {
                                                                                                                        ::std::snprintf(szValue, sizeof szValue, "if_value_%d", nIter);
                                                                                                                        {
                                                                                                                            if (!jsItem[szValue].empty())
                                                                                                                            {
                                                                                                                                if (jsItem[szValue].is_string())
                                                                                                                                {
                                                                                                                                    if ((jsItem[szValue].get < ::std::string >()).empty())
                                                                                                                                    {
                                                                                                                                        if (::g_mapKeyValues[nEntity][(jsItem[szKeyName].get < ::std::string >())].empty())
                                                                                                                                        {
                                                                                                                                            bKeep = { };
                                                                                                                                        }
                                                                                                                                    }

                                                                                                                                    else
                                                                                                                                    {
                                                                                                                                        if (!((jsItem[szValue].get < ::std::string >()).compare(::g_mapKeyValues[nEntity][(jsItem[szKeyName].get < ::std::string >())])))
                                                                                                                                        {
                                                                                                                                            bKeep = { };
                                                                                                                                        }
                                                                                                                                    }
                                                                                                                                }
                                                                                                                            }
                                                                                                                        }
                                                                                                                    }
                                                                                                                }
                                                                                                            }
                                                                                                        }
                                                                                                    }
                                                                                                }

                                                                                                if (!bKeep)
                                                                                                {
                                                                                                    ::std::printf("REMOVED '%s' [%f %f %f]\n", strClass.c_str(), pEntity->v.origin.x, pEntity->v.origin.y, pEntity->v.origin.z);
                                                                                                    {
                                                                                                        (*::g_engfuncs.pfnRemoveEntity) (pEntity);
                                                                                                    }

                                                                                                    {
                                                                                                        do
                                                                                                        {
                                                                                                            ::gpMetaGlobals->mres = ::MRES_SUPERCEDE;

                                                                                                            return xTo(true, int);
                                                                                                        }

                                                                                                        while (false);
                                                                                                    }
                                                                                                }

                                                                                                else
                                                                                                {
                                                                                                    if (nEntity > 0)
                                                                                                    {
                                                                                                        ::std::printf("PRESERVED '%s' (%d) [%f %f %f]\n", strClass.c_str(), nEntity, pEntity->v.origin.x, pEntity->v.origin.y, pEntity->v.origin.z);
                                                                                                    }

                                                                                                    else
                                                                                                    {
                                                                                                        ::std::printf("PRESERVED '%s' [%f %f %f]\n", strClass.c_str(), pEntity->v.origin.x, pEntity->v.origin.y, pEntity->v.origin.z);
                                                                                                    }
                                                                                                }
                                                                                            }
                                                                                        }
                                                                                    }
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    if (!bErrorKvAdder)
    {
        if (pEntity)
        {
            strClass.assign(STRING(pEntity->v.classname));
            {
                if (!strClass.empty())
                {
                    for (auto& jsItem : jsTreeKvAdder)
                    {
                        if (!jsItem.empty())
                        {
                            if (jsItem.is_object())
                            {
                                if (!jsItem["map"].empty())
                                {
                                    if (jsItem["map"].is_string())
                                    {
                                        if (!strMap.compare(jsItem["map"].get < ::std::string >()) || (jsItem["map"].get < ::std::string >()).empty())
                                        {
                                            if (!jsItem["classname"].empty())
                                            {
                                                if (jsItem["classname"].is_string())
                                                {
                                                    if (!strClass.compare(jsItem["classname"].get < ::std::string >()))
                                                    {
                                                        if (!jsItem["origin_x"].empty())
                                                        {
                                                            if (jsItem["origin_x"].is_number())
                                                            {
                                                                if (pEntity->v.origin.x == jsItem["origin_x"].get < float >() || jsItem["origin_x"].get < float >() == 2147483647.f)
                                                                {
                                                                    if (!jsItem["origin_y"].empty())
                                                                    {
                                                                        if (jsItem["origin_y"].is_number())
                                                                        {
                                                                            if (pEntity->v.origin.y == jsItem["origin_y"].get < float >() || jsItem["origin_y"].get < float >() == 2147483647.f)
                                                                            {
                                                                                if (!jsItem["origin_z"].empty())
                                                                                {
                                                                                    if (jsItem["origin_z"].is_number())
                                                                                    {
                                                                                        if (pEntity->v.origin.z == jsItem["origin_z"].get < float >() || jsItem["origin_z"].get < float >() == 2147483647.f)
                                                                                        {
                                                                                            if (!jsItem["kv_classname"].empty())
                                                                                            {
                                                                                                if (jsItem["kv_classname"].is_string())
                                                                                                {
                                                                                                    if (!jsItem["key"].empty())
                                                                                                    {
                                                                                                        if (jsItem["key"].is_string())
                                                                                                        {
                                                                                                            if (!jsItem["value"].empty())
                                                                                                            {
                                                                                                                if (jsItem["value"].is_string())
                                                                                                                {
                                                                                                                    ::std::printf("IN '%s' [%f %f %f] ADDED '%s: %s = %s'\n", strClass.c_str(), pEntity->v.origin.x, pEntity->v.origin.y, pEntity->v.origin.z, (jsItem["kv_classname"].get < ::std::string >()).c_str(), (jsItem["key"].get < ::std::string >()).c_str(), (jsItem["value"].get < ::std::string >()).c_str());
                                                                                                                    {
                                                                                                                        ::std::snprintf(szClassName, sizeof szClassName, (jsItem["kv_classname"].get < ::std::string >()).c_str());
                                                                                                                        ::std::snprintf(szKeyName, sizeof szKeyName, (jsItem["key"].get < ::std::string >()).c_str());
                                                                                                                        ::std::snprintf(szValue, sizeof szValue, (jsItem["value"].get < ::std::string >()).c_str());
                                                                                                                        {
                                                                                                                            kvData.szClassName = szClassName;
                                                                                                                            kvData.szKeyName = szKeyName;
                                                                                                                            kvData.szValue = szValue;
                                                                                                                            {
                                                                                                                                kvData.fHandled = { };
                                                                                                                            }
                                                                                                                        }
                                                                                                                    }
                                                                                                                    {
                                                                                                                        ::gpGamedllFuncs->dllapi_table->pfnKeyValue(pEntity, &kvData);
                                                                                                                        {
                                                                                                                            ::std::printf("* THE CALL WAS '%s'\n", kvData.fHandled ? "HANDLED" : "UNHANDLED");
                                                                                                                        }
                                                                                                                    }
                                                                                                                }
                                                                                                            }
                                                                                                        }
                                                                                                    }
                                                                                                }
                                                                                            }
                                                                                        }
                                                                                    }
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    do
    {
        ::gpMetaGlobals->mres = ::MRES_IGNORED;

        return xTo(false, int);
    }

    while (false);
}

void Hook_ServerActivate(::edict_t*, int, int) noexcept
{

#ifdef WIN32

    static ::std::array arConVars{ "port", "sv_port", "sv_hostport", "sv_host_port", "hostport", "host_port", };

#else

    static ::std::array < const char*, xTo(6, unsigned int) > arConVars{ "port", "sv_port", "sv_hostport", "sv_host_port", "hostport", "host_port", };

#endif

#ifndef WIN32

    static ::FILE* pICpu{ }, * pOCpu{ }, * pIMem{ }, * pOMem{ };

    static char szCpu[256]{ }, szMem[256]{ };

#endif

    static ::std::string strPath{ }, strCmd{ }, strVal{ };
    static ::std::ifstream inFile{ };
    static ::nlohmann::json jsTree{ };
    static ::cvar_t* pConVar{ };
    static ::FILE* pISys{ };

    static char szBuffer[2048]{ }, szTrimmed[2048]{ }, szSys[256]{ };
    static int nPrio{ }, nIter{ }, nLen{ };

#ifdef WIN32

    static int nDisablePrioBoost{ };

    static bool bDisablePrioBoost{ };

#endif

    ::g_strWelcome_1.clear();
    {
        ::g_strWelcome_1.shrink_to_fit();
    }

    ::g_strWelcome_2.clear();
    {
        ::g_strWelcome_2.shrink_to_fit();
    }

    ::g_strWelcome_3.clear();
    {
        ::g_strWelcome_3.shrink_to_fit();
    }

    ::g_strWelcome_4.clear();
    {
        ::g_strWelcome_4.shrink_to_fit();
    }

    ::g_strWelcome_5.clear();
    {
        ::g_strWelcome_5.shrink_to_fit();
    }

    ::g_strWelcome_6.clear();
    {
        ::g_strWelcome_6.shrink_to_fit();
    }

    ::g_strNotify_1.clear();
    {
        ::g_strNotify_1.shrink_to_fit();
    }

    ::g_strNotify_2.clear();
    {
        ::g_strNotify_2.shrink_to_fit();
    }

    ::g_bHideRadar = { };
    ::g_bHideMoney = { };
    ::g_bHideTimer = { };
    ::g_bHideFlash = { };
    ::g_bZeroMoney = { };
    ::g_bFixScore = { };
    ::g_bRealScore = { };
    ::g_bRealScoreFriendly = { };
    ::g_bFixKills = { };
    ::g_bFixDeaths = { };
    ::g_bNoWeapons = { };
    ::g_bKeepHelmet = { };
    ::g_bHourlyRestart = { };
    ::g_bOneReliable = { };
    ::g_bAllReliable = { };

    ::g_nArmor = { };

    ::std::snprintf(szSys, sizeof szSys, "%s/addons/hgsm/sys.cfg", ::g_strGameDir.c_str());

#ifndef WIN32

    ::std::snprintf(szCpu, sizeof szCpu, "%s/addons/hgsm/cpu.txt", ::g_strGameDir.c_str());
    ::std::snprintf(szMem, sizeof szMem, "%s/addons/hgsm/mem.txt", ::g_strGameDir.c_str());

#endif

    if (pISys = ::std::fopen(szSys, "r"))
    {
        while (!::std::feof(pISys))
        {
            nLen = { };
            {
                szBuffer[0] = { };
                szTrimmed[0] = { };
            }

            ::std::fgets(szBuffer, sizeof szBuffer, pISys);
            {
                for (nIter = { }; nIter < xTo(::std::strlen(szBuffer), int); nIter++)
                {
                    if (szBuffer[nIter] != '\n' && szBuffer[nIter] != '\r' && szBuffer[nIter] != '\a' && szBuffer[nIter] != '\b' && szBuffer[nIter] != '\f' && szBuffer[nIter] != '\v')
                    {
                        if (szBuffer[nIter] == '\t')
                        {
                            szTrimmed[nLen] = ' ';
                        }

                        else
                        {
                            szTrimmed[nLen] = szBuffer[nIter];
                        }

                        nLen++;
                    }
                }
            }

            szTrimmed[nLen] = { };
            {
                if (!nLen || !szTrimmed[0] || szTrimmed[0] == ';' || szTrimmed[0] == '#' || szTrimmed[0] == '/')
                {
                    continue;
                }
            }

            ::std::printf("EXECUTING '%s'\n", szTrimmed);
            {
                ::std::system(szTrimmed);
            }
        }

        ::std::fclose(pISys);
        {
            ::std::printf("Executed '%s'\n", szSys);
        }
    }

    else
    {
        ::std::printf("Failed To Read '%s'\n", szSys);
    }

#ifndef WIN32

    if (pICpu = ::std::fopen("/proc/cpuinfo", "r"))
    {
        ::unlink(szCpu);
        {
            while (!::std::feof(pICpu))
            {
                szBuffer[0] = { };
                {
                    ::std::fgets(szBuffer, sizeof szBuffer, pICpu);
                }

                if (pOCpu = ::std::fopen(szCpu, "a"))
                {
                    ::std::fputs(szBuffer, pOCpu);
                    {
                        ::std::fclose(pOCpu);
                    }
                }
            }

            ::std::fclose(pICpu);
        }
    }

    ::std::printf("Tried To Create '%s'\n", szCpu);

#endif

#ifndef WIN32

    if (pIMem = ::std::fopen("/proc/meminfo", "r"))
    {
        ::unlink(szMem);
        {
            while (!::std::feof(pIMem))
            {
                szBuffer[0] = { };
                {
                    ::std::fgets(szBuffer, sizeof szBuffer, pIMem);
                }

                if (pOMem = ::std::fopen(szMem, "a"))
                {
                    ::std::fputs(szBuffer, pOMem);
                    {
                        ::std::fclose(pOMem);
                    }
                }
            }

            ::std::fclose(pIMem);
        }
    }

    ::std::printf("Tried To Create '%s'\n", szMem);

#endif

    if (!pConVar)
    {
        for (auto& cvItem : arConVars)
        {
            if (pConVar = (*::g_engfuncs.pfnCVarGetPointer) (cvItem))
            {
                if (pConVar->string)
                {
                    if (*pConVar->string)
                    {
                        break;
                    }
                }
            }
        }
    }

    if (pConVar)
    {
        if (pConVar->string)
        {
            if (*pConVar->string)
            {
                strVal.assign(pConVar->string);

                strPath.assign(::g_strGameDir + "/" + strVal + ".cfg");

                if (::FileExists(strPath))
                {
                    strCmd.assign("exec \"" + strVal + ".cfg\"\n");
                    {
                        (*::g_engfuncs.pfnServerCommand) (strCmd.c_str());
                        {
                            ::std::printf("Executed '%s'\n", strPath.c_str());
                        }
                    }
                }

                else
                {
                    ::std::printf("Failed To Find '%s'\n", strPath.c_str());
                }
            }
        }
    }

    strPath.assign(::g_strGameDir + "/addons/hgsm/offs.cfg");

    if (!::FileExists(strPath))
    {
        ::std::printf("Failed To Find '%s'\n", strPath.c_str());
        {
            ::std::printf("'linux' Is Now '%d'\n", ::m_iLinux);
            ::std::printf("'windows' Is Now '%d'\n", ::m_iWindows);
            ::std::printf("'dod_score' Is Now '%d'\n", ::m_iDodScore);
            ::std::printf("'dod_deaths' Is Now '%d'\n", ::m_iDodDeaths);
            ::std::printf("'dod_team' Is Now '%d'\n", ::m_iDodTeam);
            ::std::printf("'kevlar' Is Now '%d'\n", ::m_iKevlar);
            ::std::printf("'team' Is Now '%d'\n", ::m_iTeam);
            ::std::printf("'money' Is Now '%d'\n", ::m_iMoney);
            ::std::printf("'fov' Is Now '%d'\n", ::m_iFov);
            ::std::printf("'deaths' Is Now '%d'\n", ::m_iDeaths);
        }
    }

    else
    {
        inFile.open(strPath);

        if (!inFile.is_open())
        {
            inFile.clear();
            {
                ::std::printf("Failed To Open '%s'\n", strPath.c_str());
                {
                    ::std::printf("'linux' Is Now '%d'\n", ::m_iLinux);
                    ::std::printf("'windows' Is Now '%d'\n", ::m_iWindows);
                    ::std::printf("'dod_score' Is Now '%d'\n", ::m_iDodScore);
                    ::std::printf("'dod_deaths' Is Now '%d'\n", ::m_iDodDeaths);
                    ::std::printf("'dod_team' Is Now '%d'\n", ::m_iDodTeam);
                    ::std::printf("'kevlar' Is Now '%d'\n", ::m_iKevlar);
                    ::std::printf("'team' Is Now '%d'\n", ::m_iTeam);
                    ::std::printf("'money' Is Now '%d'\n", ::m_iMoney);
                    ::std::printf("'fov' Is Now '%d'\n", ::m_iFov);
                    ::std::printf("'deaths' Is Now '%d'\n", ::m_iDeaths);
                }
            }
        }

        else
        {
            jsTree.clear();
            {
                jsTree = ::nlohmann::json::parse(inFile, nullptr, { }, true);
                {
                    inFile.close();

                    inFile.clear();
                }
            }

            if (jsTree.is_discarded())
            {
                jsTree.clear();
                {
                    ::std::printf("'%s' Has INVALID 'JSON'\n", strPath.c_str());
                    {
                        ::std::printf("'linux' Is Now '%d'\n", ::m_iLinux);
                        ::std::printf("'windows' Is Now '%d'\n", ::m_iWindows);
                        ::std::printf("'dod_score' Is Now '%d'\n", ::m_iDodScore);
                        ::std::printf("'dod_deaths' Is Now '%d'\n", ::m_iDodDeaths);
                        ::std::printf("'dod_team' Is Now '%d'\n", ::m_iDodTeam);
                        ::std::printf("'kevlar' Is Now '%d'\n", ::m_iKevlar);
                        ::std::printf("'team' Is Now '%d'\n", ::m_iTeam);
                        ::std::printf("'money' Is Now '%d'\n", ::m_iMoney);
                        ::std::printf("'fov' Is Now '%d'\n", ::m_iFov);
                        ::std::printf("'deaths' Is Now '%d'\n", ::m_iDeaths);
                    }
                }
            }

            else
            {
                if (jsTree.empty())
                {
                    jsTree.clear();
                    {
                        ::std::printf("Loaded '%s'\n", strPath.c_str());
                        {
                            ::std::printf("'linux' Is Now '%d'\n", ::m_iLinux);
                            ::std::printf("'windows' Is Now '%d'\n", ::m_iWindows);
                            ::std::printf("'dod_score' Is Now '%d'\n", ::m_iDodScore);
                            ::std::printf("'dod_deaths' Is Now '%d'\n", ::m_iDodDeaths);
                            ::std::printf("'dod_team' Is Now '%d'\n", ::m_iDodTeam);
                            ::std::printf("'kevlar' Is Now '%d'\n", ::m_iKevlar);
                            ::std::printf("'team' Is Now '%d'\n", ::m_iTeam);
                            ::std::printf("'money' Is Now '%d'\n", ::m_iMoney);
                            ::std::printf("'fov' Is Now '%d'\n", ::m_iFov);
                            ::std::printf("'deaths' Is Now '%d'\n", ::m_iDeaths);
                        }
                    }
                }

                else
                {
                    if (!jsTree.is_object())
                    {
                        jsTree.clear();
                        {
                            ::std::printf("Loaded '%s'\n", strPath.c_str());
                            {
                                ::std::printf("'linux' Is Now '%d'\n", ::m_iLinux);
                                ::std::printf("'windows' Is Now '%d'\n", ::m_iWindows);
                                ::std::printf("'dod_score' Is Now '%d'\n", ::m_iDodScore);
                                ::std::printf("'dod_deaths' Is Now '%d'\n", ::m_iDodDeaths);
                                ::std::printf("'dod_team' Is Now '%d'\n", ::m_iDodTeam);
                                ::std::printf("'kevlar' Is Now '%d'\n", ::m_iKevlar);
                                ::std::printf("'team' Is Now '%d'\n", ::m_iTeam);
                                ::std::printf("'money' Is Now '%d'\n", ::m_iMoney);
                                ::std::printf("'fov' Is Now '%d'\n", ::m_iFov);
                                ::std::printf("'deaths' Is Now '%d'\n", ::m_iDeaths);
                            }
                        }
                    }

                    else
                    {
                        if (jsTree["linux"].empty())
                        {
                            ::std::printf("'linux' Is Now '%d'\n", ::m_iLinux);
                        }

                        else if (!jsTree["linux"].is_number())
                        {
                            ::std::printf("'linux' Is Now '%d'\n", ::m_iLinux);
                        }

                        else
                        {
                            ::m_iLinux = jsTree["linux"].get < int >();
                            {
                                ::std::printf("'linux' Is Now '%d'\n", ::m_iLinux);
                            }
                        }

                        if (jsTree["windows"].empty())
                        {
                            ::std::printf("'windows' Is Now '%d'\n", ::m_iWindows);
                        }

                        else if (!jsTree["windows"].is_number())
                        {
                            ::std::printf("'windows' Is Now '%d'\n", ::m_iWindows);
                        }

                        else
                        {
                            ::m_iWindows = jsTree["windows"].get < int >();
                            {
                                ::std::printf("'windows' Is Now '%d'\n", ::m_iWindows);
                            }
                        }

                        if (jsTree["dod_score"].empty())
                        {
                            ::std::printf("'dod_score' Is Now '%d'\n", ::m_iDodScore);
                        }

                        else if (!jsTree["dod_score"].is_number())
                        {
                            ::std::printf("'dod_score' Is Now '%d'\n", ::m_iDodScore);
                        }

                        else
                        {
                            ::m_iDodScore = jsTree["dod_score"].get < int >();
                            {
                                ::std::printf("'dod_score' Is Now '%d'\n", ::m_iDodScore);
                            }
                        }

                        if (jsTree["dod_deaths"].empty())
                        {
                            ::std::printf("'dod_deaths' Is Now '%d'\n", ::m_iDodDeaths);
                        }

                        else if (!jsTree["dod_deaths"].is_number())
                        {
                            ::std::printf("'dod_deaths' Is Now '%d'\n", ::m_iDodDeaths);
                        }

                        else
                        {
                            ::m_iDodDeaths = jsTree["dod_deaths"].get < int >();
                            {
                                ::std::printf("'dod_deaths' Is Now '%d'\n", ::m_iDodDeaths);
                            }
                        }

                        if (jsTree["dod_team"].empty())
                        {
                            ::std::printf("'dod_team' Is Now '%d'\n", ::m_iDodTeam);
                        }

                        else if (!jsTree["dod_team"].is_number())
                        {
                            ::std::printf("'dod_team' Is Now '%d'\n", ::m_iDodTeam);
                        }

                        else
                        {
                            ::m_iDodTeam = jsTree["dod_team"].get < int >();
                            {
                                ::std::printf("'dod_team' Is Now '%d'\n", ::m_iDodTeam);
                            }
                        }

                        if (jsTree["kevlar"].empty())
                        {
                            ::std::printf("'kevlar' Is Now '%d'\n", ::m_iKevlar);
                        }

                        else if (!jsTree["kevlar"].is_number())
                        {
                            ::std::printf("'kevlar' Is Now '%d'\n", ::m_iKevlar);
                        }

                        else
                        {
                            ::m_iKevlar = jsTree["kevlar"].get < int >();
                            {
                                ::std::printf("'kevlar' Is Now '%d'\n", ::m_iKevlar);
                            }
                        }

                        if (jsTree["team"].empty())
                        {
                            ::std::printf("'team' Is Now '%d'\n", ::m_iTeam);
                        }

                        else if (!jsTree["team"].is_number())
                        {
                            ::std::printf("'team' Is Now '%d'\n", ::m_iTeam);
                        }

                        else
                        {
                            ::m_iTeam = jsTree["team"].get < int >();
                            {
                                ::std::printf("'team' Is Now '%d'\n", ::m_iTeam);
                            }
                        }

                        if (jsTree["money"].empty())
                        {
                            ::std::printf("'money' Is Now '%d'\n", ::m_iMoney);
                        }

                        else if (!jsTree["money"].is_number())
                        {
                            ::std::printf("'money' Is Now '%d'\n", ::m_iMoney);
                        }

                        else
                        {
                            ::m_iMoney = jsTree["money"].get < int >();
                            {
                                ::std::printf("'money' Is Now '%d'\n", ::m_iMoney);
                            }
                        }

                        if (jsTree["fov"].empty())
                        {
                            ::std::printf("'fov' Is Now '%d'\n", ::m_iFov);
                        }

                        else if (!jsTree["fov"].is_number())
                        {
                            ::std::printf("'fov' Is Now '%d'\n", ::m_iFov);
                        }

                        else
                        {
                            ::m_iFov = jsTree["fov"].get < int >();
                            {
                                ::std::printf("'fov' Is Now '%d'\n", ::m_iFov);
                            }
                        }

                        if (jsTree["deaths"].empty())
                        {
                            ::std::printf("'deaths' Is Now '%d'\n", ::m_iDeaths);
                        }

                        else if (!jsTree["deaths"].is_number())
                        {
                            ::std::printf("'deaths' Is Now '%d'\n", ::m_iDeaths);
                        }

                        else
                        {
                            ::m_iDeaths = jsTree["deaths"].get < int >();
                            {
                                ::std::printf("'deaths' Is Now '%d'\n", ::m_iDeaths);
                            }
                        }

                        jsTree.clear();
                        {
                            ::std::printf("Loaded '%s'\n", strPath.c_str());
                        }
                    }
                }
            }
        }
    }

#ifdef WIN32

    ::g_nOsOffs = ::m_iWindows;

#else

    ::g_nOsOffs = ::m_iLinux;

#endif

    strPath.assign(::g_strGameDir + "/addons/hgsm/server_tick_rate_and_priority.cfg");

    if (!::FileExists(strPath))
    {
        ::std::printf("Failed To Find '%s'\n", strPath.c_str());
        {
            ::std::printf("'Ticks Loop' Is Now '%d'\n", ::g_nTicks);
            ::std::printf("'Keep Helmet' Is Now '%s'\n", ::g_bKeepHelmet ? "TRUE" : "FALSE");
            ::std::printf("'Hourly Restart' Is Now '%s'\n", ::g_bHourlyRestart ? "TRUE" : "FALSE");
            ::std::printf("'No Weapons' Is Now '%s'\n", ::g_bNoWeapons ? "TRUE" : "FALSE");
            ::std::printf("'Fix Score' Is Now '%s'\n", ::g_bFixScore ? "TRUE" : "FALSE");
            ::std::printf("'Real Score' Is Now '%s'\n", ::g_bRealScore ? "TRUE" : "FALSE");
            ::std::printf("'Real Score Friendly' Is Now '%s'\n", ::g_bRealScoreFriendly ? "TRUE" : "FALSE");
            ::std::printf("'Fix Kills' Is Now '%s'\n", ::g_bFixKills ? "TRUE" : "FALSE");
            ::std::printf("'Fix Deaths' Is Now '%s'\n", ::g_bFixDeaths ? "TRUE" : "FALSE");
            ::std::printf("'Zero Money' Is Now '%s'\n", ::g_bZeroMoney ? "TRUE" : "FALSE");
            ::std::printf("'One Reliable' Is Now '%s'\n", ::g_bOneReliable ? "TRUE" : "FALSE");
            ::std::printf("'All Reliable' Is Now '%s'\n", ::g_bAllReliable ? "TRUE" : "FALSE");
            ::std::printf("'Hide Flash' Is Now '%s'\n", ::g_bHideFlash ? "TRUE" : "FALSE");
            ::std::printf("'Hide Timer' Is Now '%s'\n", ::g_bHideTimer ? "TRUE" : "FALSE");
            ::std::printf("'Hide Money' Is Now '%s'\n", ::g_bHideMoney ? "TRUE" : "FALSE");
            ::std::printf("'Welcome Console 1' Is Now '%s'\n", ::g_strWelcome_1.c_str());
            ::std::printf("'Welcome Console 2' Is Now '%s'\n", ::g_strWelcome_2.c_str());
            ::std::printf("'Welcome Console 3' Is Now '%s'\n", ::g_strWelcome_3.c_str());
            ::std::printf("'Welcome Console 4' Is Now '%s'\n", ::g_strWelcome_4.c_str());
            ::std::printf("'Welcome Console 5' Is Now '%s'\n", ::g_strWelcome_5.c_str());
            ::std::printf("'Welcome Console 6' Is Now '%s'\n", ::g_strWelcome_6.c_str());
            ::std::printf("'Welcome Notify 1' Is Now '%s'\n", ::g_strNotify_1.c_str());
            ::std::printf("'Welcome Notify 2' Is Now '%s'\n", ::g_strNotify_2.c_str());
            ::std::printf("'Hide Radar' Is Now '%s'\n", ::g_bHideRadar ? "TRUE" : "FALSE");
            ::std::printf("'Give Armor In Spawn' Is Now '%d'\n", ::g_nArmor);

#ifdef WIN32

            nDisablePrioBoost = { };
            {
                ::GetProcessPriorityBoost(::GetCurrentProcess(), &nDisablePrioBoost);
                {
                    ::std::printf("'Disable Priority Boost' Is Now '%s'\n", xTo(nDisablePrioBoost, bool) ? "TRUE" : "FALSE");
                }
            }

#endif

#ifdef WIN32

            ::std::printf("'Windows Priority Class' Is Now '%d'\n", xTo(::GetPriorityClass(::GetCurrentProcess()), int));

#else

            ::std::printf("'Linux Priority Class' Is Now '%d'\n", ::getpriority(::PRIO_PROCESS, ::getpid()));

#endif

            {
                if (!::g_bNoReturn)
                {
                    do
                    {
                        ::gpMetaGlobals->mres = ::MRES_IGNORED;

                        return;
                    }

                    while (false);
                }

                else
                {
                    return;
                }
            }
        }
    }

    inFile.open(strPath);

    if (!inFile.is_open())
    {
        ::std::printf("Failed To Open '%s'\n", strPath.c_str());
        {
            inFile.clear();
        }

        ::std::printf("'Ticks Loop' Is Now '%d'\n", ::g_nTicks);
        ::std::printf("'Keep Helmet' Is Now '%s'\n", ::g_bKeepHelmet ? "TRUE" : "FALSE");
        ::std::printf("'Hourly Restart' Is Now '%s'\n", ::g_bHourlyRestart ? "TRUE" : "FALSE");
        ::std::printf("'No Weapons' Is Now '%s'\n", ::g_bNoWeapons ? "TRUE" : "FALSE");
        ::std::printf("'Fix Score' Is Now '%s'\n", ::g_bFixScore ? "TRUE" : "FALSE");
        ::std::printf("'Real Score' Is Now '%s'\n", ::g_bRealScore ? "TRUE" : "FALSE");
        ::std::printf("'Real Score Friendly' Is Now '%s'\n", ::g_bRealScoreFriendly ? "TRUE" : "FALSE");
        ::std::printf("'Fix Kills' Is Now '%s'\n", ::g_bFixKills ? "TRUE" : "FALSE");
        ::std::printf("'Fix Deaths' Is Now '%s'\n", ::g_bFixDeaths ? "TRUE" : "FALSE");
        ::std::printf("'Zero Money' Is Now '%s'\n", ::g_bZeroMoney ? "TRUE" : "FALSE");
        ::std::printf("'One Reliable' Is Now '%s'\n", ::g_bOneReliable ? "TRUE" : "FALSE");
        ::std::printf("'All Reliable' Is Now '%s'\n", ::g_bAllReliable ? "TRUE" : "FALSE");
        ::std::printf("'Hide Flash' Is Now '%s'\n", ::g_bHideFlash ? "TRUE" : "FALSE");
        ::std::printf("'Hide Timer' Is Now '%s'\n", ::g_bHideTimer ? "TRUE" : "FALSE");
        ::std::printf("'Hide Money' Is Now '%s'\n", ::g_bHideMoney ? "TRUE" : "FALSE");
        ::std::printf("'Welcome Console 1' Is Now '%s'\n", ::g_strWelcome_1.c_str());
        ::std::printf("'Welcome Console 2' Is Now '%s'\n", ::g_strWelcome_2.c_str());
        ::std::printf("'Welcome Console 3' Is Now '%s'\n", ::g_strWelcome_3.c_str());
        ::std::printf("'Welcome Console 4' Is Now '%s'\n", ::g_strWelcome_4.c_str());
        ::std::printf("'Welcome Console 5' Is Now '%s'\n", ::g_strWelcome_5.c_str());
        ::std::printf("'Welcome Console 6' Is Now '%s'\n", ::g_strWelcome_6.c_str());
        ::std::printf("'Welcome Notify 1' Is Now '%s'\n", ::g_strNotify_1.c_str());
        ::std::printf("'Welcome Notify 2' Is Now '%s'\n", ::g_strNotify_2.c_str());
        ::std::printf("'Hide Radar' Is Now '%s'\n", ::g_bHideRadar ? "TRUE" : "FALSE");
        ::std::printf("'Give Armor In Spawn' Is Now '%d'\n", ::g_nArmor);

#ifdef WIN32

        nDisablePrioBoost = { };
        {
            ::GetProcessPriorityBoost(::GetCurrentProcess(), &nDisablePrioBoost);
            {
                ::std::printf("'Disable Priority Boost' Is Now '%s'\n", xTo(nDisablePrioBoost, bool) ? "TRUE" : "FALSE");
            }
        }

#endif

#ifdef WIN32

        ::std::printf("'Windows Priority Class' Is Now '%d'\n", xTo(::GetPriorityClass(::GetCurrentProcess()), int));

#else

        ::std::printf("'Linux Priority Class' Is Now '%d'\n", ::getpriority(::PRIO_PROCESS, ::getpid()));

#endif

        if (!::g_bNoReturn)
        {
            do
            {
                ::gpMetaGlobals->mres = ::MRES_IGNORED;

                return;
            }

            while (false);
        }

        else
        {
            return;
        }
    }

    jsTree.clear();
    {
        jsTree = ::nlohmann::json::parse(inFile, nullptr, { }, true);
        {
            inFile.close();

            inFile.clear();
        }
    }

    if (jsTree.is_discarded())
    {
        jsTree.clear();
        {
            ::std::printf("'Ticks Loop' Is Now '%d'\n", ::g_nTicks);
            ::std::printf("'Keep Helmet' Is Now '%s'\n", ::g_bKeepHelmet ? "TRUE" : "FALSE");
            ::std::printf("'Hourly Restart' Is Now '%s'\n", ::g_bHourlyRestart ? "TRUE" : "FALSE");
            ::std::printf("'No Weapons' Is Now '%s'\n", ::g_bNoWeapons ? "TRUE" : "FALSE");
            ::std::printf("'Fix Score' Is Now '%s'\n", ::g_bFixScore ? "TRUE" : "FALSE");
            ::std::printf("'Real Score' Is Now '%s'\n", ::g_bRealScore ? "TRUE" : "FALSE");
            ::std::printf("'Real Score Friendly' Is Now '%s'\n", ::g_bRealScoreFriendly ? "TRUE" : "FALSE");
            ::std::printf("'Fix Kills' Is Now '%s'\n", ::g_bFixKills ? "TRUE" : "FALSE");
            ::std::printf("'Fix Deaths' Is Now '%s'\n", ::g_bFixDeaths ? "TRUE" : "FALSE");
            ::std::printf("'Zero Money' Is Now '%s'\n", ::g_bZeroMoney ? "TRUE" : "FALSE");
            ::std::printf("'One Reliable' Is Now '%s'\n", ::g_bOneReliable ? "TRUE" : "FALSE");
            ::std::printf("'All Reliable' Is Now '%s'\n", ::g_bAllReliable ? "TRUE" : "FALSE");
            ::std::printf("'Hide Flash' Is Now '%s'\n", ::g_bHideFlash ? "TRUE" : "FALSE");
            ::std::printf("'Hide Timer' Is Now '%s'\n", ::g_bHideTimer ? "TRUE" : "FALSE");
            ::std::printf("'Hide Money' Is Now '%s'\n", ::g_bHideMoney ? "TRUE" : "FALSE");
            ::std::printf("'Welcome Console 1' Is Now '%s'\n", ::g_strWelcome_1.c_str());
            ::std::printf("'Welcome Console 2' Is Now '%s'\n", ::g_strWelcome_2.c_str());
            ::std::printf("'Welcome Console 3' Is Now '%s'\n", ::g_strWelcome_3.c_str());
            ::std::printf("'Welcome Console 4' Is Now '%s'\n", ::g_strWelcome_4.c_str());
            ::std::printf("'Welcome Console 5' Is Now '%s'\n", ::g_strWelcome_5.c_str());
            ::std::printf("'Welcome Console 6' Is Now '%s'\n", ::g_strWelcome_6.c_str());
            ::std::printf("'Welcome Notify 1' Is Now '%s'\n", ::g_strNotify_1.c_str());
            ::std::printf("'Welcome Notify 2' Is Now '%s'\n", ::g_strNotify_2.c_str());
            ::std::printf("'Hide Radar' Is Now '%s'\n", ::g_bHideRadar ? "TRUE" : "FALSE");
            ::std::printf("'Give Armor In Spawn' Is Now '%d'\n", ::g_nArmor);

#ifdef WIN32

            nDisablePrioBoost = { };
            {
                ::GetProcessPriorityBoost(::GetCurrentProcess(), &nDisablePrioBoost);
                {
                    ::std::printf("'Disable Priority Boost' Is Now '%s'\n", xTo(nDisablePrioBoost, bool) ? "TRUE" : "FALSE");
                }
            }

#endif

#ifdef WIN32

            ::std::printf("'Windows Priority Class' Is Now '%d'\n", xTo(::GetPriorityClass(::GetCurrentProcess()), int));

#else

            ::std::printf("'Linux Priority Class' Is Now '%d'\n", ::getpriority(::PRIO_PROCESS, ::getpid()));

#endif

            {
                ::std::printf("'%s' Has INVALID 'JSON'\n", strPath.c_str());
            }
        }

        if (!::g_bNoReturn)
        {
            do
            {
                ::gpMetaGlobals->mres = ::MRES_IGNORED;

                return;
            }

            while (false);
        }

        else
        {
            return;
        }
    }

    else if (jsTree.empty())
    {
        jsTree.clear();
        {
            ::std::printf("Loaded '%s'\n", strPath.c_str());
            {
                ::std::printf("'Ticks Loop' Is Now '%d'\n", ::g_nTicks);
                ::std::printf("'Keep Helmet' Is Now '%s'\n", ::g_bKeepHelmet ? "TRUE" : "FALSE");
                ::std::printf("'Hourly Restart' Is Now '%s'\n", ::g_bHourlyRestart ? "TRUE" : "FALSE");
                ::std::printf("'No Weapons' Is Now '%s'\n", ::g_bNoWeapons ? "TRUE" : "FALSE");
                ::std::printf("'Fix Score' Is Now '%s'\n", ::g_bFixScore ? "TRUE" : "FALSE");
                ::std::printf("'Real Score' Is Now '%s'\n", ::g_bRealScore ? "TRUE" : "FALSE");
                ::std::printf("'Real Score Friendly' Is Now '%s'\n", ::g_bRealScoreFriendly ? "TRUE" : "FALSE");
                ::std::printf("'Fix Kills' Is Now '%s'\n", ::g_bFixKills ? "TRUE" : "FALSE");
                ::std::printf("'Fix Deaths' Is Now '%s'\n", ::g_bFixDeaths ? "TRUE" : "FALSE");
                ::std::printf("'Zero Money' Is Now '%s'\n", ::g_bZeroMoney ? "TRUE" : "FALSE");
                ::std::printf("'One Reliable' Is Now '%s'\n", ::g_bOneReliable ? "TRUE" : "FALSE");
                ::std::printf("'All Reliable' Is Now '%s'\n", ::g_bAllReliable ? "TRUE" : "FALSE");
                ::std::printf("'Hide Flash' Is Now '%s'\n", ::g_bHideFlash ? "TRUE" : "FALSE");
                ::std::printf("'Hide Timer' Is Now '%s'\n", ::g_bHideTimer ? "TRUE" : "FALSE");
                ::std::printf("'Hide Money' Is Now '%s'\n", ::g_bHideMoney ? "TRUE" : "FALSE");
                ::std::printf("'Welcome Console 1' Is Now '%s'\n", ::g_strWelcome_1.c_str());
                ::std::printf("'Welcome Console 2' Is Now '%s'\n", ::g_strWelcome_2.c_str());
                ::std::printf("'Welcome Console 3' Is Now '%s'\n", ::g_strWelcome_3.c_str());
                ::std::printf("'Welcome Console 4' Is Now '%s'\n", ::g_strWelcome_4.c_str());
                ::std::printf("'Welcome Console 5' Is Now '%s'\n", ::g_strWelcome_5.c_str());
                ::std::printf("'Welcome Console 6' Is Now '%s'\n", ::g_strWelcome_6.c_str());
                ::std::printf("'Welcome Notify 1' Is Now '%s'\n", ::g_strNotify_1.c_str());
                ::std::printf("'Welcome Notify 2' Is Now '%s'\n", ::g_strNotify_2.c_str());
                ::std::printf("'Hide Radar' Is Now '%s'\n", ::g_bHideRadar ? "TRUE" : "FALSE");
                ::std::printf("'Give Armor In Spawn' Is Now '%d'\n", ::g_nArmor);

#ifdef WIN32

                nDisablePrioBoost = { };
                {
                    ::GetProcessPriorityBoost(::GetCurrentProcess(), &nDisablePrioBoost);
                    {
                        ::std::printf("'Disable Priority Boost' Is Now '%s'\n", xTo(nDisablePrioBoost, bool) ? "TRUE" : "FALSE");
                    }
                }

#endif

#ifdef WIN32

                ::std::printf("'Windows Priority Class' Is Now '%d'\n", xTo(::GetPriorityClass(::GetCurrentProcess()), int));

#else

                ::std::printf("'Linux Priority Class' Is Now '%d'\n", ::getpriority(::PRIO_PROCESS, ::getpid()));

#endif

            }
        }

        if (!::g_bNoReturn)
        {
            do
            {
                ::gpMetaGlobals->mres = ::MRES_IGNORED;

                return;
            }

            while (false);
        }

        else
        {
            return;
        }
    }

    else if (!jsTree.is_object())
    {
        jsTree.clear();
        {
            ::std::printf("Loaded '%s'\n", strPath.c_str());
            {
                ::std::printf("'Ticks Loop' Is Now '%d'\n", ::g_nTicks);
                ::std::printf("'Keep Helmet' Is Now '%s'\n", ::g_bKeepHelmet ? "TRUE" : "FALSE");
                ::std::printf("'Hourly Restart' Is Now '%s'\n", ::g_bHourlyRestart ? "TRUE" : "FALSE");
                ::std::printf("'No Weapons' Is Now '%s'\n", ::g_bNoWeapons ? "TRUE" : "FALSE");
                ::std::printf("'Fix Score' Is Now '%s'\n", ::g_bFixScore ? "TRUE" : "FALSE");
                ::std::printf("'Real Score' Is Now '%s'\n", ::g_bRealScore ? "TRUE" : "FALSE");
                ::std::printf("'Real Score Friendly' Is Now '%s'\n", ::g_bRealScoreFriendly ? "TRUE" : "FALSE");
                ::std::printf("'Fix Kills' Is Now '%s'\n", ::g_bFixKills ? "TRUE" : "FALSE");
                ::std::printf("'Fix Deaths' Is Now '%s'\n", ::g_bFixDeaths ? "TRUE" : "FALSE");
                ::std::printf("'Zero Money' Is Now '%s'\n", ::g_bZeroMoney ? "TRUE" : "FALSE");
                ::std::printf("'One Reliable' Is Now '%s'\n", ::g_bOneReliable ? "TRUE" : "FALSE");
                ::std::printf("'All Reliable' Is Now '%s'\n", ::g_bAllReliable ? "TRUE" : "FALSE");
                ::std::printf("'Hide Flash' Is Now '%s'\n", ::g_bHideFlash ? "TRUE" : "FALSE");
                ::std::printf("'Hide Timer' Is Now '%s'\n", ::g_bHideTimer ? "TRUE" : "FALSE");
                ::std::printf("'Hide Money' Is Now '%s'\n", ::g_bHideMoney ? "TRUE" : "FALSE");
                ::std::printf("'Welcome Console 1' Is Now '%s'\n", ::g_strWelcome_1.c_str());
                ::std::printf("'Welcome Console 2' Is Now '%s'\n", ::g_strWelcome_2.c_str());
                ::std::printf("'Welcome Console 3' Is Now '%s'\n", ::g_strWelcome_3.c_str());
                ::std::printf("'Welcome Console 4' Is Now '%s'\n", ::g_strWelcome_4.c_str());
                ::std::printf("'Welcome Console 5' Is Now '%s'\n", ::g_strWelcome_5.c_str());
                ::std::printf("'Welcome Console 6' Is Now '%s'\n", ::g_strWelcome_6.c_str());
                ::std::printf("'Welcome Notify 1' Is Now '%s'\n", ::g_strNotify_1.c_str());
                ::std::printf("'Welcome Notify 2' Is Now '%s'\n", ::g_strNotify_2.c_str());
                ::std::printf("'Hide Radar' Is Now '%s'\n", ::g_bHideRadar ? "TRUE" : "FALSE");
                ::std::printf("'Give Armor In Spawn' Is Now '%d'\n", ::g_nArmor);

#ifdef WIN32

                nDisablePrioBoost = { };
                {
                    ::GetProcessPriorityBoost(::GetCurrentProcess(), &nDisablePrioBoost);
                    {
                        ::std::printf("'Disable Priority Boost' Is Now '%s'\n", xTo(nDisablePrioBoost, bool) ? "TRUE" : "FALSE");
                    }
                }

#endif

#ifdef WIN32

                ::std::printf("'Windows Priority Class' Is Now '%d'\n", xTo(::GetPriorityClass(::GetCurrentProcess()), int));

#else

                ::std::printf("'Linux Priority Class' Is Now '%d'\n", ::getpriority(::PRIO_PROCESS, ::getpid()));

#endif

            }
        }

        if (!::g_bNoReturn)
        {
            do
            {
                ::gpMetaGlobals->mres = ::MRES_IGNORED;

                return;
            }

            while (false);
        }

        else
        {
            return;
        }
    }

    if (jsTree["Ticks Loop"].empty())
    {
        ::std::printf("'Ticks Loop' Is Now '%d'\n", ::g_nTicks);
    }

    else if (!jsTree["Ticks Loop"].is_number())
    {
        ::std::printf("'Ticks Loop' Is Now '%d'\n", ::g_nTicks);
    }

    else
    {
        ::g_nTicks = jsTree["Ticks Loop"].get < int >();
        {
            ::std::printf("'Ticks Loop' Is Now '%d'\n", ::g_nTicks);
        }
    }

    if (jsTree["Keep Helmet"].empty())
    {
        ::std::printf("'Keep Helmet' Is Now '%s'\n", ::g_bKeepHelmet ? "TRUE" : "FALSE");
    }

    else if (!jsTree["Keep Helmet"].is_boolean())
    {
        ::std::printf("'Keep Helmet' Is Now '%s'\n", ::g_bKeepHelmet ? "TRUE" : "FALSE");
    }

    else
    {
        ::g_bKeepHelmet = jsTree["Keep Helmet"].get < bool >();
        {
            ::std::printf("'Keep Helmet' Is Now '%s'\n", ::g_bKeepHelmet ? "TRUE" : "FALSE");
        }
    }

    if (jsTree["Hourly Restart"].empty())
    {
        ::std::printf("'Hourly Restart' Is Now '%s'\n", ::g_bHourlyRestart ? "TRUE" : "FALSE");
    }

    else if (!jsTree["Hourly Restart"].is_boolean())
    {
        ::std::printf("'Hourly Restart' Is Now '%s'\n", ::g_bHourlyRestart ? "TRUE" : "FALSE");
    }

    else
    {
        ::g_bHourlyRestart = jsTree["Hourly Restart"].get < bool >();
        {
            ::std::printf("'Hourly Restart' Is Now '%s'\n", ::g_bHourlyRestart ? "TRUE" : "FALSE");
        }
    }

    if (jsTree["No Weapons"].empty())
    {
        ::std::printf("'No Weapons' Is Now '%s'\n", ::g_bNoWeapons ? "TRUE" : "FALSE");
    }

    else if (!jsTree["No Weapons"].is_boolean())
    {
        ::std::printf("'No Weapons' Is Now '%s'\n", ::g_bNoWeapons ? "TRUE" : "FALSE");
    }

    else
    {
        ::g_bNoWeapons = jsTree["No Weapons"].get < bool >();
        {
            ::std::printf("'No Weapons' Is Now '%s'\n", ::g_bNoWeapons ? "TRUE" : "FALSE");
        }
    }

    if (jsTree["Fix Score"].empty())
    {
        ::std::printf("'Fix Score' Is Now '%s'\n", ::g_bFixScore ? "TRUE" : "FALSE");
    }

    else if (!jsTree["Fix Score"].is_boolean())
    {
        ::std::printf("'Fix Score' Is Now '%s'\n", ::g_bFixScore ? "TRUE" : "FALSE");
    }

    else
    {
        ::g_bFixScore = jsTree["Fix Score"].get < bool >();
        {
            ::std::printf("'Fix Score' Is Now '%s'\n", ::g_bFixScore ? "TRUE" : "FALSE");
        }
    }

    if (jsTree["Real Score"].empty())
    {
        ::std::printf("'Real Score' Is Now '%s'\n", ::g_bRealScore ? "TRUE" : "FALSE");
    }

    else if (!jsTree["Real Score"].is_boolean())
    {
        ::std::printf("'Real Score' Is Now '%s'\n", ::g_bRealScore ? "TRUE" : "FALSE");
    }

    else
    {
        ::g_bRealScore = jsTree["Real Score"].get < bool >();
        {
            ::std::printf("'Real Score' Is Now '%s'\n", ::g_bRealScore ? "TRUE" : "FALSE");
        }
    }

    if (jsTree["Real Score Friendly"].empty())
    {
        ::std::printf("'Real Score Friendly' Is Now '%s'\n", ::g_bRealScoreFriendly ? "TRUE" : "FALSE");
    }

    else if (!jsTree["Real Score Friendly"].is_boolean())
    {
        ::std::printf("'Real Score Friendly' Is Now '%s'\n", ::g_bRealScoreFriendly ? "TRUE" : "FALSE");
    }

    else
    {
        ::g_bRealScoreFriendly = jsTree["Real Score Friendly"].get < bool >();
        {
            ::std::printf("'Real Score Friendly' Is Now '%s'\n", ::g_bRealScoreFriendly ? "TRUE" : "FALSE");
        }
    }

    if (jsTree["Fix Kills"].empty())
    {
        ::std::printf("'Fix Kills' Is Now '%s'\n", ::g_bFixKills ? "TRUE" : "FALSE");
    }

    else if (!jsTree["Fix Kills"].is_boolean())
    {
        ::std::printf("'Fix Kills' Is Now '%s'\n", ::g_bFixKills ? "TRUE" : "FALSE");
    }

    else
    {
        ::g_bFixKills = jsTree["Fix Kills"].get < bool >();
        {
            ::std::printf("'Fix Kills' Is Now '%s'\n", ::g_bFixKills ? "TRUE" : "FALSE");
        }
    }

    if (jsTree["Fix Deaths"].empty())
    {
        ::std::printf("'Fix Deaths' Is Now '%s'\n", ::g_bFixDeaths ? "TRUE" : "FALSE");
    }

    else if (!jsTree["Fix Deaths"].is_boolean())
    {
        ::std::printf("'Fix Deaths' Is Now '%s'\n", ::g_bFixDeaths ? "TRUE" : "FALSE");
    }

    else
    {
        ::g_bFixDeaths = jsTree["Fix Deaths"].get < bool >();
        {
            ::std::printf("'Fix Deaths' Is Now '%s'\n", ::g_bFixDeaths ? "TRUE" : "FALSE");
        }
    }

    if (jsTree["Zero Money"].empty())
    {
        ::std::printf("'Zero Money' Is Now '%s'\n", ::g_bZeroMoney ? "TRUE" : "FALSE");
    }

    else if (!jsTree["Zero Money"].is_boolean())
    {
        ::std::printf("'Zero Money' Is Now '%s'\n", ::g_bZeroMoney ? "TRUE" : "FALSE");
    }

    else
    {
        ::g_bZeroMoney = jsTree["Zero Money"].get < bool >();
        {
            ::std::printf("'Zero Money' Is Now '%s'\n", ::g_bZeroMoney ? "TRUE" : "FALSE");
        }
    }

    if (jsTree["One Reliable"].empty())
    {
        ::std::printf("'One Reliable' Is Now '%s'\n", ::g_bOneReliable ? "TRUE" : "FALSE");
    }

    else if (!jsTree["One Reliable"].is_boolean())
    {
        ::std::printf("'One Reliable' Is Now '%s'\n", ::g_bOneReliable ? "TRUE" : "FALSE");
    }

    else
    {
        ::g_bOneReliable = jsTree["One Reliable"].get < bool >();
        {
            ::std::printf("'One Reliable' Is Now '%s'\n", ::g_bOneReliable ? "TRUE" : "FALSE");
        }
    }

    if (jsTree["All Reliable"].empty())
    {
        ::std::printf("'All Reliable' Is Now '%s'\n", ::g_bAllReliable ? "TRUE" : "FALSE");
    }

    else if (!jsTree["All Reliable"].is_boolean())
    {
        ::std::printf("'All Reliable' Is Now '%s'\n", ::g_bAllReliable ? "TRUE" : "FALSE");
    }

    else
    {
        ::g_bAllReliable = jsTree["All Reliable"].get < bool >();
        {
            ::std::printf("'All Reliable' Is Now '%s'\n", ::g_bAllReliable ? "TRUE" : "FALSE");
        }
    }

    if (jsTree["Hide Flash"].empty())
    {
        ::std::printf("'Hide Flash' Is Now '%s'\n", ::g_bHideFlash ? "TRUE" : "FALSE");
    }

    else if (!jsTree["Hide Flash"].is_boolean())
    {
        ::std::printf("'Hide Flash' Is Now '%s'\n", ::g_bHideFlash ? "TRUE" : "FALSE");
    }

    else
    {
        ::g_bHideFlash = jsTree["Hide Flash"].get < bool >();
        {
            ::std::printf("'Hide Flash' Is Now '%s'\n", ::g_bHideFlash ? "TRUE" : "FALSE");
        }
    }

    if (jsTree["Hide Timer"].empty())
    {
        ::std::printf("'Hide Timer' Is Now '%s'\n", ::g_bHideTimer ? "TRUE" : "FALSE");
    }

    else if (!jsTree["Hide Timer"].is_boolean())
    {
        ::std::printf("'Hide Timer' Is Now '%s'\n", ::g_bHideTimer ? "TRUE" : "FALSE");
    }

    else
    {
        ::g_bHideTimer = jsTree["Hide Timer"].get < bool >();
        {
            ::std::printf("'Hide Timer' Is Now '%s'\n", ::g_bHideTimer ? "TRUE" : "FALSE");
        }
    }

    if (jsTree["Hide Money"].empty())
    {
        ::std::printf("'Hide Money' Is Now '%s'\n", ::g_bHideMoney ? "TRUE" : "FALSE");
    }

    else if (!jsTree["Hide Money"].is_boolean())
    {
        ::std::printf("'Hide Money' Is Now '%s'\n", ::g_bHideMoney ? "TRUE" : "FALSE");
    }

    else
    {
        ::g_bHideMoney = jsTree["Hide Money"].get < bool >();
        {
            ::std::printf("'Hide Money' Is Now '%s'\n", ::g_bHideMoney ? "TRUE" : "FALSE");
        }
    }

    {
        if (jsTree["Welcome Console 1"].empty())
        {
            ::std::printf("'Welcome Console 1' Is Now '%s'\n", ::g_strWelcome_1.c_str());
        }

        else if (!jsTree["Welcome Console 1"].is_string())
        {
            ::std::printf("'Welcome Console 1' Is Now '%s'\n", ::g_strWelcome_1.c_str());
        }

        else
        {
            ::g_strWelcome_1 = jsTree["Welcome Console 1"].get < ::std::string >();

            if (!::g_strWelcome_1.empty())
            {
                ::g_strWelcome_1.shrink_to_fit();

                if (::g_strWelcome_1.length() > 190)
                {
                    ::g_strWelcome_1[::SafeErasePos(::g_strWelcome_1, 190)] = { };
                    {
                        ::g_strWelcome_1.erase(::SafeErasePos(::g_strWelcome_1, 190));
                        {
                            ::g_strWelcome_1.shrink_to_fit();
                        }
                    }
                }

                else if (::g_strWelcome_1.size() > 190)
                {
                    ::g_strWelcome_1[::SafeErasePos(::g_strWelcome_1, 190)] = { };
                    {
                        ::g_strWelcome_1.erase(::SafeErasePos(::g_strWelcome_1, 190));
                        {
                            ::g_strWelcome_1.shrink_to_fit();
                        }
                    }
                }

                else
                {

                }
            }

            else
            {
                ::g_strWelcome_1.shrink_to_fit();
            }

            ::std::printf("'Welcome Console 1' Is Now '%s'\n", ::g_strWelcome_1.c_str());
        }
    }

    {
        if (jsTree["Welcome Console 2"].empty())
        {
            ::std::printf("'Welcome Console 2' Is Now '%s'\n", ::g_strWelcome_2.c_str());
        }

        else if (!jsTree["Welcome Console 2"].is_string())
        {
            ::std::printf("'Welcome Console 2' Is Now '%s'\n", ::g_strWelcome_2.c_str());
        }

        else
        {
            ::g_strWelcome_2 = jsTree["Welcome Console 2"].get < ::std::string >();

            if (!::g_strWelcome_2.empty())
            {
                ::g_strWelcome_2.shrink_to_fit();

                if (::g_strWelcome_2.length() > 190)
                {
                    ::g_strWelcome_2[::SafeErasePos(::g_strWelcome_2, 190)] = { };
                    {
                        ::g_strWelcome_2.erase(::SafeErasePos(::g_strWelcome_2, 190));
                        {
                            ::g_strWelcome_2.shrink_to_fit();
                        }
                    }
                }

                else if (::g_strWelcome_2.size() > 190)
                {
                    ::g_strWelcome_2[::SafeErasePos(::g_strWelcome_2, 190)] = { };
                    {
                        ::g_strWelcome_2.erase(::SafeErasePos(::g_strWelcome_2, 190));
                        {
                            ::g_strWelcome_2.shrink_to_fit();
                        }
                    }
                }

                else
                {

                }
            }

            else
            {
                ::g_strWelcome_2.shrink_to_fit();
            }

            ::std::printf("'Welcome Console 2' Is Now '%s'\n", ::g_strWelcome_2.c_str());
        }
    }

    {
        if (jsTree["Welcome Console 3"].empty())
        {
            ::std::printf("'Welcome Console 3' Is Now '%s'\n", ::g_strWelcome_3.c_str());
        }

        else if (!jsTree["Welcome Console 3"].is_string())
        {
            ::std::printf("'Welcome Console 3' Is Now '%s'\n", ::g_strWelcome_3.c_str());
        }

        else
        {
            ::g_strWelcome_3 = jsTree["Welcome Console 3"].get < ::std::string >();

            if (!::g_strWelcome_3.empty())
            {
                ::g_strWelcome_3.shrink_to_fit();

                if (::g_strWelcome_3.length() > 190)
                {
                    ::g_strWelcome_3[::SafeErasePos(::g_strWelcome_3, 190)] = { };
                    {
                        ::g_strWelcome_3.erase(::SafeErasePos(::g_strWelcome_3, 190));
                        {
                            ::g_strWelcome_3.shrink_to_fit();
                        }
                    }
                }

                else if (::g_strWelcome_3.size() > 190)
                {
                    ::g_strWelcome_3[::SafeErasePos(::g_strWelcome_3, 190)] = { };
                    {
                        ::g_strWelcome_3.erase(::SafeErasePos(::g_strWelcome_3, 190));
                        {
                            ::g_strWelcome_3.shrink_to_fit();
                        }
                    }
                }

                else
                {

                }
            }

            else
            {
                ::g_strWelcome_3.shrink_to_fit();
            }

            ::std::printf("'Welcome Console 3' Is Now '%s'\n", ::g_strWelcome_3.c_str());
        }
    }

    {
        if (jsTree["Welcome Console 4"].empty())
        {
            ::std::printf("'Welcome Console 4' Is Now '%s'\n", ::g_strWelcome_4.c_str());
        }

        else if (!jsTree["Welcome Console 4"].is_string())
        {
            ::std::printf("'Welcome Console 4' Is Now '%s'\n", ::g_strWelcome_4.c_str());
        }

        else
        {
            ::g_strWelcome_4 = jsTree["Welcome Console 4"].get < ::std::string >();

            if (!::g_strWelcome_4.empty())
            {
                ::g_strWelcome_4.shrink_to_fit();

                if (::g_strWelcome_4.length() > 190)
                {
                    ::g_strWelcome_4[::SafeErasePos(::g_strWelcome_4, 190)] = { };
                    {
                        ::g_strWelcome_4.erase(::SafeErasePos(::g_strWelcome_4, 190));
                        {
                            ::g_strWelcome_4.shrink_to_fit();
                        }
                    }
                }

                else if (::g_strWelcome_4.size() > 190)
                {
                    ::g_strWelcome_4[::SafeErasePos(::g_strWelcome_4, 190)] = { };
                    {
                        ::g_strWelcome_4.erase(::SafeErasePos(::g_strWelcome_4, 190));
                        {
                            ::g_strWelcome_4.shrink_to_fit();
                        }
                    }
                }

                else
                {

                }
            }

            else
            {
                ::g_strWelcome_4.shrink_to_fit();
            }

            ::std::printf("'Welcome Console 4' Is Now '%s'\n", ::g_strWelcome_4.c_str());
        }
    }

    {
        if (jsTree["Welcome Console 5"].empty())
        {
            ::std::printf("'Welcome Console 5' Is Now '%s'\n", ::g_strWelcome_5.c_str());
        }

        else if (!jsTree["Welcome Console 5"].is_string())
        {
            ::std::printf("'Welcome Console 5' Is Now '%s'\n", ::g_strWelcome_5.c_str());
        }

        else
        {
            ::g_strWelcome_5 = jsTree["Welcome Console 5"].get < ::std::string >();

            if (!::g_strWelcome_5.empty())
            {
                ::g_strWelcome_5.shrink_to_fit();

                if (::g_strWelcome_5.length() > 190)
                {
                    ::g_strWelcome_5[::SafeErasePos(::g_strWelcome_5, 190)] = { };
                    {
                        ::g_strWelcome_5.erase(::SafeErasePos(::g_strWelcome_5, 190));
                        {
                            ::g_strWelcome_5.shrink_to_fit();
                        }
                    }
                }

                else if (::g_strWelcome_5.size() > 190)
                {
                    ::g_strWelcome_5[::SafeErasePos(::g_strWelcome_5, 190)] = { };
                    {
                        ::g_strWelcome_5.erase(::SafeErasePos(::g_strWelcome_5, 190));
                        {
                            ::g_strWelcome_5.shrink_to_fit();
                        }
                    }
                }

                else
                {

                }
            }

            else
            {
                ::g_strWelcome_5.shrink_to_fit();
            }

            ::std::printf("'Welcome Console 5' Is Now '%s'\n", ::g_strWelcome_5.c_str());
        }
    }

    {
        if (jsTree["Welcome Console 6"].empty())
        {
            ::std::printf("'Welcome Console 6' Is Now '%s'\n", ::g_strWelcome_6.c_str());
        }

        else if (!jsTree["Welcome Console 6"].is_string())
        {
            ::std::printf("'Welcome Console 6' Is Now '%s'\n", ::g_strWelcome_6.c_str());
        }

        else
        {
            ::g_strWelcome_6 = jsTree["Welcome Console 6"].get < ::std::string >();

            if (!::g_strWelcome_6.empty())
            {
                ::g_strWelcome_6.shrink_to_fit();

                if (::g_strWelcome_6.length() > 190)
                {
                    ::g_strWelcome_6[::SafeErasePos(::g_strWelcome_6, 190)] = { };
                    {
                        ::g_strWelcome_6.erase(::SafeErasePos(::g_strWelcome_6, 190));
                        {
                            ::g_strWelcome_6.shrink_to_fit();
                        }
                    }
                }

                else if (::g_strWelcome_6.size() > 190)
                {
                    ::g_strWelcome_6[::SafeErasePos(::g_strWelcome_6, 190)] = { };
                    {
                        ::g_strWelcome_6.erase(::SafeErasePos(::g_strWelcome_6, 190));
                        {
                            ::g_strWelcome_6.shrink_to_fit();
                        }
                    }
                }

                else
                {

                }
            }

            else
            {
                ::g_strWelcome_6.shrink_to_fit();
            }

            ::std::printf("'Welcome Console 6' Is Now '%s'\n", ::g_strWelcome_6.c_str());
        }
    }

    if (jsTree["Welcome Notify 1"].empty())
    {
        ::std::printf("'Welcome Notify 1' Is Now '%s'\n", ::g_strNotify_1.c_str());
    }

    else if (!jsTree["Welcome Notify 1"].is_string())
    {
        ::std::printf("'Welcome Notify 1' Is Now '%s'\n", ::g_strNotify_1.c_str());
    }

    else
    {
        ::g_strNotify_1 = jsTree["Welcome Notify 1"].get < ::std::string >();

        if (!::g_strNotify_1.empty())
        {
            ::g_strNotify_1.shrink_to_fit();

            if (::g_strNotify_1.length() > 190)
            {
                ::g_strNotify_1[::SafeErasePos(::g_strNotify_1, 190)] = { };
                {
                    ::g_strNotify_1.erase(::SafeErasePos(::g_strNotify_1, 190));
                    {
                        ::g_strNotify_1.shrink_to_fit();
                    }
                }
            }

            else if (::g_strNotify_1.size() > 190)
            {
                ::g_strNotify_1[::SafeErasePos(::g_strNotify_1, 190)] = { };
                {
                    ::g_strNotify_1.erase(::SafeErasePos(::g_strNotify_1, 190));
                    {
                        ::g_strNotify_1.shrink_to_fit();
                    }
                }
            }

            else
            {

            }
        }

        else
        {
            ::g_strNotify_1.shrink_to_fit();
        }

        ::std::printf("'Welcome Notify 1' Is Now '%s'\n", ::g_strNotify_1.c_str());
    }

    if (jsTree["Welcome Notify 2"].empty())
    {
        ::std::printf("'Welcome Notify 2' Is Now '%s'\n", ::g_strNotify_2.c_str());
    }

    else if (!jsTree["Welcome Notify 2"].is_string())
    {
        ::std::printf("'Welcome Notify 2' Is Now '%s'\n", ::g_strNotify_2.c_str());
    }

    else
    {
        ::g_strNotify_2 = jsTree["Welcome Notify 2"].get < ::std::string >();

        if (!::g_strNotify_2.empty())
        {
            ::g_strNotify_2.shrink_to_fit();

            if (::g_strNotify_2.length() > 190)
            {
                ::g_strNotify_2[::SafeErasePos(::g_strNotify_2, 190)] = { };
                {
                    ::g_strNotify_2.erase(::SafeErasePos(::g_strNotify_2, 190));
                    {
                        ::g_strNotify_2.shrink_to_fit();
                    }
                }
            }

            else if (::g_strNotify_2.size() > 190)
            {
                ::g_strNotify_2[::SafeErasePos(::g_strNotify_2, 190)] = { };
                {
                    ::g_strNotify_2.erase(::SafeErasePos(::g_strNotify_2, 190));
                    {
                        ::g_strNotify_2.shrink_to_fit();
                    }
                }
            }

            else
            {

            }
        }

        else
        {
            ::g_strNotify_2.shrink_to_fit();
        }

        ::std::printf("'Welcome Notify 2' Is Now '%s'\n", ::g_strNotify_2.c_str());
    }

    if (jsTree["Hide Radar"].empty())
    {
        ::std::printf("'Hide Radar' Is Now '%s'\n", ::g_bHideRadar ? "TRUE" : "FALSE");
    }

    else if (!jsTree["Hide Radar"].is_boolean())
    {
        ::std::printf("'Hide Radar' Is Now '%s'\n", ::g_bHideRadar ? "TRUE" : "FALSE");
    }

    else
    {
        ::g_bHideRadar = jsTree["Hide Radar"].get < bool >();
        {
            ::std::printf("'Hide Radar' Is Now '%s'\n", ::g_bHideRadar ? "TRUE" : "FALSE");
        }
    }

    if (jsTree["Give Armor In Spawn"].empty())
    {
        ::std::printf("'Give Armor In Spawn' Is Now '%d'\n", ::g_nArmor);
    }

    else if (!jsTree["Give Armor In Spawn"].is_number())
    {
        ::std::printf("'Give Armor In Spawn' Is Now '%d'\n", ::g_nArmor);
    }

    else
    {
        ::g_nArmor = ::ValClamp(jsTree["Give Armor In Spawn"].get < int >(), { }, 2);
        {
            ::std::printf("'Give Armor In Spawn' Is Now '%d'\n", ::g_nArmor);
        }
    }

#ifdef WIN32

    if (jsTree["Disable Priority Boost"].empty())
    {
        nDisablePrioBoost = { };
        {
            ::GetProcessPriorityBoost(::GetCurrentProcess(), &nDisablePrioBoost);
            {
                ::std::printf("'Disable Priority Boost' Is Now '%s'\n", xTo(nDisablePrioBoost, bool) ? "TRUE" : "FALSE");
            }
        }
    }

    else if (!jsTree["Disable Priority Boost"].is_boolean())
    {
        nDisablePrioBoost = { };
        {
            ::GetProcessPriorityBoost(::GetCurrentProcess(), &nDisablePrioBoost);
            {
                ::std::printf("'Disable Priority Boost' Is Now '%s'\n", xTo(nDisablePrioBoost, bool) ? "TRUE" : "FALSE");
            }
        }
    }

    else
    {
        bDisablePrioBoost = jsTree["Disable Priority Boost"].get < bool >();
        {
            ::std::printf("'Disable Priority Boost' Is Now '%s'\n", bDisablePrioBoost ? "TRUE" : "FALSE");
            {
                ::SetProcessPriorityBoost(::GetCurrentProcess(), xTo(bDisablePrioBoost, int));
            }
        }
    }

#endif

#ifdef WIN32

    if (jsTree["Windows Priority Class"].empty())

#else

    if (jsTree["Linux Priority Class"].empty())

#endif

    {

#ifdef WIN32

        ::std::printf("'Windows Priority Class' Is Now '%d'\n", xTo(::GetPriorityClass(::GetCurrentProcess()), int));

#else

        ::std::printf("'Linux Priority Class' Is Now '%d'\n", ::getpriority(::PRIO_PROCESS, ::getpid()));

#endif

    }

#ifdef WIN32

    else if (!jsTree["Windows Priority Class"].is_number())

#else

    else if (!jsTree["Linux Priority Class"].is_number())

#endif

    {

#ifdef WIN32

        ::std::printf("'Windows Priority Class' Is Now '%d'\n", xTo(::GetPriorityClass(::GetCurrentProcess()), int));

#else

        ::std::printf("'Linux Priority Class' Is Now '%d'\n", ::getpriority(::PRIO_PROCESS, ::getpid()));

#endif

    }

    else
    {

#ifdef WIN32

        nPrio = jsTree["Windows Priority Class"].get < int >();
        {
            ::std::printf("'Windows Priority Class' Is Now '%d'\n", nPrio);
            {
                ::SetPriorityClass(::GetCurrentProcess(), xTo(nPrio, unsigned long));
            }
        }

#else

        nPrio = jsTree["Linux Priority Class"].get < int >();
        {
            ::std::printf("'Linux Priority Class' Is Now '%d'\n", nPrio);
            {
                ::setpriority(::PRIO_PROCESS, ::getpid(), nPrio);
            }
        }

#endif

    }

    jsTree.clear();
    {
        ::std::printf("Loaded '%s'\n", strPath.c_str());
    }

    if (!::g_bNoReturn)
    {
        do
        {
            ::gpMetaGlobals->mres = ::MRES_IGNORED;

            return;
        }

        while (false);
    }

    else
    {
        return;
    }
}

void Hook_KeyValue(::edict_t* pEntity, ::KeyValueData* pKvData) noexcept
{
    static ::std::string strClass{ }, strMap{ }, strPath{ };
    static ::std::ifstream inFile{ };
    static ::nlohmann::json jsTree{ };
    static ::KeyValueData kvData{ };

    static int nEntity{ };
    static bool bError{ };
    static char szClassName[256]{ }, szKeyName[256]{ }, szValue[256]{ };

    if (!::g_bKvEditor)
    {
        strPath.assign(::g_strGameDir + "/addons/hgsm/kv_map_editor.cfg");
        {
            bError = true;
        }

        if (::FileExists(strPath))
        {
            inFile.open(strPath);

            if (inFile.is_open())
            {
                jsTree.clear();
                {
                    jsTree = ::nlohmann::json::parse(inFile, nullptr, { }, true);
                    {
                        inFile.close();

                        inFile.clear();
                    }
                }

                strMap.assign(STRING(::gpGlobals->mapname));

                if (jsTree.is_discarded())
                {
                    jsTree.clear();
                    {
                        ::std::printf("'%s' Has INVALID 'JSON'\n", strPath.c_str());
                    }
                }

                else if (jsTree.empty())
                {
                    jsTree.clear();
                    {
                        ::std::printf("Loaded '%s'\n", strPath.c_str());
                    }
                }

                else if (!jsTree.is_object())
                {
                    jsTree.clear();
                    {
                        ::std::printf("Loaded '%s'\n", strPath.c_str());
                    }
                }

                else if (!strMap.empty())
                {
                    bError = false;
                    {
                        ::std::printf("Loaded '%s'\n", strPath.c_str());
                    }
                }

                else
                {
                    jsTree.clear();
                    {
                        ::std::printf("Loaded '%s'\n", strPath.c_str());
                    }
                }
            }

            else
            {
                inFile.clear();
                {
                    ::std::printf("Failed To Open '%s'\n", strPath.c_str());
                }
            }
        }

        else
        {
            ::std::printf("Failed To Find '%s'\n", strPath.c_str());
        }

        ::g_bKvEditor = true;
    }

    if (pEntity)
    {
        if (pKvData)
        {
            if (pKvData->szKeyName)
            {
                if (*pKvData->szKeyName)
                {
                    if ((nEntity = ::ENTINDEX(pEntity)) > -1)
                    {
                        ::g_mapKeyValues[nEntity][pKvData->szKeyName] = pKvData->szValue ? pKvData->szValue : "";
                    }
                }
            }
        }
    }

    if (!bError)
    {
        if (pEntity)
        {
            if (pKvData)
            {
                if (pKvData->szClassName)
                {
                    if (*pKvData->szClassName)
                    {
                        if (pKvData->szKeyName)
                        {
                            if (*pKvData->szKeyName)
                            {
                                if (pKvData->szValue)
                                {
                                    if (*pKvData->szValue)
                                    {
                                        if (!pKvData->fHandled)
                                        {
                                            strClass.assign(STRING(pEntity->v.classname));

                                            if (!strClass.empty())
                                            {
                                                for (auto& jsItem : jsTree)
                                                {
                                                    if (!jsItem.empty())
                                                    {
                                                        if (jsItem.is_object())
                                                        {
                                                            if (!jsItem["map"].empty())
                                                            {
                                                                if (jsItem["map"].is_string())
                                                                {
                                                                    if (!strMap.compare(jsItem["map"].get < ::std::string >()) || (jsItem["map"].get < ::std::string >()).empty())
                                                                    {
                                                                        if (!jsItem["classname"].empty())
                                                                        {
                                                                            if (jsItem["classname"].is_string())
                                                                            {
                                                                                if (!strClass.compare(jsItem["classname"].get < ::std::string >()) || (jsItem["classname"].get < ::std::string >()).empty())
                                                                                {
                                                                                    if (!jsItem["kv_classname"].empty())
                                                                                    {
                                                                                        if (jsItem["kv_classname"].is_string())
                                                                                        {
                                                                                            if (!((jsItem["kv_classname"].get < ::std::string >()).compare(pKvData->szClassName)) || (jsItem["kv_classname"].get < ::std::string >()).empty())
                                                                                            {
                                                                                                if (!jsItem["kv_classname_new"].empty())
                                                                                                {
                                                                                                    if (jsItem["kv_classname_new"].is_string())
                                                                                                    {
                                                                                                        if (!jsItem["value"].empty())
                                                                                                        {
                                                                                                            if (jsItem["value"].is_string())
                                                                                                            {
                                                                                                                if (!((jsItem["value"].get < ::std::string >()).compare(pKvData->szValue)) || (jsItem["value"].get < ::std::string >()).empty())
                                                                                                                {
                                                                                                                    if (!jsItem["key"].empty())
                                                                                                                    {
                                                                                                                        if (jsItem["key"].is_string())
                                                                                                                        {
                                                                                                                            if (!((jsItem["key"].get < ::std::string >()).compare(pKvData->szKeyName)) || (jsItem["key"].get < ::std::string >()).empty())
                                                                                                                            {
                                                                                                                                if (!jsItem["key_new"].empty())
                                                                                                                                {
                                                                                                                                    if (jsItem["key_new"].is_string())
                                                                                                                                    {
                                                                                                                                        if (!jsItem["value_new"].empty())
                                                                                                                                        {
                                                                                                                                            if (jsItem["value_new"].is_string())
                                                                                                                                            {
                                                                                                                                                if ((jsItem["delete"].empty()) || !(jsItem["delete"].is_boolean()) || !(jsItem["delete"].get < bool >()))
                                                                                                                                                {
                                                                                                                                                    ::std::printf("IN '%s' REPLACED '%s: %s = %s' WITH '%s: %s = %s'\n", strClass.c_str(), pKvData->szClassName, pKvData->szKeyName, pKvData->szValue, (jsItem["kv_classname_new"].get < ::std::string >()).c_str(), (jsItem["key_new"].get < ::std::string >()).c_str(), (jsItem["value_new"].get < ::std::string >()).c_str());
                                                                                                                                                    {
                                                                                                                                                        ::std::snprintf(szClassName, sizeof szClassName, (jsItem["kv_classname_new"].get < ::std::string >()).c_str());
                                                                                                                                                        ::std::snprintf(szKeyName, sizeof szKeyName, (jsItem["key_new"].get < ::std::string >()).c_str());
                                                                                                                                                        ::std::snprintf(szValue, sizeof szValue, (jsItem["value_new"].get < ::std::string >()).c_str());
                                                                                                                                                        {
                                                                                                                                                            kvData.szClassName = szClassName;
                                                                                                                                                            kvData.szKeyName = szKeyName;
                                                                                                                                                            kvData.szValue = szValue;
                                                                                                                                                            {
                                                                                                                                                                kvData.fHandled = { };
                                                                                                                                                            }
                                                                                                                                                        }
                                                                                                                                                    }

                                                                                                                                                    {
                                                                                                                                                        ::gpGamedllFuncs->dllapi_table->pfnKeyValue(pEntity, &kvData);
                                                                                                                                                        {
                                                                                                                                                            ::std::printf("* THE CALL WAS '%s'\n", kvData.fHandled ? "HANDLED" : "UNHANDLED");
                                                                                                                                                        }
                                                                                                                                                    }
                                                                                                                                                }

                                                                                                                                                else
                                                                                                                                                {
                                                                                                                                                    ::std::printf("IN '%s' DELETED '%s: %s = %s'\n", strClass.c_str(), pKvData->szClassName, pKvData->szKeyName, pKvData->szValue);
                                                                                                                                                }

                                                                                                                                                {
                                                                                                                                                    do
                                                                                                                                                    {
                                                                                                                                                        ::gpMetaGlobals->mres = ::MRES_SUPERCEDE;

                                                                                                                                                        return;
                                                                                                                                                    }

                                                                                                                                                    while (false);
                                                                                                                                                }
                                                                                                                                            }
                                                                                                                                        }
                                                                                                                                    }
                                                                                                                                }
                                                                                                                            }
                                                                                                                        }
                                                                                                                    }
                                                                                                                }
                                                                                                            }
                                                                                                        }
                                                                                                    }
                                                                                                }
                                                                                            }
                                                                                        }
                                                                                    }
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    do
    {
        ::gpMetaGlobals->mres = ::MRES_IGNORED;

        return;
    }

    while (false);
}

void Hook_ClientDisconnect(::edict_t* pEntity) noexcept
{
    static int nEntity{ };

    if (pEntity)
    {
        if ((nEntity = ::ENTINDEX(pEntity)) > 0)
        {
            if (::g_bHideRadar)
            {
                ::g_bRadar[nEntity] = { };
            }

            if (::g_nArmor)
            {
                ::g_bArmor[nEntity] = { };
            }

            if (::g_bHideFlash)
            {
                if (::g_nHideWeapon > 0)
                {
                    if (::g_nCrosshair > 0)
                    {
                        ::g_bHide[nEntity] = { };
                    }
                }
            }

            else if (::g_bHideMoney)
            {
                if (::g_nHideWeapon > 0)
                {
                    if (::g_nCrosshair > 0)
                    {
                        ::g_bHide[nEntity] = { };
                    }
                }
            }

            else if (::g_bHideTimer)
            {
                if (::g_nHideWeapon > 0)
                {
                    if (::g_nCrosshair > 0)
                    {
                        ::g_bHide[nEntity] = { };
                    }
                }
            }

            else
            {

            }

            if (!::g_strWelcome_1.empty())
            {
                if (::g_strWelcome_1.length() <= 190)
                {
                    if (::g_strWelcome_1.size() <= 190)
                    {
                        if (::g_nTextMsg > 0)
                        {
                            ::g_bShow[nEntity] = { };
                        }
                    }
                }
            }
        }
    }

    do
    {
        ::gpMetaGlobals->mres = ::MRES_IGNORED;

        return;
    }

    while (false);
}

void Hook_ServerDeactivate_Post() noexcept
{
    static int nIter{ };

    {
        ::g_bStripper = { };
        ::g_bKvEditor = { };
        ::g_bKvAdder = { };

        {
            for (nIter = { }; nIter < 16384; nIter++)
            {
                ::g_mapKeyValues[nIter].clear();
            }
        }
    }

    do
    {
        ::gpMetaGlobals->mres = ::MRES_IGNORED;

        return;
    }

    while (false);
}

void Hook_ServerActivate_Post(::edict_t*, int, int) noexcept
{
    static ::std::string strPath{ }, strConVar{ };
    static ::std::ifstream inFile{ };
    static ::nlohmann::json jsTree{ };
    static ::cvar_t* pConVar{ };

    static bool bFlags{ };

    strPath.assign(::g_strGameDir + "/addons/hgsm/reveal_console_variables.cfg");

    if (!::FileExists(strPath))
    {
        ::std::printf("Failed To Find '%s'\n", strPath.c_str());
    }

    else
    {
        inFile.open(strPath);

        if (!inFile.is_open())
        {
            inFile.clear();
            {
                ::std::printf("Failed To Open '%s'\n", strPath.c_str());
            }
        }

        else
        {
            jsTree.clear();
            {
                jsTree = ::nlohmann::json::parse(inFile, nullptr, { }, true);
                {
                    inFile.close();

                    inFile.clear();
                }
            }

            if (jsTree.is_discarded())
            {
                jsTree.clear();
                {
                    ::std::printf("'%s' Has INVALID 'JSON'\n", strPath.c_str());
                }
            }

            else if (!jsTree.empty())
            {
                if (jsTree.is_object())
                {
                    if (!jsTree["Console Variables"].empty())
                    {
                        if (jsTree["Console Variables"].is_array())
                        {
                            for (auto& jsItem : jsTree["Console Variables"])
                            {
                                if (!jsItem.empty())
                                {
                                    if (jsItem.is_string())
                                    {
                                        strConVar = jsItem.get < ::std::string >();

                                        if (!strConVar.empty())
                                        {
                                            if (pConVar = (*::g_engfuncs.pfnCVarGetPointer) (strConVar.c_str()))
                                            {
                                                ::std::printf("'%s' Has Value '%s' (~ %f)\n", strConVar.c_str(), pConVar->string, pConVar->value);

                                                if (pConVar->flags)
                                                {
                                                    bFlags = { };

                                                    if (pConVar->flags & FCVAR_CLIENTDLL)
                                                    {
                                                        ::std::printf("'%s' Has 'FCVAR_CLIENTDLL'\n", strConVar.c_str());

                                                        bFlags = true;
                                                    }

                                                    if (pConVar->flags & FCVAR_PROTECTED)
                                                    {
                                                        ::std::printf("'%s' Has 'FCVAR_PROTECTED'\n", strConVar.c_str());

                                                        bFlags = true;
                                                    }

                                                    if (pConVar->flags & FCVAR_ARCHIVE)
                                                    {
                                                        ::std::printf("'%s' Has 'FCVAR_ARCHIVE'\n", strConVar.c_str());

                                                        bFlags = true;
                                                    }

                                                    if (pConVar->flags & FCVAR_PRINTABLEONLY)
                                                    {
                                                        ::std::printf("'%s' Has 'FCVAR_PRINTABLEONLY'\n", strConVar.c_str());

                                                        bFlags = true;
                                                    }

                                                    if (pConVar->flags & FCVAR_SPONLY)
                                                    {
                                                        ::std::printf("'%s' Has 'FCVAR_SPONLY'\n", strConVar.c_str());

                                                        bFlags = true;
                                                    }

                                                    if (pConVar->flags & FCVAR_UNLOGGED)
                                                    {
                                                        ::std::printf("'%s' Has 'FCVAR_UNLOGGED'\n", strConVar.c_str());

                                                        bFlags = true;
                                                    }

                                                    if (pConVar->flags & FCVAR_USERINFO)
                                                    {
                                                        ::std::printf("'%s' Has 'FCVAR_USERINFO'\n", strConVar.c_str());

                                                        bFlags = true;
                                                    }

                                                    if (pConVar->flags & FCVAR_SERVER)
                                                    {
                                                        ::std::printf("'%s' Has 'FCVAR_SERVER'\n", strConVar.c_str());

                                                        bFlags = true;
                                                    }

                                                    if (pConVar->flags & FCVAR_EXTDLL)
                                                    {
                                                        ::std::printf("'%s' Has 'FCVAR_EXTDLL'\n", strConVar.c_str());

                                                        bFlags = true;
                                                    }

                                                    if (pConVar->flags & FCVAR_NOEXTRAWHITEPACE)
                                                    {
                                                        ::std::printf("'%s' Has 'FCVAR_NOEXTRAWHITEPACE'\n", strConVar.c_str());

                                                        bFlags = true;
                                                    }

                                                    if (!bFlags)
                                                    {
                                                        ::std::printf("'%s' Has Useless Flags\n", strConVar.c_str());
                                                    }
                                                }

                                                else
                                                {
                                                    ::std::printf("'%s' Has No Flags\n", strConVar.c_str());
                                                }
                                            }

                                            else
                                            {
                                                ::std::printf("INVALID '%s'\n", strConVar.c_str());
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            jsTree.clear();
            {
                ::std::printf("Loaded '%s'\n", strPath.c_str());
            }
        }
    }

    strPath.assign(::g_strGameDir + "/addons/hgsm/apply_console_variables.cfg");

    if (!::FileExists(strPath))
    {
        ::std::printf("Failed To Find '%s'\n", strPath.c_str());
        {
            if (!::g_bNoReturn)
            {
                do
                {
                    ::gpMetaGlobals->mres = ::MRES_IGNORED;

                    return;
                }

                while (false);
            }

            else
            {
                return;
            }
        }
    }

    inFile.open(strPath);

    if (!inFile.is_open())
    {
        ::std::printf("Failed To Open '%s'\n", strPath.c_str());
        {
            inFile.clear();
            {
                if (!::g_bNoReturn)
                {
                    do
                    {
                        ::gpMetaGlobals->mres = ::MRES_IGNORED;

                        return;
                    }

                    while (false);
                }

                else
                {
                    return;
                }
            }
        }
    }

    jsTree.clear();
    {
        jsTree = ::nlohmann::json::parse(inFile, nullptr, { }, true);
        {
            inFile.close();

            inFile.clear();
        }
    }

    if (jsTree.is_discarded())
    {
        ::std::printf("'%s' Has INVALID 'JSON'\n", strPath.c_str());
        {
            jsTree.clear();
            {
                if (!::g_bNoReturn)
                {
                    do
                    {
                        ::gpMetaGlobals->mres = ::MRES_IGNORED;

                        return;
                    }

                    while (false);
                }

                else
                {
                    return;
                }
            }
        }
    }

    else if (!jsTree.empty())
    {
        if (jsTree.is_object())
        {
            for (auto& jsItem : jsTree)
            {
                if (!jsItem.empty())
                {
                    if (jsItem.is_object())
                    {
                        if (!jsItem["Console Variable"].empty())
                        {
                            if (jsItem["Console Variable"].is_string())
                            {
                                if (!jsItem["String Value"].empty())
                                {
                                    if (jsItem["String Value"].is_string())
                                    {
                                        strConVar = jsItem["Console Variable"].get < ::std::string >();

                                        if (!strConVar.empty())
                                        {
                                            if (pConVar = (*::g_engfuncs.pfnCVarGetPointer) (strConVar.c_str()))
                                            {
                                                (*::g_engfuncs.pfnCvar_DirectSet) (pConVar, (jsItem["String Value"].get < ::std::string >()).c_str());
                                                {
                                                    ::std::printf("'%s' Is Now '%s' (~ %f)\n", strConVar.c_str(), pConVar->string, pConVar->value);
                                                }
                                            }

                                            else
                                            {
                                                ::std::printf("INVALID '%s'\n", strConVar.c_str());
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    jsTree.clear();
    {
        ::std::printf("Loaded '%s'\n", strPath.c_str());
    }

    if (!::g_bNoReturn)
    {
        do
        {
            ::gpMetaGlobals->mres = ::MRES_IGNORED;

            return;
        }

        while (false);
    }

    else
    {
        return;
    }
}

void Hook_StartFrame_Post() noexcept
{
    static long long llTime{ }, llStamp{ };
    static int nEntity{ }, nIter{ }, nFov{ }, nTeam{ }, nKevlar{ };
    static bool bAltered{ }, bMsgs{ };

    static ::std::string strCmd{ };

    static ::edict_t* pEntity{ };

    if (++nIter == ::g_nTicks)
    {
        if (!bMsgs)
        {
            if (!::g_nResetHud)
            {
                ::g_nResetHud = (*::gpMetaUtilFuncs->pfnGetUserMsgID) (&::Plugin_info, "ResetHUD", nullptr);
            }

            if (::g_bRealScore)
            {
                if (!::g_nDeathMsg)
                {
                    ::g_nDeathMsg = (*::gpMetaUtilFuncs->pfnGetUserMsgID) (&::Plugin_info, "DeathMsg", nullptr);
                }
            }

            if (!::g_strWelcome_1.empty())
            {
                if (::g_strWelcome_1.length() <= 190)
                {
                    if (::g_strWelcome_1.size() <= 190)
                    {
                        if (!::g_nTextMsg)
                        {
                            ::g_nTextMsg = (*::gpMetaUtilFuncs->pfnGetUserMsgID) (&::Plugin_info, "TextMsg", nullptr);
                        }
                    }
                }
            }

            if (::g_nArmor)
            {
                if (!::g_nArmorType)
                {
                    ::g_nArmorType = (*::gpMetaUtilFuncs->pfnGetUserMsgID) (&::Plugin_info, "ArmorType", nullptr);
                }
            }

            if (::g_bFixScore || ::g_bFixKills || ::g_bFixDeaths || ::g_bRealScore)
            {
                if (!::g_nScoreInfo)
                {
                    ::g_nScoreInfo = (*::gpMetaUtilFuncs->pfnGetUserMsgID) (&::Plugin_info, "ScoreInfo", nullptr);
                }

                if (!::g_strGameDir.compare("dod"))
                {
                    if (!::g_nScoreShort)
                    {
                        ::g_nScoreShort = (*::gpMetaUtilFuncs->pfnGetUserMsgID) (&::Plugin_info, "ScoreShort", nullptr);
                    }
                }
            }

            if (::g_bHideMoney)
            {
                if (!::g_nHideWeapon)
                {
                    ::g_nHideWeapon = (*::gpMetaUtilFuncs->pfnGetUserMsgID) (&::Plugin_info, "HideWeapon", nullptr);
                }

                if (!::g_nCrosshair)
                {
                    ::g_nCrosshair = (*::gpMetaUtilFuncs->pfnGetUserMsgID) (&::Plugin_info, "Crosshair", nullptr);
                }
            }

            else if (::g_bHideTimer)
            {
                if (!::g_nHideWeapon)
                {
                    ::g_nHideWeapon = (*::gpMetaUtilFuncs->pfnGetUserMsgID) (&::Plugin_info, "HideWeapon", nullptr);
                }

                if (!::g_nCrosshair)
                {
                    ::g_nCrosshair = (*::gpMetaUtilFuncs->pfnGetUserMsgID) (&::Plugin_info, "Crosshair", nullptr);
                }
            }

            else if (::g_bHideFlash)
            {
                if (!::g_nHideWeapon)
                {
                    ::g_nHideWeapon = (*::gpMetaUtilFuncs->pfnGetUserMsgID) (&::Plugin_info, "HideWeapon", nullptr);
                }

                if (!::g_nCrosshair)
                {
                    ::g_nCrosshair = (*::gpMetaUtilFuncs->pfnGetUserMsgID) (&::Plugin_info, "Crosshair", nullptr);
                }
            }

            else
            {

            }

            bMsgs = true;
        }

        for (nEntity = xTo(true, int); nEntity <= ::gpGlobals->maxClients; nEntity++)
        {
            if (pEntity = ::INDEXENT(nEntity))
            {
                if (!pEntity->free)
                {
                    if (pEntity->pvPrivateData)
                    {
                        if (!pEntity->serialnumber)
                        {
                            if (!(pEntity->v.flags & FL_DORMANT))
                            {
                                if (pEntity->v.flags & FL_CLIENT)
                                {
                                    if (::g_nTextMsg > 0)
                                    {
                                        if (!::g_strWelcome_1.empty())
                                        {
                                            if (::g_strWelcome_1.length() <= 190)
                                            {
                                                if (::g_strWelcome_1.size() <= 190)
                                                {
                                                    if (!::g_strGameDir.compare("dod"))
                                                    {
                                                        if (((nTeam = *((char*)pEntity->pvPrivateData + ::m_iDodTeam + ::g_nOsOffs)) == ((int)('a'))) || (nTeam == ((int)('A'))))
                                                        {
                                                            if (!::g_bShow[nEntity])
                                                            {
                                                                (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                {
                                                                    (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                    (*::g_engfuncs.pfnWriteString) (::g_strWelcome_1.c_str());
                                                                }
                                                                (*::g_engfuncs.pfnMessageEnd) ();

                                                                if (!::g_strWelcome_2.empty())
                                                                {
                                                                    if (::g_strWelcome_2.length() <= 190)
                                                                    {
                                                                        if (::g_strWelcome_2.size() <= 190)
                                                                        {
                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                            {
                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                (*::g_engfuncs.pfnWriteString) (::g_strWelcome_2.c_str());
                                                                            }
                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                        }
                                                                    }
                                                                }

                                                                if (!::g_strWelcome_3.empty())
                                                                {
                                                                    if (::g_strWelcome_3.length() <= 190)
                                                                    {
                                                                        if (::g_strWelcome_3.size() <= 190)
                                                                        {
                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                            {
                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                (*::g_engfuncs.pfnWriteString) (::g_strWelcome_3.c_str());
                                                                            }
                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                        }
                                                                    }
                                                                }

                                                                if (!::g_strWelcome_4.empty())
                                                                {
                                                                    if (::g_strWelcome_4.length() <= 190)
                                                                    {
                                                                        if (::g_strWelcome_4.size() <= 190)
                                                                        {
                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                            {
                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                (*::g_engfuncs.pfnWriteString) (::g_strWelcome_4.c_str());
                                                                            }
                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                        }
                                                                    }
                                                                }

                                                                if (!::g_strWelcome_5.empty())
                                                                {
                                                                    if (::g_strWelcome_5.length() <= 190)
                                                                    {
                                                                        if (::g_strWelcome_5.size() <= 190)
                                                                        {
                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                            {
                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                (*::g_engfuncs.pfnWriteString) (::g_strWelcome_5.c_str());
                                                                            }
                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                        }
                                                                    }
                                                                }

                                                                if (!::g_strWelcome_6.empty())
                                                                {
                                                                    if (::g_strWelcome_6.length() <= 190)
                                                                    {
                                                                        if (::g_strWelcome_6.size() <= 190)
                                                                        {
                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                            {
                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                (*::g_engfuncs.pfnWriteString) (::g_strWelcome_6.c_str());
                                                                            }
                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                        }
                                                                    }
                                                                }

                                                                if (!::g_strNotify_1.empty())
                                                                {
                                                                    if (::g_strNotify_1.length() <= 190)
                                                                    {
                                                                        if (::g_strNotify_1.size() <= 190)
                                                                        {
                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                            {
                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTTALK);

                                                                                (*::g_engfuncs.pfnWriteString) (::g_strNotify_1.c_str());
                                                                            }
                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                        }
                                                                    }
                                                                }

                                                                if (!::g_strNotify_2.empty())
                                                                {
                                                                    if (::g_strNotify_2.length() <= 190)
                                                                    {
                                                                        if (::g_strNotify_2.size() <= 190)
                                                                        {
                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                            {
                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTTALK);

                                                                                (*::g_engfuncs.pfnWriteString) (::g_strNotify_2.c_str());
                                                                            }
                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                        }
                                                                    }
                                                                }

                                                                ::g_bShow[nEntity] = true;
                                                            }
                                                        }

                                                        else if ((nTeam == ((int)('b'))) || (nTeam == ((int)('B'))))
                                                        {
                                                            if (!::g_bShow[nEntity])
                                                            {
                                                                (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                {
                                                                    (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                    (*::g_engfuncs.pfnWriteString) (::g_strWelcome_1.c_str());
                                                                }
                                                                (*::g_engfuncs.pfnMessageEnd) ();

                                                                if (!::g_strWelcome_2.empty())
                                                                {
                                                                    if (::g_strWelcome_2.length() <= 190)
                                                                    {
                                                                        if (::g_strWelcome_2.size() <= 190)
                                                                        {
                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                            {
                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                (*::g_engfuncs.pfnWriteString) (::g_strWelcome_2.c_str());
                                                                            }
                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                        }
                                                                    }
                                                                }

                                                                if (!::g_strWelcome_3.empty())
                                                                {
                                                                    if (::g_strWelcome_3.length() <= 190)
                                                                    {
                                                                        if (::g_strWelcome_3.size() <= 190)
                                                                        {
                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                            {
                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                (*::g_engfuncs.pfnWriteString) (::g_strWelcome_3.c_str());
                                                                            }
                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                        }
                                                                    }
                                                                }

                                                                if (!::g_strWelcome_4.empty())
                                                                {
                                                                    if (::g_strWelcome_4.length() <= 190)
                                                                    {
                                                                        if (::g_strWelcome_4.size() <= 190)
                                                                        {
                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                            {
                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                (*::g_engfuncs.pfnWriteString) (::g_strWelcome_4.c_str());
                                                                            }
                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                        }
                                                                    }
                                                                }

                                                                if (!::g_strWelcome_5.empty())
                                                                {
                                                                    if (::g_strWelcome_5.length() <= 190)
                                                                    {
                                                                        if (::g_strWelcome_5.size() <= 190)
                                                                        {
                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                            {
                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                (*::g_engfuncs.pfnWriteString) (::g_strWelcome_5.c_str());
                                                                            }
                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                        }
                                                                    }
                                                                }

                                                                if (!::g_strWelcome_6.empty())
                                                                {
                                                                    if (::g_strWelcome_6.length() <= 190)
                                                                    {
                                                                        if (::g_strWelcome_6.size() <= 190)
                                                                        {
                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                            {
                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                (*::g_engfuncs.pfnWriteString) (::g_strWelcome_6.c_str());
                                                                            }
                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                        }
                                                                    }
                                                                }

                                                                if (!::g_strNotify_1.empty())
                                                                {
                                                                    if (::g_strNotify_1.length() <= 190)
                                                                    {
                                                                        if (::g_strNotify_1.size() <= 190)
                                                                        {
                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                            {
                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTTALK);

                                                                                (*::g_engfuncs.pfnWriteString) (::g_strNotify_1.c_str());
                                                                            }
                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                        }
                                                                    }
                                                                }

                                                                if (!::g_strNotify_2.empty())
                                                                {
                                                                    if (::g_strNotify_2.length() <= 190)
                                                                    {
                                                                        if (::g_strNotify_2.size() <= 190)
                                                                        {
                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                            {
                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTTALK);

                                                                                (*::g_engfuncs.pfnWriteString) (::g_strNotify_2.c_str());
                                                                            }
                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                        }
                                                                    }
                                                                }

                                                                ::g_bShow[nEntity] = true;
                                                            }
                                                        }

                                                        else if ((nTeam == ((int)('s'))) || (nTeam == ((int)('S'))))
                                                        {
                                                            if (!::g_bShow[nEntity])
                                                            {
                                                                (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                {
                                                                    (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                    (*::g_engfuncs.pfnWriteString) (::g_strWelcome_1.c_str());
                                                                }
                                                                (*::g_engfuncs.pfnMessageEnd) ();

                                                                if (!::g_strWelcome_2.empty())
                                                                {
                                                                    if (::g_strWelcome_2.length() <= 190)
                                                                    {
                                                                        if (::g_strWelcome_2.size() <= 190)
                                                                        {
                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                            {
                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                (*::g_engfuncs.pfnWriteString) (::g_strWelcome_2.c_str());
                                                                            }
                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                        }
                                                                    }
                                                                }

                                                                if (!::g_strWelcome_3.empty())
                                                                {
                                                                    if (::g_strWelcome_3.length() <= 190)
                                                                    {
                                                                        if (::g_strWelcome_3.size() <= 190)
                                                                        {
                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                            {
                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                (*::g_engfuncs.pfnWriteString) (::g_strWelcome_3.c_str());
                                                                            }
                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                        }
                                                                    }
                                                                }

                                                                if (!::g_strWelcome_4.empty())
                                                                {
                                                                    if (::g_strWelcome_4.length() <= 190)
                                                                    {
                                                                        if (::g_strWelcome_4.size() <= 190)
                                                                        {
                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                            {
                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                (*::g_engfuncs.pfnWriteString) (::g_strWelcome_4.c_str());
                                                                            }
                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                        }
                                                                    }
                                                                }

                                                                if (!::g_strWelcome_5.empty())
                                                                {
                                                                    if (::g_strWelcome_5.length() <= 190)
                                                                    {
                                                                        if (::g_strWelcome_5.size() <= 190)
                                                                        {
                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                            {
                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                (*::g_engfuncs.pfnWriteString) (::g_strWelcome_5.c_str());
                                                                            }
                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                        }
                                                                    }
                                                                }

                                                                if (!::g_strWelcome_6.empty())
                                                                {
                                                                    if (::g_strWelcome_6.length() <= 190)
                                                                    {
                                                                        if (::g_strWelcome_6.size() <= 190)
                                                                        {
                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                            {
                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                (*::g_engfuncs.pfnWriteString) (::g_strWelcome_6.c_str());
                                                                            }
                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                        }
                                                                    }
                                                                }

                                                                if (!::g_strNotify_1.empty())
                                                                {
                                                                    if (::g_strNotify_1.length() <= 190)
                                                                    {
                                                                        if (::g_strNotify_1.size() <= 190)
                                                                        {
                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                            {
                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTTALK);

                                                                                (*::g_engfuncs.pfnWriteString) (::g_strNotify_1.c_str());
                                                                            }
                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                        }
                                                                    }
                                                                }

                                                                if (!::g_strNotify_2.empty())
                                                                {
                                                                    if (::g_strNotify_2.length() <= 190)
                                                                    {
                                                                        if (::g_strNotify_2.size() <= 190)
                                                                        {
                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                            {
                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTTALK);

                                                                                (*::g_engfuncs.pfnWriteString) (::g_strNotify_2.c_str());
                                                                            }
                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                        }
                                                                    }
                                                                }

                                                                ::g_bShow[nEntity] = true;
                                                            }
                                                        }

                                                        else if (!nTeam)
                                                        {

                                                        }

                                                        else if ((nTeam == ((int)('u'))) || (nTeam == ((int)('U'))))
                                                        {

                                                        }

                                                        else if ((nTeam == ((int)('n'))) || (nTeam == ((int)('N'))))
                                                        {

                                                        }

                                                        else
                                                        {

                                                        }
                                                    }

                                                    else
                                                    {
                                                        if ((nTeam = *((int*)pEntity->pvPrivateData + ::m_iTeam + ::g_nOsOffs)) == 1)
                                                        {
                                                            if (!::g_bShow[nEntity])
                                                            {
                                                                (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                {
                                                                    (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                    (*::g_engfuncs.pfnWriteString) (::g_strWelcome_1.c_str());
                                                                }
                                                                (*::g_engfuncs.pfnMessageEnd) ();

                                                                if (!::g_strWelcome_2.empty())
                                                                {
                                                                    if (::g_strWelcome_2.length() <= 190)
                                                                    {
                                                                        if (::g_strWelcome_2.size() <= 190)
                                                                        {
                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                            {
                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                (*::g_engfuncs.pfnWriteString) (::g_strWelcome_2.c_str());
                                                                            }
                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                        }
                                                                    }
                                                                }

                                                                if (!::g_strWelcome_3.empty())
                                                                {
                                                                    if (::g_strWelcome_3.length() <= 190)
                                                                    {
                                                                        if (::g_strWelcome_3.size() <= 190)
                                                                        {
                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                            {
                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                (*::g_engfuncs.pfnWriteString) (::g_strWelcome_3.c_str());
                                                                            }
                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                        }
                                                                    }
                                                                }

                                                                if (!::g_strWelcome_4.empty())
                                                                {
                                                                    if (::g_strWelcome_4.length() <= 190)
                                                                    {
                                                                        if (::g_strWelcome_4.size() <= 190)
                                                                        {
                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                            {
                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                (*::g_engfuncs.pfnWriteString) (::g_strWelcome_4.c_str());
                                                                            }
                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                        }
                                                                    }
                                                                }

                                                                if (!::g_strWelcome_5.empty())
                                                                {
                                                                    if (::g_strWelcome_5.length() <= 190)
                                                                    {
                                                                        if (::g_strWelcome_5.size() <= 190)
                                                                        {
                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                            {
                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                (*::g_engfuncs.pfnWriteString) (::g_strWelcome_5.c_str());
                                                                            }
                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                        }
                                                                    }
                                                                }

                                                                if (!::g_strWelcome_6.empty())
                                                                {
                                                                    if (::g_strWelcome_6.length() <= 190)
                                                                    {
                                                                        if (::g_strWelcome_6.size() <= 190)
                                                                        {
                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                            {
                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                (*::g_engfuncs.pfnWriteString) (::g_strWelcome_6.c_str());
                                                                            }
                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                        }
                                                                    }
                                                                }

                                                                if (!::g_strNotify_1.empty())
                                                                {
                                                                    if (::g_strNotify_1.length() <= 190)
                                                                    {
                                                                        if (::g_strNotify_1.size() <= 190)
                                                                        {
                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                            {
                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTTALK);

                                                                                (*::g_engfuncs.pfnWriteString) (::g_strNotify_1.c_str());
                                                                            }
                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                        }
                                                                    }
                                                                }

                                                                if (!::g_strNotify_2.empty())
                                                                {
                                                                    if (::g_strNotify_2.length() <= 190)
                                                                    {
                                                                        if (::g_strNotify_2.size() <= 190)
                                                                        {
                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                            {
                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTTALK);

                                                                                (*::g_engfuncs.pfnWriteString) (::g_strNotify_2.c_str());
                                                                            }
                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                        }
                                                                    }
                                                                }

                                                                ::g_bShow[nEntity] = true;
                                                            }
                                                        }

                                                        else if (nTeam == 2)
                                                        {
                                                            if (!::g_bShow[nEntity])
                                                            {
                                                                (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                {
                                                                    (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                    (*::g_engfuncs.pfnWriteString) (::g_strWelcome_1.c_str());
                                                                }
                                                                (*::g_engfuncs.pfnMessageEnd) ();

                                                                if (!::g_strWelcome_2.empty())
                                                                {
                                                                    if (::g_strWelcome_2.length() <= 190)
                                                                    {
                                                                        if (::g_strWelcome_2.size() <= 190)
                                                                        {
                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                            {
                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                (*::g_engfuncs.pfnWriteString) (::g_strWelcome_2.c_str());
                                                                            }
                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                        }
                                                                    }
                                                                }

                                                                if (!::g_strWelcome_3.empty())
                                                                {
                                                                    if (::g_strWelcome_3.length() <= 190)
                                                                    {
                                                                        if (::g_strWelcome_3.size() <= 190)
                                                                        {
                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                            {
                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                (*::g_engfuncs.pfnWriteString) (::g_strWelcome_3.c_str());
                                                                            }
                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                        }
                                                                    }
                                                                }

                                                                if (!::g_strWelcome_4.empty())
                                                                {
                                                                    if (::g_strWelcome_4.length() <= 190)
                                                                    {
                                                                        if (::g_strWelcome_4.size() <= 190)
                                                                        {
                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                            {
                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                (*::g_engfuncs.pfnWriteString) (::g_strWelcome_4.c_str());
                                                                            }
                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                        }
                                                                    }
                                                                }

                                                                if (!::g_strWelcome_5.empty())
                                                                {
                                                                    if (::g_strWelcome_5.length() <= 190)
                                                                    {
                                                                        if (::g_strWelcome_5.size() <= 190)
                                                                        {
                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                            {
                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                (*::g_engfuncs.pfnWriteString) (::g_strWelcome_5.c_str());
                                                                            }
                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                        }
                                                                    }
                                                                }

                                                                if (!::g_strWelcome_6.empty())
                                                                {
                                                                    if (::g_strWelcome_6.length() <= 190)
                                                                    {
                                                                        if (::g_strWelcome_6.size() <= 190)
                                                                        {
                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                            {
                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                (*::g_engfuncs.pfnWriteString) (::g_strWelcome_6.c_str());
                                                                            }
                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                        }
                                                                    }
                                                                }

                                                                if (!::g_strNotify_1.empty())
                                                                {
                                                                    if (::g_strNotify_1.length() <= 190)
                                                                    {
                                                                        if (::g_strNotify_1.size() <= 190)
                                                                        {
                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                            {
                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTTALK);

                                                                                (*::g_engfuncs.pfnWriteString) (::g_strNotify_1.c_str());
                                                                            }
                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                        }
                                                                    }
                                                                }

                                                                if (!::g_strNotify_2.empty())
                                                                {
                                                                    if (::g_strNotify_2.length() <= 190)
                                                                    {
                                                                        if (::g_strNotify_2.size() <= 190)
                                                                        {
                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                            {
                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTTALK);

                                                                                (*::g_engfuncs.pfnWriteString) (::g_strNotify_2.c_str());
                                                                            }
                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                        }
                                                                    }
                                                                }

                                                                ::g_bShow[nEntity] = true;
                                                            }
                                                        }

                                                        else if (nTeam == 3)
                                                        {
                                                            if (!::g_bShow[nEntity])
                                                            {
                                                                (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                {
                                                                    (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                    (*::g_engfuncs.pfnWriteString) (::g_strWelcome_1.c_str());
                                                                }
                                                                (*::g_engfuncs.pfnMessageEnd) ();

                                                                if (!::g_strWelcome_2.empty())
                                                                {
                                                                    if (::g_strWelcome_2.length() <= 190)
                                                                    {
                                                                        if (::g_strWelcome_2.size() <= 190)
                                                                        {
                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                            {
                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                (*::g_engfuncs.pfnWriteString) (::g_strWelcome_2.c_str());
                                                                            }
                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                        }
                                                                    }
                                                                }

                                                                if (!::g_strWelcome_3.empty())
                                                                {
                                                                    if (::g_strWelcome_3.length() <= 190)
                                                                    {
                                                                        if (::g_strWelcome_3.size() <= 190)
                                                                        {
                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                            {
                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                (*::g_engfuncs.pfnWriteString) (::g_strWelcome_3.c_str());
                                                                            }
                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                        }
                                                                    }
                                                                }

                                                                if (!::g_strWelcome_4.empty())
                                                                {
                                                                    if (::g_strWelcome_4.length() <= 190)
                                                                    {
                                                                        if (::g_strWelcome_4.size() <= 190)
                                                                        {
                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                            {
                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                (*::g_engfuncs.pfnWriteString) (::g_strWelcome_4.c_str());
                                                                            }
                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                        }
                                                                    }
                                                                }

                                                                if (!::g_strWelcome_5.empty())
                                                                {
                                                                    if (::g_strWelcome_5.length() <= 190)
                                                                    {
                                                                        if (::g_strWelcome_5.size() <= 190)
                                                                        {
                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                            {
                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                (*::g_engfuncs.pfnWriteString) (::g_strWelcome_5.c_str());
                                                                            }
                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                        }
                                                                    }
                                                                }

                                                                if (!::g_strWelcome_6.empty())
                                                                {
                                                                    if (::g_strWelcome_6.length() <= 190)
                                                                    {
                                                                        if (::g_strWelcome_6.size() <= 190)
                                                                        {
                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                            {
                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                (*::g_engfuncs.pfnWriteString) (::g_strWelcome_6.c_str());
                                                                            }
                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                        }
                                                                    }
                                                                }

                                                                if (!::g_strNotify_1.empty())
                                                                {
                                                                    if (::g_strNotify_1.length() <= 190)
                                                                    {
                                                                        if (::g_strNotify_1.size() <= 190)
                                                                        {
                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                            {
                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTTALK);

                                                                                (*::g_engfuncs.pfnWriteString) (::g_strNotify_1.c_str());
                                                                            }
                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                        }
                                                                    }
                                                                }

                                                                if (!::g_strNotify_2.empty())
                                                                {
                                                                    if (::g_strNotify_2.length() <= 190)
                                                                    {
                                                                        if (::g_strNotify_2.size() <= 190)
                                                                        {
                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                            {
                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTTALK);

                                                                                (*::g_engfuncs.pfnWriteString) (::g_strNotify_2.c_str());
                                                                            }
                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                        }
                                                                    }
                                                                }

                                                                ::g_bShow[nEntity] = true;
                                                            }
                                                        }

                                                        else if (!nTeam)
                                                        {

                                                        }

                                                        else if (nTeam < 0)
                                                        {

                                                        }

                                                        else
                                                        {

                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }

                                    bAltered = { };

                                    if (::g_bFixScore)
                                    {
                                        if (pEntity->v.frags < 0)
                                        {
                                            bAltered = true;
                                            {
                                                pEntity->v.frags = { };
                                            }
                                        }

                                        if (pEntity->v.frags < 1)
                                        {
                                            if (!::g_strGameDir.compare("dod"))
                                            {
                                                if (*((int*)pEntity->pvPrivateData + ::m_iDodDeaths + ::g_nOsOffs) > 0)
                                                {
                                                    *((int*)pEntity->pvPrivateData + ::m_iDodDeaths + ::g_nOsOffs) = { };
                                                    {
                                                        bAltered = true;
                                                    }
                                                }
                                            }

                                            else
                                            {
                                                if (*((int*)pEntity->pvPrivateData + ::m_iDeaths + ::g_nOsOffs) > 0)
                                                {
                                                    *((int*)pEntity->pvPrivateData + ::m_iDeaths + ::g_nOsOffs) = { };
                                                    {
                                                        bAltered = true;
                                                    }
                                                }
                                            }
                                        }
                                    }

                                    else if (::g_bFixKills)
                                    {
                                        if (pEntity->v.frags < 0)
                                        {
                                            bAltered = true;
                                            {
                                                pEntity->v.frags = { };
                                            }
                                        }
                                    }

                                    else if (::g_bFixDeaths)
                                    {
                                        if (pEntity->v.frags < 1)
                                        {
                                            if (!::g_strGameDir.compare("dod"))
                                            {
                                                if (*((int*)pEntity->pvPrivateData + ::m_iDodDeaths + ::g_nOsOffs) > 0)
                                                {
                                                    *((int*)pEntity->pvPrivateData + ::m_iDodDeaths + ::g_nOsOffs) = { };
                                                    {
                                                        bAltered = true;
                                                    }
                                                }
                                            }

                                            else
                                            {
                                                if (*((int*)pEntity->pvPrivateData + ::m_iDeaths + ::g_nOsOffs) > 0)
                                                {
                                                    *((int*)pEntity->pvPrivateData + ::m_iDeaths + ::g_nOsOffs) = { };
                                                    {
                                                        bAltered = true;
                                                    }
                                                }
                                            }
                                        }
                                    }

                                    if (::g_bZeroMoney)
                                    {
                                        if (*((int*)pEntity->pvPrivateData + ::m_iMoney + ::g_nOsOffs))
                                        {
                                            *((int*)pEntity->pvPrivateData + ::m_iMoney + ::g_nOsOffs) = { };
                                        }
                                    }

                                    if (::g_bRealScore)
                                    {
                                        if (::g_nKills[nEntity] || ::g_nDeaths[nEntity])
                                        {
                                            if (!pEntity->v.frags)
                                            {
                                                if (!::g_strGameDir.compare("dod"))
                                                {
                                                    if (!(*((int*)pEntity->pvPrivateData + ::m_iDodDeaths + ::g_nOsOffs)))
                                                    {
                                                        ::g_nKills[nEntity] = { };
                                                        {
                                                            ::g_nDeaths[nEntity] = { };
                                                        }
                                                    }
                                                }

                                                else
                                                {
                                                    if (!(*((int*)pEntity->pvPrivateData + ::m_iDeaths + ::g_nOsOffs)))
                                                    {
                                                        ::g_nKills[nEntity] = { };
                                                        {
                                                            ::g_nDeaths[nEntity] = { };
                                                        }
                                                    }
                                                }
                                            }
                                        }

                                        if (pEntity->v.frags != ::g_nKills[nEntity])
                                        {
                                            bAltered = true;
                                            {
                                                pEntity->v.frags = ::g_nKills[nEntity];
                                            }
                                        }

                                        if (!::g_strGameDir.compare("dod"))
                                        {
                                            if (*((int*)pEntity->pvPrivateData + ::m_iDodDeaths + ::g_nOsOffs) != ::g_nDeaths[nEntity])
                                            {
                                                *((int*)pEntity->pvPrivateData + ::m_iDodDeaths + ::g_nOsOffs) = ::g_nDeaths[nEntity];
                                                {
                                                    bAltered = true;
                                                }
                                            }
                                        }

                                        else
                                        {
                                            if (*((int*)pEntity->pvPrivateData + ::m_iDeaths + ::g_nOsOffs) != ::g_nDeaths[nEntity])
                                            {
                                                *((int*)pEntity->pvPrivateData + ::m_iDeaths + ::g_nOsOffs) = ::g_nDeaths[nEntity];
                                                {
                                                    bAltered = true;
                                                }
                                            }
                                        }
                                    }

                                    if (::g_bFixScore || ::g_bFixKills || ::g_bFixDeaths || ::g_bRealScore)
                                    {
                                        if (bAltered)
                                        {
                                            if (!::g_strGameDir.compare("dod"))
                                            {
                                                if (::g_nScoreShort > 0)
                                                {
                                                    (*::g_engfuncs.pfnMessageBegin) (::g_bAllReliable ? MSG_ALL : MSG_BROADCAST, ::g_nScoreShort, nullptr, nullptr);
                                                    {
                                                        (*::g_engfuncs.pfnWriteByte) (nEntity);
                                                        {
                                                            (*::g_engfuncs.pfnWriteShort) (*((int*)pEntity->pvPrivateData + ::m_iDodScore + ::g_nOsOffs));
                                                            {
                                                                (*::g_engfuncs.pfnWriteShort) (pEntity->v.frags);
                                                            }
                                                            (*::g_engfuncs.pfnWriteShort) (*((int*)pEntity->pvPrivateData + ::m_iDodDeaths + ::g_nOsOffs));
                                                        }
                                                        (*::g_engfuncs.pfnWriteByte) (xTo(true, int));
                                                    }
                                                    (*::g_engfuncs.pfnMessageEnd) ();
                                                }
                                            }

                                            else
                                            {
                                                if (::g_nScoreInfo > 0)
                                                {
                                                    (*::g_engfuncs.pfnMessageBegin) (::g_bAllReliable ? MSG_ALL : MSG_BROADCAST, ::g_nScoreInfo, nullptr, nullptr);
                                                    {
                                                        (*::g_engfuncs.pfnWriteByte) (nEntity);
                                                        {
                                                            (*::g_engfuncs.pfnWriteShort) (pEntity->v.frags);
                                                            (*::g_engfuncs.pfnWriteShort) (*((int*)pEntity->pvPrivateData + ::m_iDeaths + ::g_nOsOffs));
                                                            (*::g_engfuncs.pfnWriteShort) ({ });
                                                            (*::g_engfuncs.pfnWriteShort) (*((int*)pEntity->pvPrivateData + ::m_iTeam + ::g_nOsOffs));
                                                        }
                                                    }
                                                    (*::g_engfuncs.pfnMessageEnd) ();
                                                }
                                            }
                                        }
                                    }

                                    if (!pEntity->v.deadflag)
                                    {
                                        if (pEntity->v.health >= 100.f)
                                        {
                                            if (!::g_bNoWeapons)
                                            {
                                                if (pEntity->v.weapons)
                                                {
                                                    if (::g_bHideRadar)
                                                    {
                                                        if (!::g_bRadar[nEntity])
                                                        {
                                                            (*::g_engfuncs.pfnClientCommand) (pEntity, "hideradar\n");
                                                            {
                                                                ::g_bRadar[nEntity] = true;
                                                            }
                                                        }
                                                    }

                                                    if (::g_bHideMoney || ::g_bHideTimer || ::g_bHideFlash)
                                                    {
                                                        if (::g_nHideWeapon > 0)
                                                        {
                                                            if (::g_nCrosshair > 0)
                                                            {
                                                                if ((nFov = *((int*)pEntity->pvPrivateData + ::m_iFov + ::g_nOsOffs)) != 40)
                                                                {
                                                                    if (nFov != 10)
                                                                    {
                                                                        if (nFov != 15)
                                                                        {
                                                                            if (pEntity->v.fov != 40.f)
                                                                            {
                                                                                if (pEntity->v.fov != 10.f)
                                                                                {
                                                                                    if (pEntity->v.fov != 15.f)
                                                                                    {
                                                                                        if (!::g_bHide[nEntity])
                                                                                        {
                                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nHideWeapon, nullptr, pEntity);
                                                                                            {
                                                                                                if (::g_bHideMoney)
                                                                                                {
                                                                                                    if (::g_bHideTimer)
                                                                                                    {
                                                                                                        if (::g_bHideFlash)
                                                                                                        {
                                                                                                            (*::g_engfuncs.pfnWriteByte) (50);
                                                                                                        }

                                                                                                        else
                                                                                                        {
                                                                                                            (*::g_engfuncs.pfnWriteByte) (48);
                                                                                                        }
                                                                                                    }

                                                                                                    else
                                                                                                    {
                                                                                                        if (::g_bHideFlash)
                                                                                                        {
                                                                                                            (*::g_engfuncs.pfnWriteByte) (34);
                                                                                                        }

                                                                                                        else
                                                                                                        {
                                                                                                            (*::g_engfuncs.pfnWriteByte) (32);
                                                                                                        }
                                                                                                    }
                                                                                                }

                                                                                                else if (::g_bHideTimer)
                                                                                                {
                                                                                                    if (::g_bHideFlash)
                                                                                                    {
                                                                                                        (*::g_engfuncs.pfnWriteByte) (18);
                                                                                                    }

                                                                                                    else
                                                                                                    {
                                                                                                        (*::g_engfuncs.pfnWriteByte) (16);
                                                                                                    }
                                                                                                }

                                                                                                else
                                                                                                {
                                                                                                    (*::g_engfuncs.pfnWriteByte) (2);
                                                                                                }
                                                                                            }
                                                                                            (*::g_engfuncs.pfnMessageEnd) ();

                                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nCrosshair, nullptr, pEntity);
                                                                                            {
                                                                                                (*::g_engfuncs.pfnWriteByte) ({ });
                                                                                            }
                                                                                            (*::g_engfuncs.pfnMessageEnd) ();

                                                                                            ::g_bHide[nEntity] = true;
                                                                                        }
                                                                                    }
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }

                                                    if (::g_nArmor)
                                                    {
                                                        if (pEntity->v.armorvalue < 100.f)
                                                        {
                                                            if (!::g_bArmor[nEntity])
                                                            {
                                                                if (!::g_bKeepHelmet)
                                                                {
                                                                    pEntity->v.armorvalue = 100.f;
                                                                    {
                                                                        if (*((int*)pEntity->pvPrivateData + ::m_iKevlar + ::g_nOsOffs) != ::g_nArmor)
                                                                        {
                                                                            *((int*)pEntity->pvPrivateData + ::m_iKevlar + ::g_nOsOffs) = ::g_nArmor;
                                                                        }
                                                                    }

                                                                    if (::g_nArmorType > 0)
                                                                    {
                                                                        (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nArmorType, nullptr, pEntity);
                                                                        {
                                                                            (*::g_engfuncs.pfnWriteByte) (::g_nArmor - 1);
                                                                        }
                                                                        (*::g_engfuncs.pfnMessageEnd) ();
                                                                    }
                                                                }

                                                                else if (::g_nArmor > 1)
                                                                {
                                                                    pEntity->v.armorvalue = 100.f;
                                                                    {
                                                                        if (*((int*)pEntity->pvPrivateData + ::m_iKevlar + ::g_nOsOffs) != ::g_nArmor)
                                                                        {
                                                                            *((int*)pEntity->pvPrivateData + ::m_iKevlar + ::g_nOsOffs) = ::g_nArmor;
                                                                        }
                                                                    }

                                                                    if (::g_nArmorType > 0)
                                                                    {
                                                                        (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nArmorType, nullptr, pEntity);
                                                                        {
                                                                            (*::g_engfuncs.pfnWriteByte) (::g_nArmor - 1);
                                                                        }
                                                                        (*::g_engfuncs.pfnMessageEnd) ();
                                                                    }
                                                                }

                                                                else
                                                                {
                                                                    pEntity->v.armorvalue = 100.f;
                                                                    {
                                                                        if ((nKevlar = *((int*)pEntity->pvPrivateData + ::m_iKevlar + ::g_nOsOffs)) < 2)
                                                                        {
                                                                            if (nKevlar != ::g_nArmor)
                                                                            {
                                                                                *((int*)pEntity->pvPrivateData + ::m_iKevlar + ::g_nOsOffs) = ::g_nArmor;
                                                                            }
                                                                        }
                                                                    }

                                                                    if (::g_nArmorType > 0)
                                                                    {
                                                                        (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nArmorType, nullptr, pEntity);
                                                                        {
                                                                            if (*((int*)pEntity->pvPrivateData + ::m_iKevlar + ::g_nOsOffs) < 2)
                                                                            {
                                                                                (*::g_engfuncs.pfnWriteByte) (::g_nArmor - 1);
                                                                            }

                                                                            else
                                                                            {
                                                                                (*::g_engfuncs.pfnWriteByte) (xTo(true, int));
                                                                            }
                                                                        }
                                                                        (*::g_engfuncs.pfnMessageEnd) ();
                                                                    }
                                                                }

                                                                ::g_bArmor[nEntity] = true;
                                                            }
                                                        }

                                                        else
                                                        {
                                                            ::g_bArmor[nEntity] = true;
                                                        }
                                                    }

                                                    else
                                                    {
                                                        ::g_bArmor[nEntity] = { };
                                                    }
                                                }

                                                else
                                                {
                                                    if (::g_nArmor)
                                                    {
                                                        ::g_bArmor[nEntity] = { };
                                                    }

                                                    if (::g_bHideFlash || ::g_bHideTimer || ::g_bHideMoney)
                                                    {
                                                        if (::g_nHideWeapon)
                                                        {
                                                            if (::g_nCrosshair)
                                                            {
                                                                ::g_bHide[nEntity] = { };
                                                            }
                                                        }
                                                    }

                                                    if (::g_bHideRadar)
                                                    {
                                                        ::g_bRadar[nEntity] = { };
                                                    }
                                                }
                                            }

                                            else
                                            {
                                                if (::g_bHideRadar)
                                                {
                                                    if (!::g_bRadar[nEntity])
                                                    {
                                                        (*::g_engfuncs.pfnClientCommand) (pEntity, "hideradar\n");
                                                        {
                                                            ::g_bRadar[nEntity] = true;
                                                        }
                                                    }
                                                }

                                                if (::g_bHideMoney || ::g_bHideTimer || ::g_bHideFlash)
                                                {
                                                    if (::g_nHideWeapon > 0)
                                                    {
                                                        if (::g_nCrosshair > 0)
                                                        {
                                                            if ((nFov = *((int*)pEntity->pvPrivateData + ::m_iFov + ::g_nOsOffs)) != 40)
                                                            {
                                                                if (nFov != 10)
                                                                {
                                                                    if (nFov != 15)
                                                                    {
                                                                        if (pEntity->v.fov != 40.f)
                                                                        {
                                                                            if (pEntity->v.fov != 10.f)
                                                                            {
                                                                                if (pEntity->v.fov != 15.f)
                                                                                {
                                                                                    if (!::g_bHide[nEntity])
                                                                                    {
                                                                                        (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nHideWeapon, nullptr, pEntity);
                                                                                        {
                                                                                            if (::g_bHideMoney)
                                                                                            {
                                                                                                if (::g_bHideTimer)
                                                                                                {
                                                                                                    if (::g_bHideFlash)
                                                                                                    {
                                                                                                        (*::g_engfuncs.pfnWriteByte) (50);
                                                                                                    }

                                                                                                    else
                                                                                                    {
                                                                                                        (*::g_engfuncs.pfnWriteByte) (48);
                                                                                                    }
                                                                                                }

                                                                                                else
                                                                                                {
                                                                                                    if (::g_bHideFlash)
                                                                                                    {
                                                                                                        (*::g_engfuncs.pfnWriteByte) (34);
                                                                                                    }

                                                                                                    else
                                                                                                    {
                                                                                                        (*::g_engfuncs.pfnWriteByte) (32);
                                                                                                    }
                                                                                                }
                                                                                            }

                                                                                            else if (::g_bHideTimer)
                                                                                            {
                                                                                                if (::g_bHideFlash)
                                                                                                {
                                                                                                    (*::g_engfuncs.pfnWriteByte) (18);
                                                                                                }

                                                                                                else
                                                                                                {
                                                                                                    (*::g_engfuncs.pfnWriteByte) (16);
                                                                                                }
                                                                                            }

                                                                                            else
                                                                                            {
                                                                                                (*::g_engfuncs.pfnWriteByte) (2);
                                                                                            }
                                                                                        }
                                                                                        (*::g_engfuncs.pfnMessageEnd) ();

                                                                                        (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nCrosshair, nullptr, pEntity);
                                                                                        {
                                                                                            (*::g_engfuncs.pfnWriteByte) ({ });
                                                                                        }
                                                                                        (*::g_engfuncs.pfnMessageEnd) ();

                                                                                        ::g_bHide[nEntity] = true;
                                                                                    }
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }

                                                if (::g_nArmor)
                                                {
                                                    if (pEntity->v.armorvalue < 100.f)
                                                    {
                                                        if (!::g_bArmor[nEntity])
                                                        {
                                                            if (!::g_bKeepHelmet)
                                                            {
                                                                pEntity->v.armorvalue = 100.f;
                                                                {
                                                                    if (*((int*)pEntity->pvPrivateData + ::m_iKevlar + ::g_nOsOffs) != ::g_nArmor)
                                                                    {
                                                                        *((int*)pEntity->pvPrivateData + ::m_iKevlar + ::g_nOsOffs) = ::g_nArmor;
                                                                    }
                                                                }

                                                                if (::g_nArmorType > 0)
                                                                {
                                                                    (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nArmorType, nullptr, pEntity);
                                                                    {
                                                                        (*::g_engfuncs.pfnWriteByte) (::g_nArmor - 1);
                                                                    }
                                                                    (*::g_engfuncs.pfnMessageEnd) ();
                                                                }
                                                            }

                                                            else if (::g_nArmor > 1)
                                                            {
                                                                pEntity->v.armorvalue = 100.f;
                                                                {
                                                                    if (*((int*)pEntity->pvPrivateData + ::m_iKevlar + ::g_nOsOffs) != ::g_nArmor)
                                                                    {
                                                                        *((int*)pEntity->pvPrivateData + ::m_iKevlar + ::g_nOsOffs) = ::g_nArmor;
                                                                    }
                                                                }

                                                                if (::g_nArmorType > 0)
                                                                {
                                                                    (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nArmorType, nullptr, pEntity);
                                                                    {
                                                                        (*::g_engfuncs.pfnWriteByte) (::g_nArmor - 1);
                                                                    }
                                                                    (*::g_engfuncs.pfnMessageEnd) ();
                                                                }
                                                            }

                                                            else
                                                            {
                                                                pEntity->v.armorvalue = 100.f;
                                                                {
                                                                    if ((nKevlar = *((int*)pEntity->pvPrivateData + ::m_iKevlar + ::g_nOsOffs)) < 2)
                                                                    {
                                                                        if (nKevlar != ::g_nArmor)
                                                                        {
                                                                            *((int*)pEntity->pvPrivateData + ::m_iKevlar + ::g_nOsOffs) = ::g_nArmor;
                                                                        }
                                                                    }
                                                                }

                                                                if (::g_nArmorType > 0)
                                                                {
                                                                    (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nArmorType, nullptr, pEntity);
                                                                    {
                                                                        if (*((int*)pEntity->pvPrivateData + ::m_iKevlar + ::g_nOsOffs) < 2)
                                                                        {
                                                                            (*::g_engfuncs.pfnWriteByte) (::g_nArmor - 1);
                                                                        }

                                                                        else
                                                                        {
                                                                            (*::g_engfuncs.pfnWriteByte) (xTo(true, int));
                                                                        }
                                                                    }
                                                                    (*::g_engfuncs.pfnMessageEnd) ();
                                                                }
                                                            }

                                                            ::g_bArmor[nEntity] = true;
                                                        }
                                                    }

                                                    else
                                                    {
                                                        ::g_bArmor[nEntity] = true;
                                                    }
                                                }

                                                else
                                                {
                                                    ::g_bArmor[nEntity] = { };
                                                }
                                            }
                                        }

                                        else
                                        {
                                            if (pEntity->v.health <= 0.f)
                                            {
                                                if (::g_nArmor)
                                                {
                                                    ::g_bArmor[nEntity] = { };
                                                }

                                                if (::g_bHideFlash || ::g_bHideTimer || ::g_bHideMoney)
                                                {
                                                    if (::g_nHideWeapon)
                                                    {
                                                        if (::g_nCrosshair)
                                                        {
                                                            ::g_bHide[nEntity] = { };
                                                        }
                                                    }
                                                }

                                                if (::g_bHideRadar)
                                                {
                                                    ::g_bRadar[nEntity] = { };
                                                }
                                            }

                                            else
                                            {

                                            }
                                        }
                                    }

                                    else
                                    {
                                        if (::g_nArmor)
                                        {
                                            ::g_bArmor[nEntity] = { };
                                        }

                                        if (::g_bHideFlash || ::g_bHideTimer || ::g_bHideMoney)
                                        {
                                            if (::g_nHideWeapon)
                                            {
                                                if (::g_nCrosshair)
                                                {
                                                    ::g_bHide[nEntity] = { };
                                                }
                                            }
                                        }

                                        if (::g_bHideRadar)
                                        {
                                            ::g_bRadar[nEntity] = { };
                                        }
                                    }
                                }
                            }
                        }

                        else
                        {
                            if (::g_nArmor)
                            {
                                ::g_bArmor[nEntity] = { };
                            }

                            if (::g_bHideFlash || ::g_bHideTimer || ::g_bHideMoney)
                            {
                                if (::g_nHideWeapon)
                                {
                                    if (::g_nCrosshair)
                                    {
                                        ::g_bHide[nEntity] = { };
                                    }
                                }
                            }

                            if (::g_bHideRadar)
                            {
                                ::g_bRadar[nEntity] = { };
                            }
                        }
                    }

                    else
                    {
                        if (::g_nArmor)
                        {
                            ::g_bArmor[nEntity] = { };
                        }

                        if (::g_bHideFlash || ::g_bHideTimer || ::g_bHideMoney)
                        {
                            if (::g_nHideWeapon)
                            {
                                if (::g_nCrosshair)
                                {
                                    ::g_bHide[nEntity] = { };
                                }
                            }
                        }

                        if (::g_bHideRadar)
                        {
                            ::g_bRadar[nEntity] = { };
                        }
                    }
                }

                else
                {
                    if (::g_nArmor)
                    {
                        ::g_bArmor[nEntity] = { };
                    }

                    if (::g_bHideFlash || ::g_bHideTimer || ::g_bHideMoney)
                    {
                        if (::g_nHideWeapon)
                        {
                            if (::g_nCrosshair)
                            {
                                ::g_bHide[nEntity] = { };
                            }
                        }
                    }

                    if (::g_bHideRadar)
                    {
                        ::g_bRadar[nEntity] = { };
                    }
                }
            }

            else
            {
                if (::g_nArmor)
                {
                    ::g_bArmor[nEntity] = { };
                }

                if (::g_bHideFlash || ::g_bHideTimer || ::g_bHideMoney)
                {
                    if (::g_nHideWeapon)
                    {
                        if (::g_nCrosshair)
                        {
                            ::g_bHide[nEntity] = { };
                        }
                    }
                }

                if (::g_bHideRadar)
                {
                    ::g_bRadar[nEntity] = { };
                }
            }
        }

        if (::g_bHourlyRestart)
        {
            llTime = ::std::time(nullptr);
            {
                if (llTime - llStamp > 0)
                {
                    if (!((llTime + 15) % 3600))
                    {
                        strCmd.assign("say");
                        {
                            strCmd.append(" ");
                            {
                                strCmd.append("CHANGING MAP TO");
                                {
                                    strCmd.append(" ");
                                    {
                                        strCmd.append(STRING(::gpGlobals->mapname));
                                        {
                                            strCmd.append(" ");
                                            {
                                                strCmd.append("IN 15 SECONDS");
                                                {
                                                    strCmd.append("\n");
                                                    {
                                                        strCmd.shrink_to_fit();
                                                        {
                                                            (*::g_engfuncs.pfnServerCommand) (strCmd.c_str());
                                                            {
                                                                strCmd.clear();
                                                                {
                                                                    strCmd.shrink_to_fit();
                                                                    {
                                                                        llStamp = llTime;
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }

                    else if (!((llTime + 10) % 3600))
                    {
                        strCmd.assign("say");
                        {
                            strCmd.append(" ");
                            {
                                strCmd.append("CHANGING MAP TO");
                                {
                                    strCmd.append(" ");
                                    {
                                        strCmd.append(STRING(::gpGlobals->mapname));
                                        {
                                            strCmd.append(" ");
                                            {
                                                strCmd.append("IN 10 SECONDS");
                                                {
                                                    strCmd.append("\n");
                                                    {
                                                        strCmd.shrink_to_fit();
                                                        {
                                                            (*::g_engfuncs.pfnServerCommand) (strCmd.c_str());
                                                            {
                                                                strCmd.clear();
                                                                {
                                                                    strCmd.shrink_to_fit();
                                                                    {
                                                                        llStamp = llTime;
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }

                    else if (!((llTime + 5) % 3600))
                    {
                        strCmd.assign("say");
                        {
                            strCmd.append(" ");
                            {
                                strCmd.append("CHANGING MAP TO");
                                {
                                    strCmd.append(" ");
                                    {
                                        strCmd.append(STRING(::gpGlobals->mapname));
                                        {
                                            strCmd.append(" ");
                                            {
                                                strCmd.append("IN 5 SECONDS");
                                                {
                                                    strCmd.append("\n");
                                                    {
                                                        strCmd.shrink_to_fit();
                                                        {
                                                            (*::g_engfuncs.pfnServerCommand) (strCmd.c_str());
                                                            {
                                                                strCmd.clear();
                                                                {
                                                                    strCmd.shrink_to_fit();
                                                                    {
                                                                        llStamp = llTime;
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }

                    else if (!(llTime % 3600))
                    {
                        strCmd.assign("changelevel");
                        {
                            strCmd.append(" ");
                            {
                                strCmd.append("\"");
                                {
                                    strCmd.append(STRING(::gpGlobals->mapname));
                                    {
                                        strCmd.append("\"");
                                        {
                                            strCmd.append("\n");
                                            {
                                                strCmd.shrink_to_fit();
                                                {
                                                    (*::g_engfuncs.pfnServerCommand) (strCmd.c_str());
                                                    {
                                                        strCmd.clear();
                                                        {
                                                            strCmd.shrink_to_fit();
                                                            {
                                                                llStamp = llTime;
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        nIter = { };
    }

    do
    {
        ::gpMetaGlobals->mres = ::MRES_IGNORED;

        return;
    }

    while (false);
}

void Hook_ClientPutInServer_Post(::edict_t* pEntity) noexcept
{
    static int nEntity{ };
    static bool bMsgs{ };

    if (!bMsgs)
    {
        if (!::g_nResetHud)
        {
            ::g_nResetHud = (*::gpMetaUtilFuncs->pfnGetUserMsgID) (&::Plugin_info, "ResetHUD", nullptr);
        }

        if (::g_bRealScore)
        {
            if (!::g_nDeathMsg)
            {
                ::g_nDeathMsg = (*::gpMetaUtilFuncs->pfnGetUserMsgID) (&::Plugin_info, "DeathMsg", nullptr);
            }
        }

        if (!::g_strWelcome_1.empty())
        {
            if (::g_strWelcome_1.length() <= 190)
            {
                if (::g_strWelcome_1.size() <= 190)
                {
                    if (!::g_nTextMsg)
                    {
                        ::g_nTextMsg = (*::gpMetaUtilFuncs->pfnGetUserMsgID) (&::Plugin_info, "TextMsg", nullptr);
                    }
                }
            }
        }

        if (::g_nArmor)
        {
            if (!::g_nArmorType)
            {
                ::g_nArmorType = (*::gpMetaUtilFuncs->pfnGetUserMsgID) (&::Plugin_info, "ArmorType", nullptr);
            }
        }

        if (::g_bFixScore || ::g_bFixKills || ::g_bFixDeaths || ::g_bRealScore)
        {
            if (!::g_nScoreInfo)
            {
                ::g_nScoreInfo = (*::gpMetaUtilFuncs->pfnGetUserMsgID) (&::Plugin_info, "ScoreInfo", nullptr);
            }

            if (!::g_strGameDir.compare("dod"))
            {
                if (!::g_nScoreShort)
                {
                    ::g_nScoreShort = (*::gpMetaUtilFuncs->pfnGetUserMsgID) (&::Plugin_info, "ScoreShort", nullptr);
                }
            }
        }

        if (::g_bHideMoney)
        {
            if (!::g_nHideWeapon)
            {
                ::g_nHideWeapon = (*::gpMetaUtilFuncs->pfnGetUserMsgID) (&::Plugin_info, "HideWeapon", nullptr);
            }

            if (!::g_nCrosshair)
            {
                ::g_nCrosshair = (*::gpMetaUtilFuncs->pfnGetUserMsgID) (&::Plugin_info, "Crosshair", nullptr);
            }
        }

        else if (::g_bHideTimer)
        {
            if (!::g_nHideWeapon)
            {
                ::g_nHideWeapon = (*::gpMetaUtilFuncs->pfnGetUserMsgID) (&::Plugin_info, "HideWeapon", nullptr);
            }

            if (!::g_nCrosshair)
            {
                ::g_nCrosshair = (*::gpMetaUtilFuncs->pfnGetUserMsgID) (&::Plugin_info, "Crosshair", nullptr);
            }
        }

        else if (::g_bHideFlash)
        {
            if (!::g_nHideWeapon)
            {
                ::g_nHideWeapon = (*::gpMetaUtilFuncs->pfnGetUserMsgID) (&::Plugin_info, "HideWeapon", nullptr);
            }

            if (!::g_nCrosshair)
            {
                ::g_nCrosshair = (*::gpMetaUtilFuncs->pfnGetUserMsgID) (&::Plugin_info, "Crosshair", nullptr);
            }
        }

        else
        {

        }

        bMsgs = true;
    }

    if (pEntity)
    {
        if ((nEntity = ::ENTINDEX(pEntity)) > 0)
        {
            if (::g_bHideRadar)
            {
                (*::g_engfuncs.pfnClientCommand) (pEntity, "hideradar\n");
                {
                    ::g_bRadar[nEntity] = { };
                }
            }

            if (::g_nArmor)
            {
                ::g_bArmor[nEntity] = { };
            }

            if (::g_bRealScore)
            {
                ::g_nDeaths[nEntity] = { };
                {
                    ::g_nKills[nEntity] = { };
                }
            }

            if (::g_bHideFlash)
            {
                if (::g_nHideWeapon > 0)
                {
                    if (::g_nCrosshair > 0)
                    {
                        ::g_bHide[nEntity] = { };
                    }
                }
            }

            else if (::g_bHideMoney)
            {
                if (::g_nHideWeapon > 0)
                {
                    if (::g_nCrosshair > 0)
                    {
                        ::g_bHide[nEntity] = { };
                    }
                }
            }

            else if (::g_bHideTimer)
            {
                if (::g_nHideWeapon > 0)
                {
                    if (::g_nCrosshair > 0)
                    {
                        ::g_bHide[nEntity] = { };
                    }
                }
            }

            else
            {

            }

            if (!::g_strWelcome_1.empty())
            {
                if (::g_strWelcome_1.length() <= 190)
                {
                    if (::g_strWelcome_1.size() <= 190)
                    {
                        if (::g_nTextMsg > 0)
                        {
                            ::g_bShow[nEntity] = { };
                        }
                    }
                }
            }
        }
    }

    if (!::g_bNoReturn)
    {
        do
        {
            ::gpMetaGlobals->mres = ::MRES_IGNORED;

            return;
        }

        while (false);
    }

    else
    {
        return;
    }
}

void Hook_ClientDisconnect_Post(::edict_t* pEntity) noexcept
{
    static int nEntity{ };

    if (pEntity)
    {
        if ((nEntity = ::ENTINDEX(pEntity)) > 0)
        {
            if (::g_bHideRadar)
            {
                ::g_bRadar[nEntity] = { };
            }

            if (::g_nArmor)
            {
                ::g_bArmor[nEntity] = { };
            }

            if (::g_bRealScore)
            {
                ::g_nDeaths[nEntity] = { };
                {
                    ::g_nKills[nEntity] = { };
                }
            }

            if (::g_bHideFlash)
            {
                if (::g_nHideWeapon > 0)
                {
                    if (::g_nCrosshair > 0)
                    {
                        ::g_bHide[nEntity] = { };
                    }
                }
            }

            else if (::g_bHideMoney)
            {
                if (::g_nHideWeapon > 0)
                {
                    if (::g_nCrosshair > 0)
                    {
                        ::g_bHide[nEntity] = { };
                    }
                }
            }

            else if (::g_bHideTimer)
            {
                if (::g_nHideWeapon > 0)
                {
                    if (::g_nCrosshair > 0)
                    {
                        ::g_bHide[nEntity] = { };
                    }
                }
            }

            else
            {

            }

            if (!::g_strWelcome_1.empty())
            {
                if (::g_strWelcome_1.length() <= 190)
                {
                    if (::g_strWelcome_1.size() <= 190)
                    {
                        if (::g_nTextMsg > 0)
                        {
                            ::g_bShow[nEntity] = { };
                        }
                    }
                }
            }
        }
    }

    do
    {
        ::gpMetaGlobals->mres = ::MRES_IGNORED;

        return;
    }

    while (false);
}

void Hook_MessageBegin_Post(int, int nMsg, const float*, ::edict_t* pEntity) noexcept
{
    static bool bMsgs{ };
    static int nEntity{ };

    if (!bMsgs)
    {
        if (!::g_nResetHud)
        {
            ::g_nResetHud = (*::gpMetaUtilFuncs->pfnGetUserMsgID) (&::Plugin_info, "ResetHUD", nullptr);
        }

        if (::g_bRealScore)
        {
            if (!::g_nDeathMsg)
            {
                ::g_nDeathMsg = (*::gpMetaUtilFuncs->pfnGetUserMsgID) (&::Plugin_info, "DeathMsg", nullptr);
            }
        }

        if (!::g_strWelcome_1.empty())
        {
            if (::g_strWelcome_1.length() <= 190)
            {
                if (::g_strWelcome_1.size() <= 190)
                {
                    if (!::g_nTextMsg)
                    {
                        ::g_nTextMsg = (*::gpMetaUtilFuncs->pfnGetUserMsgID) (&::Plugin_info, "TextMsg", nullptr);
                    }
                }
            }
        }

        if (::g_nArmor)
        {
            if (!::g_nArmorType)
            {
                ::g_nArmorType = (*::gpMetaUtilFuncs->pfnGetUserMsgID) (&::Plugin_info, "ArmorType", nullptr);
            }
        }

        if (::g_bFixScore || ::g_bFixKills || ::g_bFixDeaths || ::g_bRealScore)
        {
            if (!::g_nScoreInfo)
            {
                ::g_nScoreInfo = (*::gpMetaUtilFuncs->pfnGetUserMsgID) (&::Plugin_info, "ScoreInfo", nullptr);
            }

            if (!::g_strGameDir.compare("dod"))
            {
                if (!::g_nScoreShort)
                {
                    ::g_nScoreShort = (*::gpMetaUtilFuncs->pfnGetUserMsgID) (&::Plugin_info, "ScoreShort", nullptr);
                }
            }
        }

        if (::g_bHideMoney)
        {
            if (!::g_nHideWeapon)
            {
                ::g_nHideWeapon = (*::gpMetaUtilFuncs->pfnGetUserMsgID) (&::Plugin_info, "HideWeapon", nullptr);
            }

            if (!::g_nCrosshair)
            {
                ::g_nCrosshair = (*::gpMetaUtilFuncs->pfnGetUserMsgID) (&::Plugin_info, "Crosshair", nullptr);
            }
        }

        else if (::g_bHideTimer)
        {
            if (!::g_nHideWeapon)
            {
                ::g_nHideWeapon = (*::gpMetaUtilFuncs->pfnGetUserMsgID) (&::Plugin_info, "HideWeapon", nullptr);
            }

            if (!::g_nCrosshair)
            {
                ::g_nCrosshair = (*::gpMetaUtilFuncs->pfnGetUserMsgID) (&::Plugin_info, "Crosshair", nullptr);
            }
        }

        else if (::g_bHideFlash)
        {
            if (!::g_nHideWeapon)
            {
                ::g_nHideWeapon = (*::gpMetaUtilFuncs->pfnGetUserMsgID) (&::Plugin_info, "HideWeapon", nullptr);
            }

            if (!::g_nCrosshair)
            {
                ::g_nCrosshair = (*::gpMetaUtilFuncs->pfnGetUserMsgID) (&::Plugin_info, "Crosshair", nullptr);
            }
        }

        else
        {

        }

        bMsgs = true;
    }

    if (pEntity)
    {
        if (::g_nResetHud > 0)
        {
            if (nMsg == ::g_nResetHud)
            {
                if (!pEntity->free)
                {
                    if (pEntity->pvPrivateData)
                    {
                        if (!pEntity->serialnumber)
                        {
                            if (!pEntity->v.deadflag)
                            {
                                if (pEntity->v.health >= 100.f)
                                {
                                    if (::g_bNoWeapons)
                                    {
                                        if (!(pEntity->v.flags & FL_DORMANT))
                                        {
                                            if (pEntity->v.flags & FL_CLIENT)
                                            {
                                                if (!::g_bResetHud)
                                                {
                                                    ::g_bResetHud = true;
                                                    {
                                                        if ((::g_nResetHudId = ::ENTINDEX(pEntity)) > 0)
                                                        {
                                                            if (::g_nArmor)
                                                            {
                                                                ::g_bArmor[::g_nResetHudId] = { };
                                                            }
                                                        }

                                                        else
                                                        {
                                                            ::g_bResetHud = { };
                                                            {
                                                                ::g_nResetHudId = { };
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }

                                    else
                                    {
                                        if (pEntity->v.weapons)
                                        {
                                            if (!(pEntity->v.flags & FL_DORMANT))
                                            {
                                                if (pEntity->v.flags & FL_CLIENT)
                                                {
                                                    if (!::g_bResetHud)
                                                    {
                                                        ::g_bResetHud = true;
                                                        {
                                                            if ((::g_nResetHudId = ::ENTINDEX(pEntity)) > 0)
                                                            {
                                                                if (::g_nArmor)
                                                                {
                                                                    ::g_bArmor[::g_nResetHudId] = { };
                                                                }
                                                            }

                                                            else
                                                            {
                                                                ::g_bResetHud = { };
                                                                {
                                                                    ::g_nResetHudId = { };
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }

                                        else
                                        {
                                            if ((nEntity = ::ENTINDEX(pEntity)) > 0)
                                            {
                                                if (::g_nArmor)
                                                {
                                                    ::g_bArmor[nEntity] = { };
                                                }

                                                if (::g_bHideFlash || ::g_bHideTimer || ::g_bHideMoney)
                                                {
                                                    if (::g_nHideWeapon)
                                                    {
                                                        if (::g_nCrosshair)
                                                        {
                                                            ::g_bHide[nEntity] = { };
                                                        }
                                                    }
                                                }

                                                if (::g_bHideRadar)
                                                {
                                                    ::g_bRadar[nEntity] = { };
                                                }
                                            }
                                        }
                                    }
                                }

                                else
                                {
                                    if (pEntity->v.health <= 0.f)
                                    {
                                        if ((nEntity = ::ENTINDEX(pEntity)) > 0)
                                        {
                                            if (::g_nArmor)
                                            {
                                                ::g_bArmor[nEntity] = { };
                                            }

                                            if (::g_bHideFlash || ::g_bHideTimer || ::g_bHideMoney)
                                            {
                                                if (::g_nHideWeapon)
                                                {
                                                    if (::g_nCrosshair)
                                                    {
                                                        ::g_bHide[nEntity] = { };
                                                    }
                                                }
                                            }

                                            if (::g_bHideRadar)
                                            {
                                                ::g_bRadar[nEntity] = { };
                                            }
                                        }
                                    }

                                    else
                                    {

                                    }
                                }
                            }

                            else
                            {
                                if ((nEntity = ::ENTINDEX(pEntity)) > 0)
                                {
                                    if (::g_nArmor)
                                    {
                                        ::g_bArmor[nEntity] = { };
                                    }

                                    if (::g_bHideFlash || ::g_bHideTimer || ::g_bHideMoney)
                                    {
                                        if (::g_nHideWeapon)
                                        {
                                            if (::g_nCrosshair)
                                            {
                                                ::g_bHide[nEntity] = { };
                                            }
                                        }
                                    }

                                    if (::g_bHideRadar)
                                    {
                                        ::g_bRadar[nEntity] = { };
                                    }
                                }
                            }
                        }

                        else
                        {
                            if ((nEntity = ::ENTINDEX(pEntity)) > 0)
                            {
                                if (::g_nArmor)
                                {
                                    ::g_bArmor[nEntity] = { };
                                }

                                if (::g_bHideFlash || ::g_bHideTimer || ::g_bHideMoney)
                                {
                                    if (::g_nHideWeapon)
                                    {
                                        if (::g_nCrosshair)
                                        {
                                            ::g_bHide[nEntity] = { };
                                        }
                                    }
                                }

                                if (::g_bHideRadar)
                                {
                                    ::g_bRadar[nEntity] = { };
                                }
                            }
                        }
                    }

                    else
                    {
                        if ((nEntity = ::ENTINDEX(pEntity)) > 0)
                        {
                            if (::g_nArmor)
                            {
                                ::g_bArmor[nEntity] = { };
                            }

                            if (::g_bHideFlash || ::g_bHideTimer || ::g_bHideMoney)
                            {
                                if (::g_nHideWeapon)
                                {
                                    if (::g_nCrosshair)
                                    {
                                        ::g_bHide[nEntity] = { };
                                    }
                                }
                            }

                            if (::g_bHideRadar)
                            {
                                ::g_bRadar[nEntity] = { };
                            }
                        }
                    }
                }

                else
                {
                    if ((nEntity = ::ENTINDEX(pEntity)) > 0)
                    {
                        if (::g_nArmor)
                        {
                            ::g_bArmor[nEntity] = { };
                        }

                        if (::g_bHideFlash || ::g_bHideTimer || ::g_bHideMoney)
                        {
                            if (::g_nHideWeapon)
                            {
                                if (::g_nCrosshair)
                                {
                                    ::g_bHide[nEntity] = { };
                                }
                            }
                        }

                        if (::g_bHideRadar)
                        {
                            ::g_bRadar[nEntity] = { };
                        }
                    }
                }
            }

            else
            {
                if (::g_nDeathMsg > 0)
                {
                    if (nMsg == ::g_nDeathMsg)
                    {
                        if (::g_bRealScore)
                        {
                            ::g_bDeathMsg = true;
                            {
                                ::g_nDeathMsgByte = { };
                                {
                                    ::g_nKiller = { };
                                    {
                                        ::g_nVictim = { };
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        else
        {
            if (::g_nDeathMsg > 0)
            {
                if (nMsg == ::g_nDeathMsg)
                {
                    if (::g_bRealScore)
                    {
                        ::g_bDeathMsg = true;
                        {
                            ::g_nDeathMsgByte = { };
                            {
                                ::g_nKiller = { };
                                {
                                    ::g_nVictim = { };
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    else
    {
        if (::g_nDeathMsg > 0)
        {
            if (nMsg == ::g_nDeathMsg)
            {
                if (::g_bRealScore)
                {
                    ::g_bDeathMsg = true;
                    {
                        ::g_nDeathMsgByte = { };
                        {
                            ::g_nKiller = { };
                            {
                                ::g_nVictim = { };
                            }
                        }
                    }
                }
            }
        }
    }

    do
    {
        ::gpMetaGlobals->mres = ::MRES_IGNORED;

        return;
    }

    while (false);
}

#ifdef WIN32

constexpr void Hook_WriteByte_Post(int nByte) noexcept

#else

void Hook_WriteByte_Post(int nByte) noexcept

#endif

{
    if (::g_nDeathMsg > 0)
    {
        if (::g_bDeathMsg)
        {
            if (::g_bRealScore)
            {
                if (++ ::g_nDeathMsgByte == xTo(true, int))
                {
                    ::g_nKiller = nByte;
                }

                else
                {
                    if (::g_nDeathMsgByte == 2)
                    {
                        ::g_nVictim = nByte;
                    }
                }
            }
        }
    }

    do
    {
        ::gpMetaGlobals->mres = ::MRES_IGNORED;

        return;
    }

    while (false);
}

void Hook_MessageEnd_Post() noexcept
{
    static ::cvar_t* pFriendlyFire{ };
    static ::edict_t* pEntity{ }, * pVictim{ };

    static int nTeam{ }, nFov{ }, nKevlar{ };
    static bool bAltered{ }, bMsgs{ }, bConVars{ }, bSafe{ };

    if (!bMsgs)
    {
        if (!::g_nResetHud)
        {
            ::g_nResetHud = (*::gpMetaUtilFuncs->pfnGetUserMsgID) (&::Plugin_info, "ResetHUD", nullptr);
        }

        if (::g_bRealScore)
        {
            if (!::g_nDeathMsg)
            {
                ::g_nDeathMsg = (*::gpMetaUtilFuncs->pfnGetUserMsgID) (&::Plugin_info, "DeathMsg", nullptr);
            }
        }

        if (!::g_strWelcome_1.empty())
        {
            if (::g_strWelcome_1.length() <= 190)
            {
                if (::g_strWelcome_1.size() <= 190)
                {
                    if (!::g_nTextMsg)
                    {
                        ::g_nTextMsg = (*::gpMetaUtilFuncs->pfnGetUserMsgID) (&::Plugin_info, "TextMsg", nullptr);
                    }
                }
            }
        }

        if (::g_nArmor)
        {
            if (!::g_nArmorType)
            {
                ::g_nArmorType = (*::gpMetaUtilFuncs->pfnGetUserMsgID) (&::Plugin_info, "ArmorType", nullptr);
            }
        }

        if (::g_bFixScore || ::g_bFixKills || ::g_bFixDeaths || ::g_bRealScore)
        {
            if (!::g_nScoreInfo)
            {
                ::g_nScoreInfo = (*::gpMetaUtilFuncs->pfnGetUserMsgID) (&::Plugin_info, "ScoreInfo", nullptr);
            }

            if (!::g_strGameDir.compare("dod"))
            {
                if (!::g_nScoreShort)
                {
                    ::g_nScoreShort = (*::gpMetaUtilFuncs->pfnGetUserMsgID) (&::Plugin_info, "ScoreShort", nullptr);
                }
            }
        }

        if (::g_bHideMoney)
        {
            if (!::g_nHideWeapon)
            {
                ::g_nHideWeapon = (*::gpMetaUtilFuncs->pfnGetUserMsgID) (&::Plugin_info, "HideWeapon", nullptr);
            }

            if (!::g_nCrosshair)
            {
                ::g_nCrosshair = (*::gpMetaUtilFuncs->pfnGetUserMsgID) (&::Plugin_info, "Crosshair", nullptr);
            }
        }

        else if (::g_bHideTimer)
        {
            if (!::g_nHideWeapon)
            {
                ::g_nHideWeapon = (*::gpMetaUtilFuncs->pfnGetUserMsgID) (&::Plugin_info, "HideWeapon", nullptr);
            }

            if (!::g_nCrosshair)
            {
                ::g_nCrosshair = (*::gpMetaUtilFuncs->pfnGetUserMsgID) (&::Plugin_info, "Crosshair", nullptr);
            }
        }

        else if (::g_bHideFlash)
        {
            if (!::g_nHideWeapon)
            {
                ::g_nHideWeapon = (*::gpMetaUtilFuncs->pfnGetUserMsgID) (&::Plugin_info, "HideWeapon", nullptr);
            }

            if (!::g_nCrosshair)
            {
                ::g_nCrosshair = (*::gpMetaUtilFuncs->pfnGetUserMsgID) (&::Plugin_info, "Crosshair", nullptr);
            }
        }

        else
        {

        }

        bMsgs = true;
    }

    if (::g_nResetHud > 0)
    {
        if (::g_bResetHud)
        {
            if (::g_nResetHudId > 0)
            {
                if (pEntity = ::INDEXENT(::g_nResetHudId))
                {
                    if (!pEntity->free)
                    {
                        if (pEntity->pvPrivateData)
                        {
                            if (!pEntity->serialnumber)
                            {
                                if (!pEntity->v.deadflag)
                                {
                                    if (pEntity->v.health >= 100.f)
                                    {
                                        if (::g_bNoWeapons)
                                        {
                                            if (!(pEntity->v.flags & FL_DORMANT))
                                            {
                                                if (pEntity->v.flags & FL_CLIENT)
                                                {
                                                    if (::g_nTextMsg > 0)
                                                    {
                                                        if (!::g_strWelcome_1.empty())
                                                        {
                                                            if (::g_strWelcome_1.length() <= 190)
                                                            {
                                                                if (::g_strWelcome_1.size() <= 190)
                                                                {
                                                                    if (!::g_strGameDir.compare("dod"))
                                                                    {
                                                                        if (((nTeam = *((char*)pEntity->pvPrivateData + ::m_iDodTeam + ::g_nOsOffs)) == ((int)('a'))) || (nTeam == ((int)('A'))))
                                                                        {
                                                                            if (!::g_bShow[::g_nResetHudId])
                                                                            {
                                                                                (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                {
                                                                                    (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                    (*::g_engfuncs.pfnWriteString) (::g_strWelcome_1.c_str());
                                                                                }
                                                                                (*::g_engfuncs.pfnMessageEnd) ();

                                                                                if (!::g_strWelcome_2.empty())
                                                                                {
                                                                                    if (::g_strWelcome_2.length() <= 190)
                                                                                    {
                                                                                        if (::g_strWelcome_2.size() <= 190)
                                                                                        {
                                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                            {
                                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                                (*::g_engfuncs.pfnWriteString) (::g_strWelcome_2.c_str());
                                                                                            }
                                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                                        }
                                                                                    }
                                                                                }

                                                                                if (!::g_strWelcome_3.empty())
                                                                                {
                                                                                    if (::g_strWelcome_3.length() <= 190)
                                                                                    {
                                                                                        if (::g_strWelcome_3.size() <= 190)
                                                                                        {
                                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                            {
                                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                                (*::g_engfuncs.pfnWriteString) (::g_strWelcome_3.c_str());
                                                                                            }
                                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                                        }
                                                                                    }
                                                                                }

                                                                                if (!::g_strWelcome_4.empty())
                                                                                {
                                                                                    if (::g_strWelcome_4.length() <= 190)
                                                                                    {
                                                                                        if (::g_strWelcome_4.size() <= 190)
                                                                                        {
                                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                            {
                                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                                (*::g_engfuncs.pfnWriteString) (::g_strWelcome_4.c_str());
                                                                                            }
                                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                                        }
                                                                                    }
                                                                                }

                                                                                if (!::g_strWelcome_5.empty())
                                                                                {
                                                                                    if (::g_strWelcome_5.length() <= 190)
                                                                                    {
                                                                                        if (::g_strWelcome_5.size() <= 190)
                                                                                        {
                                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                            {
                                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                                (*::g_engfuncs.pfnWriteString) (::g_strWelcome_5.c_str());
                                                                                            }
                                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                                        }
                                                                                    }
                                                                                }

                                                                                if (!::g_strWelcome_6.empty())
                                                                                {
                                                                                    if (::g_strWelcome_6.length() <= 190)
                                                                                    {
                                                                                        if (::g_strWelcome_6.size() <= 190)
                                                                                        {
                                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                            {
                                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                                (*::g_engfuncs.pfnWriteString) (::g_strWelcome_6.c_str());
                                                                                            }
                                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                                        }
                                                                                    }
                                                                                }

                                                                                if (!::g_strNotify_1.empty())
                                                                                {
                                                                                    if (::g_strNotify_1.length() <= 190)
                                                                                    {
                                                                                        if (::g_strNotify_1.size() <= 190)
                                                                                        {
                                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                            {
                                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTTALK);

                                                                                                (*::g_engfuncs.pfnWriteString) (::g_strNotify_1.c_str());
                                                                                            }
                                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                                        }
                                                                                    }
                                                                                }

                                                                                if (!::g_strNotify_2.empty())
                                                                                {
                                                                                    if (::g_strNotify_2.length() <= 190)
                                                                                    {
                                                                                        if (::g_strNotify_2.size() <= 190)
                                                                                        {
                                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                            {
                                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTTALK);

                                                                                                (*::g_engfuncs.pfnWriteString) (::g_strNotify_2.c_str());
                                                                                            }
                                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                                        }
                                                                                    }
                                                                                }

                                                                                ::g_bShow[::g_nResetHudId] = true;
                                                                            }
                                                                        }

                                                                        else if ((nTeam == ((int)('b'))) || (nTeam == ((int)('B'))))
                                                                        {
                                                                            if (!::g_bShow[::g_nResetHudId])
                                                                            {
                                                                                (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                {
                                                                                    (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                    (*::g_engfuncs.pfnWriteString) (::g_strWelcome_1.c_str());
                                                                                }
                                                                                (*::g_engfuncs.pfnMessageEnd) ();

                                                                                if (!::g_strWelcome_2.empty())
                                                                                {
                                                                                    if (::g_strWelcome_2.length() <= 190)
                                                                                    {
                                                                                        if (::g_strWelcome_2.size() <= 190)
                                                                                        {
                                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                            {
                                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                                (*::g_engfuncs.pfnWriteString) (::g_strWelcome_2.c_str());
                                                                                            }
                                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                                        }
                                                                                    }
                                                                                }

                                                                                if (!::g_strWelcome_3.empty())
                                                                                {
                                                                                    if (::g_strWelcome_3.length() <= 190)
                                                                                    {
                                                                                        if (::g_strWelcome_3.size() <= 190)
                                                                                        {
                                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                            {
                                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                                (*::g_engfuncs.pfnWriteString) (::g_strWelcome_3.c_str());
                                                                                            }
                                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                                        }
                                                                                    }
                                                                                }

                                                                                if (!::g_strWelcome_4.empty())
                                                                                {
                                                                                    if (::g_strWelcome_4.length() <= 190)
                                                                                    {
                                                                                        if (::g_strWelcome_4.size() <= 190)
                                                                                        {
                                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                            {
                                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                                (*::g_engfuncs.pfnWriteString) (::g_strWelcome_4.c_str());
                                                                                            }
                                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                                        }
                                                                                    }
                                                                                }

                                                                                if (!::g_strWelcome_5.empty())
                                                                                {
                                                                                    if (::g_strWelcome_5.length() <= 190)
                                                                                    {
                                                                                        if (::g_strWelcome_5.size() <= 190)
                                                                                        {
                                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                            {
                                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                                (*::g_engfuncs.pfnWriteString) (::g_strWelcome_5.c_str());
                                                                                            }
                                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                                        }
                                                                                    }
                                                                                }

                                                                                if (!::g_strWelcome_6.empty())
                                                                                {
                                                                                    if (::g_strWelcome_6.length() <= 190)
                                                                                    {
                                                                                        if (::g_strWelcome_6.size() <= 190)
                                                                                        {
                                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                            {
                                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                                (*::g_engfuncs.pfnWriteString) (::g_strWelcome_6.c_str());
                                                                                            }
                                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                                        }
                                                                                    }
                                                                                }

                                                                                if (!::g_strNotify_1.empty())
                                                                                {
                                                                                    if (::g_strNotify_1.length() <= 190)
                                                                                    {
                                                                                        if (::g_strNotify_1.size() <= 190)
                                                                                        {
                                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                            {
                                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTTALK);

                                                                                                (*::g_engfuncs.pfnWriteString) (::g_strNotify_1.c_str());
                                                                                            }
                                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                                        }
                                                                                    }
                                                                                }

                                                                                if (!::g_strNotify_2.empty())
                                                                                {
                                                                                    if (::g_strNotify_2.length() <= 190)
                                                                                    {
                                                                                        if (::g_strNotify_2.size() <= 190)
                                                                                        {
                                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                            {
                                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTTALK);

                                                                                                (*::g_engfuncs.pfnWriteString) (::g_strNotify_2.c_str());
                                                                                            }
                                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                                        }
                                                                                    }
                                                                                }

                                                                                ::g_bShow[::g_nResetHudId] = true;
                                                                            }
                                                                        }

                                                                        else if ((nTeam == ((int)('s'))) || (nTeam == ((int)('S'))))
                                                                        {
                                                                            if (!::g_bShow[::g_nResetHudId])
                                                                            {
                                                                                (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                {
                                                                                    (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                    (*::g_engfuncs.pfnWriteString) (::g_strWelcome_1.c_str());
                                                                                }
                                                                                (*::g_engfuncs.pfnMessageEnd) ();

                                                                                if (!::g_strWelcome_2.empty())
                                                                                {
                                                                                    if (::g_strWelcome_2.length() <= 190)
                                                                                    {
                                                                                        if (::g_strWelcome_2.size() <= 190)
                                                                                        {
                                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                            {
                                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                                (*::g_engfuncs.pfnWriteString) (::g_strWelcome_2.c_str());
                                                                                            }
                                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                                        }
                                                                                    }
                                                                                }

                                                                                if (!::g_strWelcome_3.empty())
                                                                                {
                                                                                    if (::g_strWelcome_3.length() <= 190)
                                                                                    {
                                                                                        if (::g_strWelcome_3.size() <= 190)
                                                                                        {
                                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                            {
                                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                                (*::g_engfuncs.pfnWriteString) (::g_strWelcome_3.c_str());
                                                                                            }
                                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                                        }
                                                                                    }
                                                                                }

                                                                                if (!::g_strWelcome_4.empty())
                                                                                {
                                                                                    if (::g_strWelcome_4.length() <= 190)
                                                                                    {
                                                                                        if (::g_strWelcome_4.size() <= 190)
                                                                                        {
                                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                            {
                                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                                (*::g_engfuncs.pfnWriteString) (::g_strWelcome_4.c_str());
                                                                                            }
                                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                                        }
                                                                                    }
                                                                                }

                                                                                if (!::g_strWelcome_5.empty())
                                                                                {
                                                                                    if (::g_strWelcome_5.length() <= 190)
                                                                                    {
                                                                                        if (::g_strWelcome_5.size() <= 190)
                                                                                        {
                                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                            {
                                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                                (*::g_engfuncs.pfnWriteString) (::g_strWelcome_5.c_str());
                                                                                            }
                                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                                        }
                                                                                    }
                                                                                }

                                                                                if (!::g_strWelcome_6.empty())
                                                                                {
                                                                                    if (::g_strWelcome_6.length() <= 190)
                                                                                    {
                                                                                        if (::g_strWelcome_6.size() <= 190)
                                                                                        {
                                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                            {
                                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                                (*::g_engfuncs.pfnWriteString) (::g_strWelcome_6.c_str());
                                                                                            }
                                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                                        }
                                                                                    }
                                                                                }

                                                                                if (!::g_strNotify_1.empty())
                                                                                {
                                                                                    if (::g_strNotify_1.length() <= 190)
                                                                                    {
                                                                                        if (::g_strNotify_1.size() <= 190)
                                                                                        {
                                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                            {
                                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTTALK);

                                                                                                (*::g_engfuncs.pfnWriteString) (::g_strNotify_1.c_str());
                                                                                            }
                                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                                        }
                                                                                    }
                                                                                }

                                                                                if (!::g_strNotify_2.empty())
                                                                                {
                                                                                    if (::g_strNotify_2.length() <= 190)
                                                                                    {
                                                                                        if (::g_strNotify_2.size() <= 190)
                                                                                        {
                                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                            {
                                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTTALK);

                                                                                                (*::g_engfuncs.pfnWriteString) (::g_strNotify_2.c_str());
                                                                                            }
                                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                                        }
                                                                                    }
                                                                                }

                                                                                ::g_bShow[::g_nResetHudId] = true;
                                                                            }
                                                                        }

                                                                        else if (!nTeam)
                                                                        {

                                                                        }

                                                                        else if ((nTeam == ((int)('u'))) || (nTeam == ((int)('U'))))
                                                                        {

                                                                        }

                                                                        else if ((nTeam == ((int)('n'))) || (nTeam == ((int)('N'))))
                                                                        {

                                                                        }

                                                                        else
                                                                        {

                                                                        }
                                                                    }

                                                                    else
                                                                    {
                                                                        if ((nTeam = *((int*)pEntity->pvPrivateData + ::m_iTeam + ::g_nOsOffs)) == 1)
                                                                        {
                                                                            if (!::g_bShow[::g_nResetHudId])
                                                                            {
                                                                                (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                {
                                                                                    (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                    (*::g_engfuncs.pfnWriteString) (::g_strWelcome_1.c_str());
                                                                                }
                                                                                (*::g_engfuncs.pfnMessageEnd) ();

                                                                                if (!::g_strWelcome_2.empty())
                                                                                {
                                                                                    if (::g_strWelcome_2.length() <= 190)
                                                                                    {
                                                                                        if (::g_strWelcome_2.size() <= 190)
                                                                                        {
                                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                            {
                                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                                (*::g_engfuncs.pfnWriteString) (::g_strWelcome_2.c_str());
                                                                                            }
                                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                                        }
                                                                                    }
                                                                                }

                                                                                if (!::g_strWelcome_3.empty())
                                                                                {
                                                                                    if (::g_strWelcome_3.length() <= 190)
                                                                                    {
                                                                                        if (::g_strWelcome_3.size() <= 190)
                                                                                        {
                                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                            {
                                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                                (*::g_engfuncs.pfnWriteString) (::g_strWelcome_3.c_str());
                                                                                            }
                                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                                        }
                                                                                    }
                                                                                }

                                                                                if (!::g_strWelcome_4.empty())
                                                                                {
                                                                                    if (::g_strWelcome_4.length() <= 190)
                                                                                    {
                                                                                        if (::g_strWelcome_4.size() <= 190)
                                                                                        {
                                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                            {
                                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                                (*::g_engfuncs.pfnWriteString) (::g_strWelcome_4.c_str());
                                                                                            }
                                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                                        }
                                                                                    }
                                                                                }

                                                                                if (!::g_strWelcome_5.empty())
                                                                                {
                                                                                    if (::g_strWelcome_5.length() <= 190)
                                                                                    {
                                                                                        if (::g_strWelcome_5.size() <= 190)
                                                                                        {
                                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                            {
                                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                                (*::g_engfuncs.pfnWriteString) (::g_strWelcome_5.c_str());
                                                                                            }
                                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                                        }
                                                                                    }
                                                                                }

                                                                                if (!::g_strWelcome_6.empty())
                                                                                {
                                                                                    if (::g_strWelcome_6.length() <= 190)
                                                                                    {
                                                                                        if (::g_strWelcome_6.size() <= 190)
                                                                                        {
                                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                            {
                                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                                (*::g_engfuncs.pfnWriteString) (::g_strWelcome_6.c_str());
                                                                                            }
                                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                                        }
                                                                                    }
                                                                                }

                                                                                if (!::g_strNotify_1.empty())
                                                                                {
                                                                                    if (::g_strNotify_1.length() <= 190)
                                                                                    {
                                                                                        if (::g_strNotify_1.size() <= 190)
                                                                                        {
                                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                            {
                                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTTALK);

                                                                                                (*::g_engfuncs.pfnWriteString) (::g_strNotify_1.c_str());
                                                                                            }
                                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                                        }
                                                                                    }
                                                                                }

                                                                                if (!::g_strNotify_2.empty())
                                                                                {
                                                                                    if (::g_strNotify_2.length() <= 190)
                                                                                    {
                                                                                        if (::g_strNotify_2.size() <= 190)
                                                                                        {
                                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                            {
                                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTTALK);

                                                                                                (*::g_engfuncs.pfnWriteString) (::g_strNotify_2.c_str());
                                                                                            }
                                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                                        }
                                                                                    }
                                                                                }

                                                                                ::g_bShow[::g_nResetHudId] = true;
                                                                            }
                                                                        }

                                                                        else if (nTeam == 2)
                                                                        {
                                                                            if (!::g_bShow[::g_nResetHudId])
                                                                            {
                                                                                (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                {
                                                                                    (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                    (*::g_engfuncs.pfnWriteString) (::g_strWelcome_1.c_str());
                                                                                }
                                                                                (*::g_engfuncs.pfnMessageEnd) ();

                                                                                if (!::g_strWelcome_2.empty())
                                                                                {
                                                                                    if (::g_strWelcome_2.length() <= 190)
                                                                                    {
                                                                                        if (::g_strWelcome_2.size() <= 190)
                                                                                        {
                                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                            {
                                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                                (*::g_engfuncs.pfnWriteString) (::g_strWelcome_2.c_str());
                                                                                            }
                                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                                        }
                                                                                    }
                                                                                }

                                                                                if (!::g_strWelcome_3.empty())
                                                                                {
                                                                                    if (::g_strWelcome_3.length() <= 190)
                                                                                    {
                                                                                        if (::g_strWelcome_3.size() <= 190)
                                                                                        {
                                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                            {
                                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                                (*::g_engfuncs.pfnWriteString) (::g_strWelcome_3.c_str());
                                                                                            }
                                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                                        }
                                                                                    }
                                                                                }

                                                                                if (!::g_strWelcome_4.empty())
                                                                                {
                                                                                    if (::g_strWelcome_4.length() <= 190)
                                                                                    {
                                                                                        if (::g_strWelcome_4.size() <= 190)
                                                                                        {
                                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                            {
                                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                                (*::g_engfuncs.pfnWriteString) (::g_strWelcome_4.c_str());
                                                                                            }
                                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                                        }
                                                                                    }
                                                                                }

                                                                                if (!::g_strWelcome_5.empty())
                                                                                {
                                                                                    if (::g_strWelcome_5.length() <= 190)
                                                                                    {
                                                                                        if (::g_strWelcome_5.size() <= 190)
                                                                                        {
                                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                            {
                                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                                (*::g_engfuncs.pfnWriteString) (::g_strWelcome_5.c_str());
                                                                                            }
                                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                                        }
                                                                                    }
                                                                                }

                                                                                if (!::g_strWelcome_6.empty())
                                                                                {
                                                                                    if (::g_strWelcome_6.length() <= 190)
                                                                                    {
                                                                                        if (::g_strWelcome_6.size() <= 190)
                                                                                        {
                                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                            {
                                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                                (*::g_engfuncs.pfnWriteString) (::g_strWelcome_6.c_str());
                                                                                            }
                                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                                        }
                                                                                    }
                                                                                }

                                                                                if (!::g_strNotify_1.empty())
                                                                                {
                                                                                    if (::g_strNotify_1.length() <= 190)
                                                                                    {
                                                                                        if (::g_strNotify_1.size() <= 190)
                                                                                        {
                                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                            {
                                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTTALK);

                                                                                                (*::g_engfuncs.pfnWriteString) (::g_strNotify_1.c_str());
                                                                                            }
                                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                                        }
                                                                                    }
                                                                                }

                                                                                if (!::g_strNotify_2.empty())
                                                                                {
                                                                                    if (::g_strNotify_2.length() <= 190)
                                                                                    {
                                                                                        if (::g_strNotify_2.size() <= 190)
                                                                                        {
                                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                            {
                                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTTALK);

                                                                                                (*::g_engfuncs.pfnWriteString) (::g_strNotify_2.c_str());
                                                                                            }
                                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                                        }
                                                                                    }
                                                                                }

                                                                                ::g_bShow[::g_nResetHudId] = true;
                                                                            }
                                                                        }

                                                                        else if (nTeam == 3)
                                                                        {
                                                                            if (!::g_bShow[::g_nResetHudId])
                                                                            {
                                                                                (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                {
                                                                                    (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                    (*::g_engfuncs.pfnWriteString) (::g_strWelcome_1.c_str());
                                                                                }
                                                                                (*::g_engfuncs.pfnMessageEnd) ();

                                                                                if (!::g_strWelcome_2.empty())
                                                                                {
                                                                                    if (::g_strWelcome_2.length() <= 190)
                                                                                    {
                                                                                        if (::g_strWelcome_2.size() <= 190)
                                                                                        {
                                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                            {
                                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                                (*::g_engfuncs.pfnWriteString) (::g_strWelcome_2.c_str());
                                                                                            }
                                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                                        }
                                                                                    }
                                                                                }

                                                                                if (!::g_strWelcome_3.empty())
                                                                                {
                                                                                    if (::g_strWelcome_3.length() <= 190)
                                                                                    {
                                                                                        if (::g_strWelcome_3.size() <= 190)
                                                                                        {
                                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                            {
                                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                                (*::g_engfuncs.pfnWriteString) (::g_strWelcome_3.c_str());
                                                                                            }
                                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                                        }
                                                                                    }
                                                                                }

                                                                                if (!::g_strWelcome_4.empty())
                                                                                {
                                                                                    if (::g_strWelcome_4.length() <= 190)
                                                                                    {
                                                                                        if (::g_strWelcome_4.size() <= 190)
                                                                                        {
                                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                            {
                                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                                (*::g_engfuncs.pfnWriteString) (::g_strWelcome_4.c_str());
                                                                                            }
                                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                                        }
                                                                                    }
                                                                                }

                                                                                if (!::g_strWelcome_5.empty())
                                                                                {
                                                                                    if (::g_strWelcome_5.length() <= 190)
                                                                                    {
                                                                                        if (::g_strWelcome_5.size() <= 190)
                                                                                        {
                                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                            {
                                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                                (*::g_engfuncs.pfnWriteString) (::g_strWelcome_5.c_str());
                                                                                            }
                                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                                        }
                                                                                    }
                                                                                }

                                                                                if (!::g_strWelcome_6.empty())
                                                                                {
                                                                                    if (::g_strWelcome_6.length() <= 190)
                                                                                    {
                                                                                        if (::g_strWelcome_6.size() <= 190)
                                                                                        {
                                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                            {
                                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                                (*::g_engfuncs.pfnWriteString) (::g_strWelcome_6.c_str());
                                                                                            }
                                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                                        }
                                                                                    }
                                                                                }

                                                                                if (!::g_strNotify_1.empty())
                                                                                {
                                                                                    if (::g_strNotify_1.length() <= 190)
                                                                                    {
                                                                                        if (::g_strNotify_1.size() <= 190)
                                                                                        {
                                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                            {
                                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTTALK);

                                                                                                (*::g_engfuncs.pfnWriteString) (::g_strNotify_1.c_str());
                                                                                            }
                                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                                        }
                                                                                    }
                                                                                }

                                                                                if (!::g_strNotify_2.empty())
                                                                                {
                                                                                    if (::g_strNotify_2.length() <= 190)
                                                                                    {
                                                                                        if (::g_strNotify_2.size() <= 190)
                                                                                        {
                                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                            {
                                                                                                (*::g_engfuncs.pfnWriteByte) (HUD_PRINTTALK);

                                                                                                (*::g_engfuncs.pfnWriteString) (::g_strNotify_2.c_str());
                                                                                            }
                                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                                        }
                                                                                    }
                                                                                }

                                                                                ::g_bShow[::g_nResetHudId] = true;
                                                                            }
                                                                        }

                                                                        else if (!nTeam)
                                                                        {

                                                                        }

                                                                        else if (nTeam < 0)
                                                                        {

                                                                        }

                                                                        else
                                                                        {

                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }

                                                    bAltered = { };

                                                    if (::g_bFixScore)
                                                    {
                                                        if (pEntity->v.frags < 0)
                                                        {
                                                            bAltered = true;
                                                            {
                                                                pEntity->v.frags = { };
                                                            }
                                                        }

                                                        if (pEntity->v.frags < 1)
                                                        {
                                                            if (!::g_strGameDir.compare("dod"))
                                                            {
                                                                if (*((int*)pEntity->pvPrivateData + ::m_iDodDeaths + ::g_nOsOffs) > 0)
                                                                {
                                                                    *((int*)pEntity->pvPrivateData + ::m_iDodDeaths + ::g_nOsOffs) = { };
                                                                    {
                                                                        bAltered = true;
                                                                    }
                                                                }
                                                            }

                                                            else
                                                            {
                                                                if (*((int*)pEntity->pvPrivateData + ::m_iDeaths + ::g_nOsOffs) > 0)
                                                                {
                                                                    *((int*)pEntity->pvPrivateData + ::m_iDeaths + ::g_nOsOffs) = { };
                                                                    {
                                                                        bAltered = true;
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }

                                                    else if (::g_bFixKills)
                                                    {
                                                        if (pEntity->v.frags < 0)
                                                        {
                                                            bAltered = true;
                                                            {
                                                                pEntity->v.frags = { };
                                                            }
                                                        }
                                                    }

                                                    else if (::g_bFixDeaths)
                                                    {
                                                        if (pEntity->v.frags < 1)
                                                        {
                                                            if (!::g_strGameDir.compare("dod"))
                                                            {
                                                                if (*((int*)pEntity->pvPrivateData + ::m_iDodDeaths + ::g_nOsOffs) > 0)
                                                                {
                                                                    *((int*)pEntity->pvPrivateData + ::m_iDodDeaths + ::g_nOsOffs) = { };
                                                                    {
                                                                        bAltered = true;
                                                                    }
                                                                }
                                                            }

                                                            else
                                                            {
                                                                if (*((int*)pEntity->pvPrivateData + ::m_iDeaths + ::g_nOsOffs) > 0)
                                                                {
                                                                    *((int*)pEntity->pvPrivateData + ::m_iDeaths + ::g_nOsOffs) = { };
                                                                    {
                                                                        bAltered = true;
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }

                                                    if (::g_bZeroMoney)
                                                    {
                                                        if (*((int*)pEntity->pvPrivateData + ::m_iMoney + ::g_nOsOffs))
                                                        {
                                                            *((int*)pEntity->pvPrivateData + ::m_iMoney + ::g_nOsOffs) = { };
                                                        }
                                                    }

                                                    if (::g_bRealScore)
                                                    {
                                                        if (::g_nKills[::g_nResetHudId] || ::g_nDeaths[::g_nResetHudId])
                                                        {
                                                            if (!pEntity->v.frags)
                                                            {
                                                                if (!::g_strGameDir.compare("dod"))
                                                                {
                                                                    if (!(*((int*)pEntity->pvPrivateData + ::m_iDodDeaths + ::g_nOsOffs)))
                                                                    {
                                                                        ::g_nKills[::g_nResetHudId] = { };
                                                                        {
                                                                            ::g_nDeaths[::g_nResetHudId] = { };
                                                                        }
                                                                    }
                                                                }

                                                                else
                                                                {
                                                                    if (!(*((int*)pEntity->pvPrivateData + ::m_iDeaths + ::g_nOsOffs)))
                                                                    {
                                                                        ::g_nKills[::g_nResetHudId] = { };
                                                                        {
                                                                            ::g_nDeaths[::g_nResetHudId] = { };
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }

                                                        if (pEntity->v.frags != ::g_nKills[::g_nResetHudId])
                                                        {
                                                            bAltered = true;
                                                            {
                                                                pEntity->v.frags = ::g_nKills[::g_nResetHudId];
                                                            }
                                                        }

                                                        if (!::g_strGameDir.compare("dod"))
                                                        {
                                                            if (*((int*)pEntity->pvPrivateData + ::m_iDodDeaths + ::g_nOsOffs) != ::g_nDeaths[::g_nResetHudId])
                                                            {
                                                                *((int*)pEntity->pvPrivateData + ::m_iDodDeaths + ::g_nOsOffs) = ::g_nDeaths[::g_nResetHudId];
                                                                {
                                                                    bAltered = true;
                                                                }
                                                            }
                                                        }

                                                        else
                                                        {
                                                            if (*((int*)pEntity->pvPrivateData + ::m_iDeaths + ::g_nOsOffs) != ::g_nDeaths[::g_nResetHudId])
                                                            {
                                                                *((int*)pEntity->pvPrivateData + ::m_iDeaths + ::g_nOsOffs) = ::g_nDeaths[::g_nResetHudId];
                                                                {
                                                                    bAltered = true;
                                                                }
                                                            }
                                                        }
                                                    }

                                                    if (::g_bFixScore || ::g_bFixKills || ::g_bFixDeaths || ::g_bRealScore)
                                                    {
                                                        if (bAltered)
                                                        {
                                                            if (!::g_strGameDir.compare("dod"))
                                                            {
                                                                if (::g_nScoreShort > 0)
                                                                {
                                                                    (*::g_engfuncs.pfnMessageBegin) (::g_bAllReliable ? MSG_ALL : MSG_BROADCAST, ::g_nScoreShort, nullptr, nullptr);
                                                                    {
                                                                        (*::g_engfuncs.pfnWriteByte) (::g_nResetHudId);
                                                                        {
                                                                            (*::g_engfuncs.pfnWriteShort) (*((int*)pEntity->pvPrivateData + ::m_iDodScore + ::g_nOsOffs));
                                                                            {
                                                                                (*::g_engfuncs.pfnWriteShort) (pEntity->v.frags);
                                                                            }
                                                                            (*::g_engfuncs.pfnWriteShort) (*((int*)pEntity->pvPrivateData + ::m_iDodDeaths + ::g_nOsOffs));
                                                                        }
                                                                        (*::g_engfuncs.pfnWriteByte) (xTo(true, int));
                                                                    }
                                                                    (*::g_engfuncs.pfnMessageEnd) ();
                                                                }
                                                            }

                                                            else
                                                            {
                                                                if (::g_nScoreInfo > 0)
                                                                {
                                                                    (*::g_engfuncs.pfnMessageBegin) (::g_bAllReliable ? MSG_ALL : MSG_BROADCAST, ::g_nScoreInfo, nullptr, nullptr);
                                                                    {
                                                                        (*::g_engfuncs.pfnWriteByte) (::g_nResetHudId);
                                                                        {
                                                                            (*::g_engfuncs.pfnWriteShort) (pEntity->v.frags);
                                                                            (*::g_engfuncs.pfnWriteShort) (*((int*)pEntity->pvPrivateData + ::m_iDeaths + ::g_nOsOffs));
                                                                            (*::g_engfuncs.pfnWriteShort) ({ });
                                                                            (*::g_engfuncs.pfnWriteShort) (*((int*)pEntity->pvPrivateData + ::m_iTeam + ::g_nOsOffs));
                                                                        }
                                                                    }
                                                                    (*::g_engfuncs.pfnMessageEnd) ();
                                                                }
                                                            }
                                                        }
                                                    }

                                                    if (::g_bHideRadar)
                                                    {
                                                        if (!::g_bRadar[::g_nResetHudId])
                                                        {
                                                            (*::g_engfuncs.pfnClientCommand) (pEntity, "hideradar\n");
                                                            {
                                                                ::g_bRadar[::g_nResetHudId] = true;
                                                            }
                                                        }
                                                    }

                                                    if (::g_bHideMoney || ::g_bHideTimer || ::g_bHideFlash)
                                                    {
                                                        if (::g_nHideWeapon > 0)
                                                        {
                                                            if (::g_nCrosshair > 0)
                                                            {
                                                                if ((nFov = *((int*)pEntity->pvPrivateData + ::m_iFov + ::g_nOsOffs)) != 40)
                                                                {
                                                                    if (nFov != 10)
                                                                    {
                                                                        if (nFov != 15)
                                                                        {
                                                                            if (pEntity->v.fov != 40.f)
                                                                            {
                                                                                if (pEntity->v.fov != 10.f)
                                                                                {
                                                                                    if (pEntity->v.fov != 15.f)
                                                                                    {
                                                                                        if (!::g_bHide[::g_nResetHudId])
                                                                                        {
                                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nHideWeapon, nullptr, pEntity);
                                                                                            {
                                                                                                if (::g_bHideMoney)
                                                                                                {
                                                                                                    if (::g_bHideTimer)
                                                                                                    {
                                                                                                        if (::g_bHideFlash)
                                                                                                        {
                                                                                                            (*::g_engfuncs.pfnWriteByte) (50);
                                                                                                        }

                                                                                                        else
                                                                                                        {
                                                                                                            (*::g_engfuncs.pfnWriteByte) (48);
                                                                                                        }
                                                                                                    }

                                                                                                    else
                                                                                                    {
                                                                                                        if (::g_bHideFlash)
                                                                                                        {
                                                                                                            (*::g_engfuncs.pfnWriteByte) (34);
                                                                                                        }

                                                                                                        else
                                                                                                        {
                                                                                                            (*::g_engfuncs.pfnWriteByte) (32);
                                                                                                        }
                                                                                                    }
                                                                                                }

                                                                                                else if (::g_bHideTimer)
                                                                                                {
                                                                                                    if (::g_bHideFlash)
                                                                                                    {
                                                                                                        (*::g_engfuncs.pfnWriteByte) (18);
                                                                                                    }

                                                                                                    else
                                                                                                    {
                                                                                                        (*::g_engfuncs.pfnWriteByte) (16);
                                                                                                    }
                                                                                                }

                                                                                                else
                                                                                                {
                                                                                                    (*::g_engfuncs.pfnWriteByte) (2);
                                                                                                }
                                                                                            }
                                                                                            (*::g_engfuncs.pfnMessageEnd) ();

                                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nCrosshair, nullptr, pEntity);
                                                                                            {
                                                                                                (*::g_engfuncs.pfnWriteByte) ({ });
                                                                                            }
                                                                                            (*::g_engfuncs.pfnMessageEnd) ();

                                                                                            ::g_bHide[::g_nResetHudId] = true;
                                                                                        }
                                                                                    }
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }

                                                    if (::g_nArmor)
                                                    {
                                                        if (pEntity->v.armorvalue < 100.f)
                                                        {
                                                            if (!::g_bArmor[::g_nResetHudId])
                                                            {
                                                                if (!::g_bKeepHelmet)
                                                                {
                                                                    pEntity->v.armorvalue = 100.f;
                                                                    {
                                                                        if (*((int*)pEntity->pvPrivateData + ::m_iKevlar + ::g_nOsOffs) != ::g_nArmor)
                                                                        {
                                                                            *((int*)pEntity->pvPrivateData + ::m_iKevlar + ::g_nOsOffs) = ::g_nArmor;
                                                                        }
                                                                    }

                                                                    if (::g_nArmorType > 0)
                                                                    {
                                                                        (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nArmorType, nullptr, pEntity);
                                                                        {
                                                                            (*::g_engfuncs.pfnWriteByte) (::g_nArmor - 1);
                                                                        }
                                                                        (*::g_engfuncs.pfnMessageEnd) ();
                                                                    }
                                                                }

                                                                else if (::g_nArmor > 1)
                                                                {
                                                                    pEntity->v.armorvalue = 100.f;
                                                                    {
                                                                        if (*((int*)pEntity->pvPrivateData + ::m_iKevlar + ::g_nOsOffs) != ::g_nArmor)
                                                                        {
                                                                            *((int*)pEntity->pvPrivateData + ::m_iKevlar + ::g_nOsOffs) = ::g_nArmor;
                                                                        }
                                                                    }

                                                                    if (::g_nArmorType > 0)
                                                                    {
                                                                        (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nArmorType, nullptr, pEntity);
                                                                        {
                                                                            (*::g_engfuncs.pfnWriteByte) (::g_nArmor - 1);
                                                                        }
                                                                        (*::g_engfuncs.pfnMessageEnd) ();
                                                                    }
                                                                }

                                                                else
                                                                {
                                                                    pEntity->v.armorvalue = 100.f;
                                                                    {
                                                                        if ((nKevlar = *((int*)pEntity->pvPrivateData + ::m_iKevlar + ::g_nOsOffs)) < 2)
                                                                        {
                                                                            if (nKevlar != ::g_nArmor)
                                                                            {
                                                                                *((int*)pEntity->pvPrivateData + ::m_iKevlar + ::g_nOsOffs) = ::g_nArmor;
                                                                            }
                                                                        }
                                                                    }

                                                                    if (::g_nArmorType > 0)
                                                                    {
                                                                        (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nArmorType, nullptr, pEntity);
                                                                        {
                                                                            if (*((int*)pEntity->pvPrivateData + ::m_iKevlar + ::g_nOsOffs) < 2)
                                                                            {
                                                                                (*::g_engfuncs.pfnWriteByte) (::g_nArmor - 1);
                                                                            }

                                                                            else
                                                                            {
                                                                                (*::g_engfuncs.pfnWriteByte) (xTo(true, int));
                                                                            }
                                                                        }
                                                                        (*::g_engfuncs.pfnMessageEnd) ();
                                                                    }
                                                                }

                                                                ::g_bArmor[::g_nResetHudId] = true;
                                                            }
                                                        }

                                                        else
                                                        {
                                                            ::g_bArmor[::g_nResetHudId] = true;
                                                        }
                                                    }

                                                    else
                                                    {
                                                        ::g_bArmor[::g_nResetHudId] = { };
                                                    }
                                                }
                                            }
                                        }

                                        else
                                        {
                                            if (pEntity->v.weapons)
                                            {
                                                if (!(pEntity->v.flags & FL_DORMANT))
                                                {
                                                    if (pEntity->v.flags & FL_CLIENT)
                                                    {
                                                        if (::g_nTextMsg > 0)
                                                        {
                                                            if (!::g_strWelcome_1.empty())
                                                            {
                                                                if (::g_strWelcome_1.length() <= 190)
                                                                {
                                                                    if (::g_strWelcome_1.size() <= 190)
                                                                    {
                                                                        if (!::g_strGameDir.compare("dod"))
                                                                        {
                                                                            if (((nTeam = *((char*)pEntity->pvPrivateData + ::m_iDodTeam + ::g_nOsOffs)) == ((int)('a'))) || (nTeam == ((int)('A'))))
                                                                            {
                                                                                if (!::g_bShow[::g_nResetHudId])
                                                                                {
                                                                                    (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                    {
                                                                                        (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                        (*::g_engfuncs.pfnWriteString) (::g_strWelcome_1.c_str());
                                                                                    }
                                                                                    (*::g_engfuncs.pfnMessageEnd) ();

                                                                                    if (!::g_strWelcome_2.empty())
                                                                                    {
                                                                                        if (::g_strWelcome_2.length() <= 190)
                                                                                        {
                                                                                            if (::g_strWelcome_2.size() <= 190)
                                                                                            {
                                                                                                (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                                {
                                                                                                    (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                                    (*::g_engfuncs.pfnWriteString) (::g_strWelcome_2.c_str());
                                                                                                }
                                                                                                (*::g_engfuncs.pfnMessageEnd) ();
                                                                                            }
                                                                                        }
                                                                                    }

                                                                                    if (!::g_strWelcome_3.empty())
                                                                                    {
                                                                                        if (::g_strWelcome_3.length() <= 190)
                                                                                        {
                                                                                            if (::g_strWelcome_3.size() <= 190)
                                                                                            {
                                                                                                (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                                {
                                                                                                    (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                                    (*::g_engfuncs.pfnWriteString) (::g_strWelcome_3.c_str());
                                                                                                }
                                                                                                (*::g_engfuncs.pfnMessageEnd) ();
                                                                                            }
                                                                                        }
                                                                                    }

                                                                                    if (!::g_strWelcome_4.empty())
                                                                                    {
                                                                                        if (::g_strWelcome_4.length() <= 190)
                                                                                        {
                                                                                            if (::g_strWelcome_4.size() <= 190)
                                                                                            {
                                                                                                (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                                {
                                                                                                    (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                                    (*::g_engfuncs.pfnWriteString) (::g_strWelcome_4.c_str());
                                                                                                }
                                                                                                (*::g_engfuncs.pfnMessageEnd) ();
                                                                                            }
                                                                                        }
                                                                                    }

                                                                                    if (!::g_strWelcome_5.empty())
                                                                                    {
                                                                                        if (::g_strWelcome_5.length() <= 190)
                                                                                        {
                                                                                            if (::g_strWelcome_5.size() <= 190)
                                                                                            {
                                                                                                (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                                {
                                                                                                    (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                                    (*::g_engfuncs.pfnWriteString) (::g_strWelcome_5.c_str());
                                                                                                }
                                                                                                (*::g_engfuncs.pfnMessageEnd) ();
                                                                                            }
                                                                                        }
                                                                                    }

                                                                                    if (!::g_strWelcome_6.empty())
                                                                                    {
                                                                                        if (::g_strWelcome_6.length() <= 190)
                                                                                        {
                                                                                            if (::g_strWelcome_6.size() <= 190)
                                                                                            {
                                                                                                (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                                {
                                                                                                    (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                                    (*::g_engfuncs.pfnWriteString) (::g_strWelcome_6.c_str());
                                                                                                }
                                                                                                (*::g_engfuncs.pfnMessageEnd) ();
                                                                                            }
                                                                                        }
                                                                                    }

                                                                                    if (!::g_strNotify_1.empty())
                                                                                    {
                                                                                        if (::g_strNotify_1.length() <= 190)
                                                                                        {
                                                                                            if (::g_strNotify_1.size() <= 190)
                                                                                            {
                                                                                                (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                                {
                                                                                                    (*::g_engfuncs.pfnWriteByte) (HUD_PRINTTALK);

                                                                                                    (*::g_engfuncs.pfnWriteString) (::g_strNotify_1.c_str());
                                                                                                }
                                                                                                (*::g_engfuncs.pfnMessageEnd) ();
                                                                                            }
                                                                                        }
                                                                                    }

                                                                                    if (!::g_strNotify_2.empty())
                                                                                    {
                                                                                        if (::g_strNotify_2.length() <= 190)
                                                                                        {
                                                                                            if (::g_strNotify_2.size() <= 190)
                                                                                            {
                                                                                                (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                                {
                                                                                                    (*::g_engfuncs.pfnWriteByte) (HUD_PRINTTALK);

                                                                                                    (*::g_engfuncs.pfnWriteString) (::g_strNotify_2.c_str());
                                                                                                }
                                                                                                (*::g_engfuncs.pfnMessageEnd) ();
                                                                                            }
                                                                                        }
                                                                                    }

                                                                                    ::g_bShow[::g_nResetHudId] = true;
                                                                                }
                                                                            }

                                                                            else if ((nTeam == ((int)('b'))) || (nTeam == ((int)('B'))))
                                                                            {
                                                                                if (!::g_bShow[::g_nResetHudId])
                                                                                {
                                                                                    (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                    {
                                                                                        (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                        (*::g_engfuncs.pfnWriteString) (::g_strWelcome_1.c_str());
                                                                                    }
                                                                                    (*::g_engfuncs.pfnMessageEnd) ();

                                                                                    if (!::g_strWelcome_2.empty())
                                                                                    {
                                                                                        if (::g_strWelcome_2.length() <= 190)
                                                                                        {
                                                                                            if (::g_strWelcome_2.size() <= 190)
                                                                                            {
                                                                                                (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                                {
                                                                                                    (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                                    (*::g_engfuncs.pfnWriteString) (::g_strWelcome_2.c_str());
                                                                                                }
                                                                                                (*::g_engfuncs.pfnMessageEnd) ();
                                                                                            }
                                                                                        }
                                                                                    }

                                                                                    if (!::g_strWelcome_3.empty())
                                                                                    {
                                                                                        if (::g_strWelcome_3.length() <= 190)
                                                                                        {
                                                                                            if (::g_strWelcome_3.size() <= 190)
                                                                                            {
                                                                                                (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                                {
                                                                                                    (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                                    (*::g_engfuncs.pfnWriteString) (::g_strWelcome_3.c_str());
                                                                                                }
                                                                                                (*::g_engfuncs.pfnMessageEnd) ();
                                                                                            }
                                                                                        }
                                                                                    }

                                                                                    if (!::g_strWelcome_4.empty())
                                                                                    {
                                                                                        if (::g_strWelcome_4.length() <= 190)
                                                                                        {
                                                                                            if (::g_strWelcome_4.size() <= 190)
                                                                                            {
                                                                                                (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                                {
                                                                                                    (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                                    (*::g_engfuncs.pfnWriteString) (::g_strWelcome_4.c_str());
                                                                                                }
                                                                                                (*::g_engfuncs.pfnMessageEnd) ();
                                                                                            }
                                                                                        }
                                                                                    }

                                                                                    if (!::g_strWelcome_5.empty())
                                                                                    {
                                                                                        if (::g_strWelcome_5.length() <= 190)
                                                                                        {
                                                                                            if (::g_strWelcome_5.size() <= 190)
                                                                                            {
                                                                                                (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                                {
                                                                                                    (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                                    (*::g_engfuncs.pfnWriteString) (::g_strWelcome_5.c_str());
                                                                                                }
                                                                                                (*::g_engfuncs.pfnMessageEnd) ();
                                                                                            }
                                                                                        }
                                                                                    }

                                                                                    if (!::g_strWelcome_6.empty())
                                                                                    {
                                                                                        if (::g_strWelcome_6.length() <= 190)
                                                                                        {
                                                                                            if (::g_strWelcome_6.size() <= 190)
                                                                                            {
                                                                                                (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                                {
                                                                                                    (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                                    (*::g_engfuncs.pfnWriteString) (::g_strWelcome_6.c_str());
                                                                                                }
                                                                                                (*::g_engfuncs.pfnMessageEnd) ();
                                                                                            }
                                                                                        }
                                                                                    }

                                                                                    if (!::g_strNotify_1.empty())
                                                                                    {
                                                                                        if (::g_strNotify_1.length() <= 190)
                                                                                        {
                                                                                            if (::g_strNotify_1.size() <= 190)
                                                                                            {
                                                                                                (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                                {
                                                                                                    (*::g_engfuncs.pfnWriteByte) (HUD_PRINTTALK);

                                                                                                    (*::g_engfuncs.pfnWriteString) (::g_strNotify_1.c_str());
                                                                                                }
                                                                                                (*::g_engfuncs.pfnMessageEnd) ();
                                                                                            }
                                                                                        }
                                                                                    }

                                                                                    if (!::g_strNotify_2.empty())
                                                                                    {
                                                                                        if (::g_strNotify_2.length() <= 190)
                                                                                        {
                                                                                            if (::g_strNotify_2.size() <= 190)
                                                                                            {
                                                                                                (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                                {
                                                                                                    (*::g_engfuncs.pfnWriteByte) (HUD_PRINTTALK);

                                                                                                    (*::g_engfuncs.pfnWriteString) (::g_strNotify_2.c_str());
                                                                                                }
                                                                                                (*::g_engfuncs.pfnMessageEnd) ();
                                                                                            }
                                                                                        }
                                                                                    }

                                                                                    ::g_bShow[::g_nResetHudId] = true;
                                                                                }
                                                                            }

                                                                            else if ((nTeam == ((int)('s'))) || (nTeam == ((int)('S'))))
                                                                            {
                                                                                if (!::g_bShow[::g_nResetHudId])
                                                                                {
                                                                                    (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                    {
                                                                                        (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                        (*::g_engfuncs.pfnWriteString) (::g_strWelcome_1.c_str());
                                                                                    }
                                                                                    (*::g_engfuncs.pfnMessageEnd) ();

                                                                                    if (!::g_strWelcome_2.empty())
                                                                                    {
                                                                                        if (::g_strWelcome_2.length() <= 190)
                                                                                        {
                                                                                            if (::g_strWelcome_2.size() <= 190)
                                                                                            {
                                                                                                (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                                {
                                                                                                    (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                                    (*::g_engfuncs.pfnWriteString) (::g_strWelcome_2.c_str());
                                                                                                }
                                                                                                (*::g_engfuncs.pfnMessageEnd) ();
                                                                                            }
                                                                                        }
                                                                                    }

                                                                                    if (!::g_strWelcome_3.empty())
                                                                                    {
                                                                                        if (::g_strWelcome_3.length() <= 190)
                                                                                        {
                                                                                            if (::g_strWelcome_3.size() <= 190)
                                                                                            {
                                                                                                (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                                {
                                                                                                    (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                                    (*::g_engfuncs.pfnWriteString) (::g_strWelcome_3.c_str());
                                                                                                }
                                                                                                (*::g_engfuncs.pfnMessageEnd) ();
                                                                                            }
                                                                                        }
                                                                                    }

                                                                                    if (!::g_strWelcome_4.empty())
                                                                                    {
                                                                                        if (::g_strWelcome_4.length() <= 190)
                                                                                        {
                                                                                            if (::g_strWelcome_4.size() <= 190)
                                                                                            {
                                                                                                (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                                {
                                                                                                    (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                                    (*::g_engfuncs.pfnWriteString) (::g_strWelcome_4.c_str());
                                                                                                }
                                                                                                (*::g_engfuncs.pfnMessageEnd) ();
                                                                                            }
                                                                                        }
                                                                                    }

                                                                                    if (!::g_strWelcome_5.empty())
                                                                                    {
                                                                                        if (::g_strWelcome_5.length() <= 190)
                                                                                        {
                                                                                            if (::g_strWelcome_5.size() <= 190)
                                                                                            {
                                                                                                (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                                {
                                                                                                    (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                                    (*::g_engfuncs.pfnWriteString) (::g_strWelcome_5.c_str());
                                                                                                }
                                                                                                (*::g_engfuncs.pfnMessageEnd) ();
                                                                                            }
                                                                                        }
                                                                                    }

                                                                                    if (!::g_strWelcome_6.empty())
                                                                                    {
                                                                                        if (::g_strWelcome_6.length() <= 190)
                                                                                        {
                                                                                            if (::g_strWelcome_6.size() <= 190)
                                                                                            {
                                                                                                (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                                {
                                                                                                    (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                                    (*::g_engfuncs.pfnWriteString) (::g_strWelcome_6.c_str());
                                                                                                }
                                                                                                (*::g_engfuncs.pfnMessageEnd) ();
                                                                                            }
                                                                                        }
                                                                                    }

                                                                                    if (!::g_strNotify_1.empty())
                                                                                    {
                                                                                        if (::g_strNotify_1.length() <= 190)
                                                                                        {
                                                                                            if (::g_strNotify_1.size() <= 190)
                                                                                            {
                                                                                                (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                                {
                                                                                                    (*::g_engfuncs.pfnWriteByte) (HUD_PRINTTALK);

                                                                                                    (*::g_engfuncs.pfnWriteString) (::g_strNotify_1.c_str());
                                                                                                }
                                                                                                (*::g_engfuncs.pfnMessageEnd) ();
                                                                                            }
                                                                                        }
                                                                                    }

                                                                                    if (!::g_strNotify_2.empty())
                                                                                    {
                                                                                        if (::g_strNotify_2.length() <= 190)
                                                                                        {
                                                                                            if (::g_strNotify_2.size() <= 190)
                                                                                            {
                                                                                                (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                                {
                                                                                                    (*::g_engfuncs.pfnWriteByte) (HUD_PRINTTALK);

                                                                                                    (*::g_engfuncs.pfnWriteString) (::g_strNotify_2.c_str());
                                                                                                }
                                                                                                (*::g_engfuncs.pfnMessageEnd) ();
                                                                                            }
                                                                                        }
                                                                                    }

                                                                                    ::g_bShow[::g_nResetHudId] = true;
                                                                                }
                                                                            }

                                                                            else if (!nTeam)
                                                                            {

                                                                            }

                                                                            else if ((nTeam == ((int)('u'))) || (nTeam == ((int)('U'))))
                                                                            {

                                                                            }

                                                                            else if ((nTeam == ((int)('n'))) || (nTeam == ((int)('N'))))
                                                                            {

                                                                            }

                                                                            else
                                                                            {

                                                                            }
                                                                        }

                                                                        else
                                                                        {
                                                                            if ((nTeam = *((int*)pEntity->pvPrivateData + ::m_iTeam + ::g_nOsOffs)) == 1)
                                                                            {
                                                                                if (!::g_bShow[::g_nResetHudId])
                                                                                {
                                                                                    (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                    {
                                                                                        (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                        (*::g_engfuncs.pfnWriteString) (::g_strWelcome_1.c_str());
                                                                                    }
                                                                                    (*::g_engfuncs.pfnMessageEnd) ();

                                                                                    if (!::g_strWelcome_2.empty())
                                                                                    {
                                                                                        if (::g_strWelcome_2.length() <= 190)
                                                                                        {
                                                                                            if (::g_strWelcome_2.size() <= 190)
                                                                                            {
                                                                                                (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                                {
                                                                                                    (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                                    (*::g_engfuncs.pfnWriteString) (::g_strWelcome_2.c_str());
                                                                                                }
                                                                                                (*::g_engfuncs.pfnMessageEnd) ();
                                                                                            }
                                                                                        }
                                                                                    }

                                                                                    if (!::g_strWelcome_3.empty())
                                                                                    {
                                                                                        if (::g_strWelcome_3.length() <= 190)
                                                                                        {
                                                                                            if (::g_strWelcome_3.size() <= 190)
                                                                                            {
                                                                                                (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                                {
                                                                                                    (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                                    (*::g_engfuncs.pfnWriteString) (::g_strWelcome_3.c_str());
                                                                                                }
                                                                                                (*::g_engfuncs.pfnMessageEnd) ();
                                                                                            }
                                                                                        }
                                                                                    }

                                                                                    if (!::g_strWelcome_4.empty())
                                                                                    {
                                                                                        if (::g_strWelcome_4.length() <= 190)
                                                                                        {
                                                                                            if (::g_strWelcome_4.size() <= 190)
                                                                                            {
                                                                                                (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                                {
                                                                                                    (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                                    (*::g_engfuncs.pfnWriteString) (::g_strWelcome_4.c_str());
                                                                                                }
                                                                                                (*::g_engfuncs.pfnMessageEnd) ();
                                                                                            }
                                                                                        }
                                                                                    }

                                                                                    if (!::g_strWelcome_5.empty())
                                                                                    {
                                                                                        if (::g_strWelcome_5.length() <= 190)
                                                                                        {
                                                                                            if (::g_strWelcome_5.size() <= 190)
                                                                                            {
                                                                                                (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                                {
                                                                                                    (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                                    (*::g_engfuncs.pfnWriteString) (::g_strWelcome_5.c_str());
                                                                                                }
                                                                                                (*::g_engfuncs.pfnMessageEnd) ();
                                                                                            }
                                                                                        }
                                                                                    }

                                                                                    if (!::g_strWelcome_6.empty())
                                                                                    {
                                                                                        if (::g_strWelcome_6.length() <= 190)
                                                                                        {
                                                                                            if (::g_strWelcome_6.size() <= 190)
                                                                                            {
                                                                                                (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                                {
                                                                                                    (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                                    (*::g_engfuncs.pfnWriteString) (::g_strWelcome_6.c_str());
                                                                                                }
                                                                                                (*::g_engfuncs.pfnMessageEnd) ();
                                                                                            }
                                                                                        }
                                                                                    }

                                                                                    if (!::g_strNotify_1.empty())
                                                                                    {
                                                                                        if (::g_strNotify_1.length() <= 190)
                                                                                        {
                                                                                            if (::g_strNotify_1.size() <= 190)
                                                                                            {
                                                                                                (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                                {
                                                                                                    (*::g_engfuncs.pfnWriteByte) (HUD_PRINTTALK);

                                                                                                    (*::g_engfuncs.pfnWriteString) (::g_strNotify_1.c_str());
                                                                                                }
                                                                                                (*::g_engfuncs.pfnMessageEnd) ();
                                                                                            }
                                                                                        }
                                                                                    }

                                                                                    if (!::g_strNotify_2.empty())
                                                                                    {
                                                                                        if (::g_strNotify_2.length() <= 190)
                                                                                        {
                                                                                            if (::g_strNotify_2.size() <= 190)
                                                                                            {
                                                                                                (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                                {
                                                                                                    (*::g_engfuncs.pfnWriteByte) (HUD_PRINTTALK);

                                                                                                    (*::g_engfuncs.pfnWriteString) (::g_strNotify_2.c_str());
                                                                                                }
                                                                                                (*::g_engfuncs.pfnMessageEnd) ();
                                                                                            }
                                                                                        }
                                                                                    }

                                                                                    ::g_bShow[::g_nResetHudId] = true;
                                                                                }
                                                                            }

                                                                            else if (nTeam == 2)
                                                                            {
                                                                                if (!::g_bShow[::g_nResetHudId])
                                                                                {
                                                                                    (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                    {
                                                                                        (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                        (*::g_engfuncs.pfnWriteString) (::g_strWelcome_1.c_str());
                                                                                    }
                                                                                    (*::g_engfuncs.pfnMessageEnd) ();

                                                                                    if (!::g_strWelcome_2.empty())
                                                                                    {
                                                                                        if (::g_strWelcome_2.length() <= 190)
                                                                                        {
                                                                                            if (::g_strWelcome_2.size() <= 190)
                                                                                            {
                                                                                                (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                                {
                                                                                                    (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                                    (*::g_engfuncs.pfnWriteString) (::g_strWelcome_2.c_str());
                                                                                                }
                                                                                                (*::g_engfuncs.pfnMessageEnd) ();
                                                                                            }
                                                                                        }
                                                                                    }

                                                                                    if (!::g_strWelcome_3.empty())
                                                                                    {
                                                                                        if (::g_strWelcome_3.length() <= 190)
                                                                                        {
                                                                                            if (::g_strWelcome_3.size() <= 190)
                                                                                            {
                                                                                                (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                                {
                                                                                                    (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                                    (*::g_engfuncs.pfnWriteString) (::g_strWelcome_3.c_str());
                                                                                                }
                                                                                                (*::g_engfuncs.pfnMessageEnd) ();
                                                                                            }
                                                                                        }
                                                                                    }

                                                                                    if (!::g_strWelcome_4.empty())
                                                                                    {
                                                                                        if (::g_strWelcome_4.length() <= 190)
                                                                                        {
                                                                                            if (::g_strWelcome_4.size() <= 190)
                                                                                            {
                                                                                                (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                                {
                                                                                                    (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                                    (*::g_engfuncs.pfnWriteString) (::g_strWelcome_4.c_str());
                                                                                                }
                                                                                                (*::g_engfuncs.pfnMessageEnd) ();
                                                                                            }
                                                                                        }
                                                                                    }

                                                                                    if (!::g_strWelcome_5.empty())
                                                                                    {
                                                                                        if (::g_strWelcome_5.length() <= 190)
                                                                                        {
                                                                                            if (::g_strWelcome_5.size() <= 190)
                                                                                            {
                                                                                                (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                                {
                                                                                                    (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                                    (*::g_engfuncs.pfnWriteString) (::g_strWelcome_5.c_str());
                                                                                                }
                                                                                                (*::g_engfuncs.pfnMessageEnd) ();
                                                                                            }
                                                                                        }
                                                                                    }

                                                                                    if (!::g_strWelcome_6.empty())
                                                                                    {
                                                                                        if (::g_strWelcome_6.length() <= 190)
                                                                                        {
                                                                                            if (::g_strWelcome_6.size() <= 190)
                                                                                            {
                                                                                                (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                                {
                                                                                                    (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                                    (*::g_engfuncs.pfnWriteString) (::g_strWelcome_6.c_str());
                                                                                                }
                                                                                                (*::g_engfuncs.pfnMessageEnd) ();
                                                                                            }
                                                                                        }
                                                                                    }

                                                                                    if (!::g_strNotify_1.empty())
                                                                                    {
                                                                                        if (::g_strNotify_1.length() <= 190)
                                                                                        {
                                                                                            if (::g_strNotify_1.size() <= 190)
                                                                                            {
                                                                                                (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                                {
                                                                                                    (*::g_engfuncs.pfnWriteByte) (HUD_PRINTTALK);

                                                                                                    (*::g_engfuncs.pfnWriteString) (::g_strNotify_1.c_str());
                                                                                                }
                                                                                                (*::g_engfuncs.pfnMessageEnd) ();
                                                                                            }
                                                                                        }
                                                                                    }

                                                                                    if (!::g_strNotify_2.empty())
                                                                                    {
                                                                                        if (::g_strNotify_2.length() <= 190)
                                                                                        {
                                                                                            if (::g_strNotify_2.size() <= 190)
                                                                                            {
                                                                                                (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                                {
                                                                                                    (*::g_engfuncs.pfnWriteByte) (HUD_PRINTTALK);

                                                                                                    (*::g_engfuncs.pfnWriteString) (::g_strNotify_2.c_str());
                                                                                                }
                                                                                                (*::g_engfuncs.pfnMessageEnd) ();
                                                                                            }
                                                                                        }
                                                                                    }

                                                                                    ::g_bShow[::g_nResetHudId] = true;
                                                                                }
                                                                            }

                                                                            else if (nTeam == 3)
                                                                            {
                                                                                if (!::g_bShow[::g_nResetHudId])
                                                                                {
                                                                                    (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                    {
                                                                                        (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                        (*::g_engfuncs.pfnWriteString) (::g_strWelcome_1.c_str());
                                                                                    }
                                                                                    (*::g_engfuncs.pfnMessageEnd) ();

                                                                                    if (!::g_strWelcome_2.empty())
                                                                                    {
                                                                                        if (::g_strWelcome_2.length() <= 190)
                                                                                        {
                                                                                            if (::g_strWelcome_2.size() <= 190)
                                                                                            {
                                                                                                (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                                {
                                                                                                    (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                                    (*::g_engfuncs.pfnWriteString) (::g_strWelcome_2.c_str());
                                                                                                }
                                                                                                (*::g_engfuncs.pfnMessageEnd) ();
                                                                                            }
                                                                                        }
                                                                                    }

                                                                                    if (!::g_strWelcome_3.empty())
                                                                                    {
                                                                                        if (::g_strWelcome_3.length() <= 190)
                                                                                        {
                                                                                            if (::g_strWelcome_3.size() <= 190)
                                                                                            {
                                                                                                (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                                {
                                                                                                    (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                                    (*::g_engfuncs.pfnWriteString) (::g_strWelcome_3.c_str());
                                                                                                }
                                                                                                (*::g_engfuncs.pfnMessageEnd) ();
                                                                                            }
                                                                                        }
                                                                                    }

                                                                                    if (!::g_strWelcome_4.empty())
                                                                                    {
                                                                                        if (::g_strWelcome_4.length() <= 190)
                                                                                        {
                                                                                            if (::g_strWelcome_4.size() <= 190)
                                                                                            {
                                                                                                (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                                {
                                                                                                    (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                                    (*::g_engfuncs.pfnWriteString) (::g_strWelcome_4.c_str());
                                                                                                }
                                                                                                (*::g_engfuncs.pfnMessageEnd) ();
                                                                                            }
                                                                                        }
                                                                                    }

                                                                                    if (!::g_strWelcome_5.empty())
                                                                                    {
                                                                                        if (::g_strWelcome_5.length() <= 190)
                                                                                        {
                                                                                            if (::g_strWelcome_5.size() <= 190)
                                                                                            {
                                                                                                (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                                {
                                                                                                    (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                                    (*::g_engfuncs.pfnWriteString) (::g_strWelcome_5.c_str());
                                                                                                }
                                                                                                (*::g_engfuncs.pfnMessageEnd) ();
                                                                                            }
                                                                                        }
                                                                                    }

                                                                                    if (!::g_strWelcome_6.empty())
                                                                                    {
                                                                                        if (::g_strWelcome_6.length() <= 190)
                                                                                        {
                                                                                            if (::g_strWelcome_6.size() <= 190)
                                                                                            {
                                                                                                (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                                {
                                                                                                    (*::g_engfuncs.pfnWriteByte) (HUD_PRINTCONSOLE);

                                                                                                    (*::g_engfuncs.pfnWriteString) (::g_strWelcome_6.c_str());
                                                                                                }
                                                                                                (*::g_engfuncs.pfnMessageEnd) ();
                                                                                            }
                                                                                        }
                                                                                    }

                                                                                    if (!::g_strNotify_1.empty())
                                                                                    {
                                                                                        if (::g_strNotify_1.length() <= 190)
                                                                                        {
                                                                                            if (::g_strNotify_1.size() <= 190)
                                                                                            {
                                                                                                (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                                {
                                                                                                    (*::g_engfuncs.pfnWriteByte) (HUD_PRINTTALK);

                                                                                                    (*::g_engfuncs.pfnWriteString) (::g_strNotify_1.c_str());
                                                                                                }
                                                                                                (*::g_engfuncs.pfnMessageEnd) ();
                                                                                            }
                                                                                        }
                                                                                    }

                                                                                    if (!::g_strNotify_2.empty())
                                                                                    {
                                                                                        if (::g_strNotify_2.length() <= 190)
                                                                                        {
                                                                                            if (::g_strNotify_2.size() <= 190)
                                                                                            {
                                                                                                (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nTextMsg, nullptr, pEntity);
                                                                                                {
                                                                                                    (*::g_engfuncs.pfnWriteByte) (HUD_PRINTTALK);

                                                                                                    (*::g_engfuncs.pfnWriteString) (::g_strNotify_2.c_str());
                                                                                                }
                                                                                                (*::g_engfuncs.pfnMessageEnd) ();
                                                                                            }
                                                                                        }
                                                                                    }

                                                                                    ::g_bShow[::g_nResetHudId] = true;
                                                                                }
                                                                            }

                                                                            else if (!nTeam)
                                                                            {

                                                                            }

                                                                            else if (nTeam < 0)
                                                                            {

                                                                            }

                                                                            else
                                                                            {

                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }

                                                        bAltered = { };

                                                        if (::g_bFixScore)
                                                        {
                                                            if (pEntity->v.frags < 0)
                                                            {
                                                                bAltered = true;
                                                                {
                                                                    pEntity->v.frags = { };
                                                                }
                                                            }

                                                            if (pEntity->v.frags < 1)
                                                            {
                                                                if (!::g_strGameDir.compare("dod"))
                                                                {
                                                                    if (*((int*)pEntity->pvPrivateData + ::m_iDodDeaths + ::g_nOsOffs) > 0)
                                                                    {
                                                                        *((int*)pEntity->pvPrivateData + ::m_iDodDeaths + ::g_nOsOffs) = { };
                                                                        {
                                                                            bAltered = true;
                                                                        }
                                                                    }
                                                                }

                                                                else
                                                                {
                                                                    if (*((int*)pEntity->pvPrivateData + ::m_iDeaths + ::g_nOsOffs) > 0)
                                                                    {
                                                                        *((int*)pEntity->pvPrivateData + ::m_iDeaths + ::g_nOsOffs) = { };
                                                                        {
                                                                            bAltered = true;
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }

                                                        else if (::g_bFixKills)
                                                        {
                                                            if (pEntity->v.frags < 0)
                                                            {
                                                                bAltered = true;
                                                                {
                                                                    pEntity->v.frags = { };
                                                                }
                                                            }
                                                        }

                                                        else if (::g_bFixDeaths)
                                                        {
                                                            if (pEntity->v.frags < 1)
                                                            {
                                                                if (!::g_strGameDir.compare("dod"))
                                                                {
                                                                    if (*((int*)pEntity->pvPrivateData + ::m_iDodDeaths + ::g_nOsOffs) > 0)
                                                                    {
                                                                        *((int*)pEntity->pvPrivateData + ::m_iDodDeaths + ::g_nOsOffs) = { };
                                                                        {
                                                                            bAltered = true;
                                                                        }
                                                                    }
                                                                }

                                                                else
                                                                {
                                                                    if (*((int*)pEntity->pvPrivateData + ::m_iDeaths + ::g_nOsOffs) > 0)
                                                                    {
                                                                        *((int*)pEntity->pvPrivateData + ::m_iDeaths + ::g_nOsOffs) = { };
                                                                        {
                                                                            bAltered = true;
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }

                                                        if (::g_bZeroMoney)
                                                        {
                                                            if (*((int*)pEntity->pvPrivateData + ::m_iMoney + ::g_nOsOffs))
                                                            {
                                                                *((int*)pEntity->pvPrivateData + ::m_iMoney + ::g_nOsOffs) = { };
                                                            }
                                                        }

                                                        if (::g_bRealScore)
                                                        {
                                                            if (::g_nKills[::g_nResetHudId] || ::g_nDeaths[::g_nResetHudId])
                                                            {
                                                                if (!pEntity->v.frags)
                                                                {
                                                                    if (!::g_strGameDir.compare("dod"))
                                                                    {
                                                                        if (!(*((int*)pEntity->pvPrivateData + ::m_iDodDeaths + ::g_nOsOffs)))
                                                                        {
                                                                            ::g_nKills[::g_nResetHudId] = { };
                                                                            {
                                                                                ::g_nDeaths[::g_nResetHudId] = { };
                                                                            }
                                                                        }
                                                                    }

                                                                    else
                                                                    {
                                                                        if (!(*((int*)pEntity->pvPrivateData + ::m_iDeaths + ::g_nOsOffs)))
                                                                        {
                                                                            ::g_nKills[::g_nResetHudId] = { };
                                                                            {
                                                                                ::g_nDeaths[::g_nResetHudId] = { };
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }

                                                            if (pEntity->v.frags != ::g_nKills[::g_nResetHudId])
                                                            {
                                                                bAltered = true;
                                                                {
                                                                    pEntity->v.frags = ::g_nKills[::g_nResetHudId];
                                                                }
                                                            }

                                                            if (!::g_strGameDir.compare("dod"))
                                                            {
                                                                if (*((int*)pEntity->pvPrivateData + ::m_iDodDeaths + ::g_nOsOffs) != ::g_nDeaths[::g_nResetHudId])
                                                                {
                                                                    *((int*)pEntity->pvPrivateData + ::m_iDodDeaths + ::g_nOsOffs) = ::g_nDeaths[::g_nResetHudId];
                                                                    {
                                                                        bAltered = true;
                                                                    }
                                                                }
                                                            }

                                                            else
                                                            {
                                                                if (*((int*)pEntity->pvPrivateData + ::m_iDeaths + ::g_nOsOffs) != ::g_nDeaths[::g_nResetHudId])
                                                                {
                                                                    *((int*)pEntity->pvPrivateData + ::m_iDeaths + ::g_nOsOffs) = ::g_nDeaths[::g_nResetHudId];
                                                                    {
                                                                        bAltered = true;
                                                                    }
                                                                }
                                                            }
                                                        }

                                                        if (::g_bFixScore || ::g_bFixKills || ::g_bFixDeaths || ::g_bRealScore)
                                                        {
                                                            if (bAltered)
                                                            {
                                                                if (!::g_strGameDir.compare("dod"))
                                                                {
                                                                    if (::g_nScoreShort > 0)
                                                                    {
                                                                        (*::g_engfuncs.pfnMessageBegin) (::g_bAllReliable ? MSG_ALL : MSG_BROADCAST, ::g_nScoreShort, nullptr, nullptr);
                                                                        {
                                                                            (*::g_engfuncs.pfnWriteByte) (::g_nResetHudId);
                                                                            {
                                                                                (*::g_engfuncs.pfnWriteShort) (*((int*)pEntity->pvPrivateData + ::m_iDodScore + ::g_nOsOffs));
                                                                                {
                                                                                    (*::g_engfuncs.pfnWriteShort) (pEntity->v.frags);
                                                                                }
                                                                                (*::g_engfuncs.pfnWriteShort) (*((int*)pEntity->pvPrivateData + ::m_iDodDeaths + ::g_nOsOffs));
                                                                            }
                                                                            (*::g_engfuncs.pfnWriteByte) (xTo(true, int));
                                                                        }
                                                                        (*::g_engfuncs.pfnMessageEnd) ();
                                                                    }
                                                                }

                                                                else
                                                                {
                                                                    if (::g_nScoreInfo > 0)
                                                                    {
                                                                        (*::g_engfuncs.pfnMessageBegin) (::g_bAllReliable ? MSG_ALL : MSG_BROADCAST, ::g_nScoreInfo, nullptr, nullptr);
                                                                        {
                                                                            (*::g_engfuncs.pfnWriteByte) (::g_nResetHudId);
                                                                            {
                                                                                (*::g_engfuncs.pfnWriteShort) (pEntity->v.frags);
                                                                                (*::g_engfuncs.pfnWriteShort) (*((int*)pEntity->pvPrivateData + ::m_iDeaths + ::g_nOsOffs));
                                                                                (*::g_engfuncs.pfnWriteShort) ({ });
                                                                                (*::g_engfuncs.pfnWriteShort) (*((int*)pEntity->pvPrivateData + ::m_iTeam + ::g_nOsOffs));
                                                                            }
                                                                        }
                                                                        (*::g_engfuncs.pfnMessageEnd) ();
                                                                    }
                                                                }
                                                            }
                                                        }

                                                        if (::g_bHideRadar)
                                                        {
                                                            if (!::g_bRadar[::g_nResetHudId])
                                                            {
                                                                (*::g_engfuncs.pfnClientCommand) (pEntity, "hideradar\n");
                                                                {
                                                                    ::g_bRadar[::g_nResetHudId] = true;
                                                                }
                                                            }
                                                        }

                                                        if (::g_bHideMoney || ::g_bHideTimer || ::g_bHideFlash)
                                                        {
                                                            if (::g_nHideWeapon > 0)
                                                            {
                                                                if (::g_nCrosshair > 0)
                                                                {
                                                                    if ((nFov = *((int*)pEntity->pvPrivateData + ::m_iFov + ::g_nOsOffs)) != 40)
                                                                    {
                                                                        if (nFov != 10)
                                                                        {
                                                                            if (nFov != 15)
                                                                            {
                                                                                if (pEntity->v.fov != 40.f)
                                                                                {
                                                                                    if (pEntity->v.fov != 10.f)
                                                                                    {
                                                                                        if (pEntity->v.fov != 15.f)
                                                                                        {
                                                                                            if (!::g_bHide[::g_nResetHudId])
                                                                                            {
                                                                                                (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nHideWeapon, nullptr, pEntity);
                                                                                                {
                                                                                                    if (::g_bHideMoney)
                                                                                                    {
                                                                                                        if (::g_bHideTimer)
                                                                                                        {
                                                                                                            if (::g_bHideFlash)
                                                                                                            {
                                                                                                                (*::g_engfuncs.pfnWriteByte) (50);
                                                                                                            }

                                                                                                            else
                                                                                                            {
                                                                                                                (*::g_engfuncs.pfnWriteByte) (48);
                                                                                                            }
                                                                                                        }

                                                                                                        else
                                                                                                        {
                                                                                                            if (::g_bHideFlash)
                                                                                                            {
                                                                                                                (*::g_engfuncs.pfnWriteByte) (34);
                                                                                                            }

                                                                                                            else
                                                                                                            {
                                                                                                                (*::g_engfuncs.pfnWriteByte) (32);
                                                                                                            }
                                                                                                        }
                                                                                                    }

                                                                                                    else if (::g_bHideTimer)
                                                                                                    {
                                                                                                        if (::g_bHideFlash)
                                                                                                        {
                                                                                                            (*::g_engfuncs.pfnWriteByte) (18);
                                                                                                        }

                                                                                                        else
                                                                                                        {
                                                                                                            (*::g_engfuncs.pfnWriteByte) (16);
                                                                                                        }
                                                                                                    }

                                                                                                    else
                                                                                                    {
                                                                                                        (*::g_engfuncs.pfnWriteByte) (2);
                                                                                                    }
                                                                                                }
                                                                                                (*::g_engfuncs.pfnMessageEnd) ();

                                                                                                (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nCrosshair, nullptr, pEntity);
                                                                                                {
                                                                                                    (*::g_engfuncs.pfnWriteByte) ({ });
                                                                                                }
                                                                                                (*::g_engfuncs.pfnMessageEnd) ();

                                                                                                ::g_bHide[::g_nResetHudId] = true;
                                                                                            }
                                                                                        }
                                                                                    }
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }

                                                        if (::g_nArmor)
                                                        {
                                                            if (pEntity->v.armorvalue < 100.f)
                                                            {
                                                                if (!::g_bArmor[::g_nResetHudId])
                                                                {
                                                                    if (!::g_bKeepHelmet)
                                                                    {
                                                                        pEntity->v.armorvalue = 100.f;
                                                                        {
                                                                            if (*((int*)pEntity->pvPrivateData + ::m_iKevlar + ::g_nOsOffs) != ::g_nArmor)
                                                                            {
                                                                                *((int*)pEntity->pvPrivateData + ::m_iKevlar + ::g_nOsOffs) = ::g_nArmor;
                                                                            }
                                                                        }

                                                                        if (::g_nArmorType > 0)
                                                                        {
                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nArmorType, nullptr, pEntity);
                                                                            {
                                                                                (*::g_engfuncs.pfnWriteByte) (::g_nArmor - 1);
                                                                            }
                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                        }
                                                                    }

                                                                    else if (::g_nArmor > 1)
                                                                    {
                                                                        pEntity->v.armorvalue = 100.f;
                                                                        {
                                                                            if (*((int*)pEntity->pvPrivateData + ::m_iKevlar + ::g_nOsOffs) != ::g_nArmor)
                                                                            {
                                                                                *((int*)pEntity->pvPrivateData + ::m_iKevlar + ::g_nOsOffs) = ::g_nArmor;
                                                                            }
                                                                        }

                                                                        if (::g_nArmorType > 0)
                                                                        {
                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nArmorType, nullptr, pEntity);
                                                                            {
                                                                                (*::g_engfuncs.pfnWriteByte) (::g_nArmor - 1);
                                                                            }
                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                        }
                                                                    }

                                                                    else
                                                                    {
                                                                        pEntity->v.armorvalue = 100.f;
                                                                        {
                                                                            if ((nKevlar = *((int*)pEntity->pvPrivateData + ::m_iKevlar + ::g_nOsOffs)) < 2)
                                                                            {
                                                                                if (nKevlar != ::g_nArmor)
                                                                                {
                                                                                    *((int*)pEntity->pvPrivateData + ::m_iKevlar + ::g_nOsOffs) = ::g_nArmor;
                                                                                }
                                                                            }
                                                                        }

                                                                        if (::g_nArmorType > 0)
                                                                        {
                                                                            (*::g_engfuncs.pfnMessageBegin) (::g_bOneReliable ? MSG_ONE : MSG_ONE_UNRELIABLE, ::g_nArmorType, nullptr, pEntity);
                                                                            {
                                                                                if (*((int*)pEntity->pvPrivateData + ::m_iKevlar + ::g_nOsOffs) < 2)
                                                                                {
                                                                                    (*::g_engfuncs.pfnWriteByte) (::g_nArmor - 1);
                                                                                }

                                                                                else
                                                                                {
                                                                                    (*::g_engfuncs.pfnWriteByte) (xTo(true, int));
                                                                                }
                                                                            }
                                                                            (*::g_engfuncs.pfnMessageEnd) ();
                                                                        }
                                                                    }

                                                                    ::g_bArmor[::g_nResetHudId] = true;
                                                                }
                                                            }

                                                            else
                                                            {
                                                                ::g_bArmor[::g_nResetHudId] = true;
                                                            }
                                                        }

                                                        else
                                                        {
                                                            ::g_bArmor[::g_nResetHudId] = { };
                                                        }
                                                    }
                                                }
                                            }

                                            else
                                            {
                                                if (::g_nArmor)
                                                {
                                                    ::g_bArmor[::g_nResetHudId] = { };
                                                }

                                                if (::g_bHideFlash || ::g_bHideTimer || ::g_bHideMoney)
                                                {
                                                    if (::g_nHideWeapon)
                                                    {
                                                        if (::g_nCrosshair)
                                                        {
                                                            ::g_bHide[::g_nResetHudId] = { };
                                                        }
                                                    }
                                                }

                                                if (::g_bHideRadar)
                                                {
                                                    ::g_bRadar[::g_nResetHudId] = { };
                                                }
                                            }
                                        }
                                    }

                                    else
                                    {
                                        if (pEntity->v.health <= 0.f)
                                        {
                                            if (::g_nArmor)
                                            {
                                                ::g_bArmor[::g_nResetHudId] = { };
                                            }

                                            if (::g_bHideFlash || ::g_bHideTimer || ::g_bHideMoney)
                                            {
                                                if (::g_nHideWeapon)
                                                {
                                                    if (::g_nCrosshair)
                                                    {
                                                        ::g_bHide[::g_nResetHudId] = { };
                                                    }
                                                }
                                            }

                                            if (::g_bHideRadar)
                                            {
                                                ::g_bRadar[::g_nResetHudId] = { };
                                            }
                                        }

                                        else
                                        {

                                        }
                                    }
                                }

                                else
                                {
                                    if (::g_nArmor)
                                    {
                                        ::g_bArmor[::g_nResetHudId] = { };
                                    }

                                    if (::g_bHideFlash || ::g_bHideTimer || ::g_bHideMoney)
                                    {
                                        if (::g_nHideWeapon)
                                        {
                                            if (::g_nCrosshair)
                                            {
                                                ::g_bHide[::g_nResetHudId] = { };
                                            }
                                        }
                                    }

                                    if (::g_bHideRadar)
                                    {
                                        ::g_bRadar[::g_nResetHudId] = { };
                                    }
                                }
                            }

                            else
                            {
                                if (::g_nArmor)
                                {
                                    ::g_bArmor[::g_nResetHudId] = { };
                                }

                                if (::g_bHideFlash || ::g_bHideTimer || ::g_bHideMoney)
                                {
                                    if (::g_nHideWeapon)
                                    {
                                        if (::g_nCrosshair)
                                        {
                                            ::g_bHide[::g_nResetHudId] = { };
                                        }
                                    }
                                }

                                if (::g_bHideRadar)
                                {
                                    ::g_bRadar[::g_nResetHudId] = { };
                                }
                            }
                        }

                        else
                        {
                            if (::g_nArmor)
                            {
                                ::g_bArmor[::g_nResetHudId] = { };
                            }

                            if (::g_bHideFlash || ::g_bHideTimer || ::g_bHideMoney)
                            {
                                if (::g_nHideWeapon)
                                {
                                    if (::g_nCrosshair)
                                    {
                                        ::g_bHide[::g_nResetHudId] = { };
                                    }
                                }
                            }

                            if (::g_bHideRadar)
                            {
                                ::g_bRadar[::g_nResetHudId] = { };
                            }
                        }
                    }

                    else
                    {
                        if (::g_nArmor)
                        {
                            ::g_bArmor[::g_nResetHudId] = { };
                        }

                        if (::g_bHideFlash || ::g_bHideTimer || ::g_bHideMoney)
                        {
                            if (::g_nHideWeapon)
                            {
                                if (::g_nCrosshair)
                                {
                                    ::g_bHide[::g_nResetHudId] = { };
                                }
                            }
                        }

                        if (::g_bHideRadar)
                        {
                            ::g_bRadar[::g_nResetHudId] = { };
                        }
                    }
                }

                else
                {
                    if (::g_nArmor)
                    {
                        ::g_bArmor[::g_nResetHudId] = { };
                    }

                    if (::g_bHideFlash || ::g_bHideTimer || ::g_bHideMoney)
                    {
                        if (::g_nHideWeapon)
                        {
                            if (::g_nCrosshair)
                            {
                                ::g_bHide[::g_nResetHudId] = { };
                            }
                        }
                    }

                    if (::g_bHideRadar)
                    {
                        ::g_bRadar[::g_nResetHudId] = { };
                    }
                }
            }

            ::g_bResetHud = { };
            {
                ::g_nResetHudId = { };
            }
        }

        else
        {
            if (::g_nDeathMsg > 0)
            {
                if (::g_bDeathMsg)
                {
                    if (::g_nVictim > 0)
                    {
                        if (::g_nKiller > 0)
                        {
                            if (::g_bRealScore)
                            {
                                if (::g_nVictim != ::g_nKiller)
                                {
                                    if (!bConVars)
                                    {
                                        pFriendlyFire = (*::g_engfuncs.pfnCVarGetPointer) ("mp_friendlyfire");
                                        {
                                            bConVars = true;
                                        }
                                    }

                                    if (!pFriendlyFire || !pFriendlyFire->value)
                                    {
                                        if (pEntity = ::INDEXENT(::g_nVictim))
                                        {
                                            if (!pEntity->free)
                                            {
                                                if (pEntity->pvPrivateData)
                                                {
                                                    if (!pEntity->serialnumber)
                                                    {
                                                        if (!(pEntity->v.flags & FL_DORMANT))
                                                        {
                                                            if (pEntity->v.flags & FL_CLIENT)
                                                            {
                                                                ::g_nDeaths[::g_nVictim] ++;
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }

                                        if (pEntity = ::INDEXENT(::g_nKiller))
                                        {
                                            if (!pEntity->free)
                                            {
                                                if (pEntity->pvPrivateData)
                                                {
                                                    if (!pEntity->serialnumber)
                                                    {
                                                        if (!(pEntity->v.flags & FL_DORMANT))
                                                        {
                                                            if (pEntity->v.flags & FL_CLIENT)
                                                            {
                                                                ::g_nKills[::g_nKiller] ++;
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }

                                    else
                                    {
                                        pVictim = nullptr;
                                        {
                                            bSafe = { };
                                        }

                                        if (pEntity = ::INDEXENT(::g_nVictim))
                                        {
                                            if (!pEntity->free)
                                            {
                                                if (pEntity->pvPrivateData)
                                                {
                                                    if (!pEntity->serialnumber)
                                                    {
                                                        if (!(pEntity->v.flags & FL_DORMANT))
                                                        {
                                                            if (pEntity->v.flags & FL_CLIENT)
                                                            {
                                                                ::g_nDeaths[::g_nVictim] ++;
                                                                {
                                                                    pVictim = pEntity;
                                                                    {
                                                                        bSafe = true;
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }

                                        if (pEntity = ::INDEXENT(::g_nKiller))
                                        {
                                            if (!pEntity->free)
                                            {
                                                if (pEntity->pvPrivateData)
                                                {
                                                    if (!pEntity->serialnumber)
                                                    {
                                                        if (!(pEntity->v.flags & FL_DORMANT))
                                                        {
                                                            if (pEntity->v.flags & FL_CLIENT)
                                                            {
                                                                if (::g_bRealScoreFriendly)
                                                                {
                                                                    ::g_nKills[::g_nKiller] ++;
                                                                }

                                                                else
                                                                {
                                                                    if (bSafe)
                                                                    {
                                                                        if (pVictim)
                                                                        {
                                                                            if (!::g_strGameDir.compare("dod"))
                                                                            {
                                                                                if ((*((char*)pVictim->pvPrivateData + ::m_iDodTeam + ::g_nOsOffs) != *((char*)pEntity->pvPrivateData + ::m_iDodTeam + ::g_nOsOffs)) && (*((char*)pVictim->pvPrivateData + ::m_iDodTeam + ::g_nOsOffs + 1) != *((char*)pEntity->pvPrivateData + ::m_iDodTeam + ::g_nOsOffs + 1)))
                                                                                {
                                                                                    ::g_nKills[::g_nKiller] ++;
                                                                                }
                                                                            }

                                                                            else
                                                                            {
                                                                                if (*((int*)pVictim->pvPrivateData + ::m_iTeam + ::g_nOsOffs) != *((int*)pEntity->pvPrivateData + ::m_iTeam + ::g_nOsOffs))
                                                                                {
                                                                                    ::g_nKills[::g_nKiller] ++;
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }

                    ::g_bDeathMsg = { };
                    {
                        ::g_nDeathMsgByte = { };
                        {
                            ::g_nVictim = { };
                            {
                                ::g_nKiller = { };
                            }
                        }
                    }
                }
            }
        }
    }

    else
    {
        if (::g_nDeathMsg > 0)
        {
            if (::g_bDeathMsg)
            {
                if (::g_nVictim > 0)
                {
                    if (::g_nKiller > 0)
                    {
                        if (::g_bRealScore)
                        {
                            if (::g_nVictim != ::g_nKiller)
                            {
                                if (!bConVars)
                                {
                                    pFriendlyFire = (*::g_engfuncs.pfnCVarGetPointer) ("mp_friendlyfire");
                                    {
                                        bConVars = true;
                                    }
                                }

                                if (!pFriendlyFire || !pFriendlyFire->value)
                                {
                                    if (pEntity = ::INDEXENT(::g_nVictim))
                                    {
                                        if (!pEntity->free)
                                        {
                                            if (pEntity->pvPrivateData)
                                            {
                                                if (!pEntity->serialnumber)
                                                {
                                                    if (!(pEntity->v.flags & FL_DORMANT))
                                                    {
                                                        if (pEntity->v.flags & FL_CLIENT)
                                                        {
                                                            ::g_nDeaths[::g_nVictim] ++;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }

                                    if (pEntity = ::INDEXENT(::g_nKiller))
                                    {
                                        if (!pEntity->free)
                                        {
                                            if (pEntity->pvPrivateData)
                                            {
                                                if (!pEntity->serialnumber)
                                                {
                                                    if (!(pEntity->v.flags & FL_DORMANT))
                                                    {
                                                        if (pEntity->v.flags & FL_CLIENT)
                                                        {
                                                            ::g_nKills[::g_nKiller] ++;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }

                                else
                                {
                                    pVictim = nullptr;
                                    {
                                        bSafe = { };
                                    }

                                    if (pEntity = ::INDEXENT(::g_nVictim))
                                    {
                                        if (!pEntity->free)
                                        {
                                            if (pEntity->pvPrivateData)
                                            {
                                                if (!pEntity->serialnumber)
                                                {
                                                    if (!(pEntity->v.flags & FL_DORMANT))
                                                    {
                                                        if (pEntity->v.flags & FL_CLIENT)
                                                        {
                                                            ::g_nDeaths[::g_nVictim] ++;
                                                            {
                                                                pVictim = pEntity;
                                                                {
                                                                    bSafe = true;
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }

                                    if (pEntity = ::INDEXENT(::g_nKiller))
                                    {
                                        if (!pEntity->free)
                                        {
                                            if (pEntity->pvPrivateData)
                                            {
                                                if (!pEntity->serialnumber)
                                                {
                                                    if (!(pEntity->v.flags & FL_DORMANT))
                                                    {
                                                        if (pEntity->v.flags & FL_CLIENT)
                                                        {
                                                            if (::g_bRealScoreFriendly)
                                                            {
                                                                ::g_nKills[::g_nKiller] ++;
                                                            }

                                                            else
                                                            {
                                                                if (bSafe)
                                                                {
                                                                    if (pVictim)
                                                                    {
                                                                        if (!::g_strGameDir.compare("dod"))
                                                                        {
                                                                            if ((*((char*)pVictim->pvPrivateData + ::m_iDodTeam + ::g_nOsOffs) != *((char*)pEntity->pvPrivateData + ::m_iDodTeam + ::g_nOsOffs)) && (*((char*)pVictim->pvPrivateData + ::m_iDodTeam + ::g_nOsOffs + 1) != *((char*)pEntity->pvPrivateData + ::m_iDodTeam + ::g_nOsOffs + 1)))
                                                                            {
                                                                                ::g_nKills[::g_nKiller] ++;
                                                                            }
                                                                        }

                                                                        else
                                                                        {
                                                                            if (*((int*)pVictim->pvPrivateData + ::m_iTeam + ::g_nOsOffs) != *((int*)pEntity->pvPrivateData + ::m_iTeam + ::g_nOsOffs))
                                                                            {
                                                                                ::g_nKills[::g_nKiller] ++;
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                ::g_bDeathMsg = { };
                {
                    ::g_nDeathMsgByte = { };
                    {
                        ::g_nVictim = { };
                        {
                            ::g_nKiller = { };
                        }
                    }
                }
            }
        }
    }

    do
    {
        ::gpMetaGlobals->mres = ::MRES_IGNORED;

        return;
    }

    while (false);
}

bool FileExists(::std::string strFile) noexcept
{

#ifndef WIN32

    static ::FILE* pFile{ };

    if (strFile.empty())
    {
        return false;
    }

    else if (pFile = ::fopen(strFile.c_str(), "r"))
    {
        ::fclose(pFile);
        {
            return true;
        }
    }

    return false;

#else

    return ::std::filesystem::exists(strFile);

#endif

}

bool FileRemove(::std::string strFile) noexcept
{

#ifndef WIN32

    if (strFile.empty())
    {
        return false;
    }

    else if (!::unlink(strFile.c_str()))
    {
        return true;
    }

    return false;

#else

    return ::std::filesystem::remove(strFile);

#endif

}

bool FileCopy(::std::string strFrom, ::std::string strTo) noexcept
{

#ifndef WIN32

    static ::FILE* pFrom{ }, * pTo{ };

    static char szBuffer[512]{ };
    static unsigned int uBytes{ };

    if (strFrom.empty())
    {
        return false;
    }

    else if (strTo.empty())
    {
        return false;
    }

    else if (!(pFrom = ::fopen(strFrom.c_str(), "r")))
    {
        return false;
    }

    else if (!(pTo = ::fopen(strTo.c_str(), "w")))
    {
        ::fclose(pFrom);
        {
            return false;
        }
    }

    while ((uBytes = ::fread(szBuffer, sizeof(char), sizeof szBuffer, pFrom)) > 0)
    {
        if (::fwrite(szBuffer, sizeof(char), uBytes, pTo) != uBytes)
        {
            ::fclose(pFrom);
            {
                ::fclose(pTo);
                {
                    return false;
                }
            }
        }
    }

    ::fclose(pFrom);
    {
        ::fclose(pTo);
        {
            return true;
        }
    }

#else

    ::std::error_code _{ };
    {
        ::std::filesystem::copy(strFrom, strTo, _);
        {
            return !(_);
        }
    }

#endif

}

bool FileDirectory(::std::string strFile) noexcept
{

#ifdef WIN32

    return ::std::filesystem::is_directory(strFile);

#else

    static ::DIR* pDir{ };

    if (pDir = ::opendir(strFile.c_str()))
    {
        ::closedir(pDir);
        {
            return true;
        }
    }

    return false;

#endif

}

#ifdef WIN32

constexpr int ValClamp(int nVal, int nMin, int nMax) noexcept

#else

int ValClamp(int nVal, int nMin, int nMax) noexcept

#endif

{

#ifdef WIN32

    return ((::std::clamp)(nVal, nMin, nMax));

#else

    if (nVal < nMin)
    {
        return nMin;
    }

    else if (nVal > nMax)
    {
        return nMax;
    }

    return nVal;

#endif

}

#ifdef WIN32

constexpr float ValClamp(float fVal, float fMin, float fMax) noexcept

#else

float ValClamp(float fVal, float fMin, float fMax) noexcept

#endif

{

#ifdef WIN32

    return ((::std::clamp)(fVal, fMin, fMax));

#else

    if (fVal < fMin)
    {
        return fMin;
    }

    else if (fVal > fMax)
    {
        return fMax;
    }

    return fVal;

#endif

}

unsigned int ComputeSafeErasePos(char* pPtr) noexcept
{
    static unsigned int uNum{ }, uByteNum{ };

    if (pPtr)
    {
        for (uNum = xTo(true, unsigned int), uByteNum = { }; (*pPtr & 0xC0) == 0x80; uNum++)
        {
            pPtr--;
        }

        switch (*pPtr & 0xF0)
        {
        case 0xC0:
        case 0xD0:
        {
            uByteNum = xTo(2, unsigned int);

            break;
        }

        case 0xE0:
        {
            uByteNum = xTo(3, unsigned int);

            break;
        }

        case 0xF0:
        {
            uByteNum = xTo(4, unsigned int);

            break;
        }
        }

        if (uByteNum != uNum)
        {
            return uNum;
        }
    }

    return { };
}

unsigned int SafeErasePos(::std::string strIn, unsigned int uPos) noexcept
{
    if (strIn.empty())
    {
        return { };
    }

    else if (uPos > xTo(false, unsigned int))
    {
        if (strIn.at(uPos - xTo(true, unsigned int)) & 1 << 7)
        {
            return uPos - ::ComputeSafeErasePos(strIn.data() + uPos - xTo(true, unsigned int));
        }
    }

    return uPos >= xTo(false, unsigned int) ? uPos : xTo(false, unsigned int);
}

#ifndef WIN32

extern "C" void __cxa_pure_virtual()
{

}

void* operator new(unsigned int uMem)
{
    return ::malloc(uMem);
}

void* operator new [](unsigned int uMem)
{
    return ::malloc(uMem);
}

void operator delete(void* pMem)
{
    ::free(pMem);
}

void operator delete [](void* pMem)
{
    ::free(pMem);
}

#endif

#ifdef WIN32

bool g_bMultiMediaFeatureIsActive{ };
bool g_bMultiMediaFeatureShouldStopThreadWorker{ };

unsigned int g_uiMultiMediaFeatureTimerResolution{ };
unsigned int g_uiMultiMediaFeatureMinPeriodAvail{ };

void* g_pMultiMediaFeatureThreadWorkerHandle{ };

#endif

#ifdef WIN32

unsigned long __stdcall MultiMediaFeatureThreadWorkerFunction(void* pMultiMediaFeatureThreadWorkerData) noexcept
{
    static long long llTimeNow{ }, llTimeStamp{ };

    static ::mmtime_tag multiMediaTime{ };

    while (!::g_bMultiMediaFeatureShouldStopThreadWorker)
    {
        llTimeNow = ::std::time(nullptr);
        {
            if ((llTimeNow - llTimeStamp) > ((long long)(5I8)))
            {
                llTimeStamp = llTimeNow;
                {
                    if (!::timeGetSystemTime(&multiMediaTime, sizeof multiMediaTime))
                    {
                        ::timeGetTime();
                    }

                    else
                    {
                        ::timeGetTime();
                    }
                }
            }
        }

        ::Sleep(((unsigned long)(100I8)));
    }

    return { };
}

#endif

#ifdef WIN32

void MultiMediaFeatureTryAttach() noexcept
{
    ::timecaps_tag timeCaps{ };
    {
        ::mmtime_tag multiMediaTime{ };
        {
            if (!::timeGetDevCaps(&timeCaps, sizeof timeCaps))
            {
                ::g_uiMultiMediaFeatureMinPeriodAvail = ((::std::max)(((unsigned int)(timeCaps.wPeriodMin)), ((unsigned int)(1I8))));
                {
                    ::g_uiMultiMediaFeatureTimerResolution = ((::std::min)(((unsigned int)(::g_uiMultiMediaFeatureMinPeriodAvail)), ((unsigned int)(timeCaps.wPeriodMax))));
                    {
                        if (!::timeBeginPeriod(::g_uiMultiMediaFeatureTimerResolution))
                        {
                            ::g_bMultiMediaFeatureIsActive = true;
                            {
                                ::std::printf("[INFO] `::timeBeginPeriod(%d);` OK\n", ((int)(::g_uiMultiMediaFeatureTimerResolution)));
                                {
                                    if (!::timeGetSystemTime(&multiMediaTime, sizeof multiMediaTime))
                                    {
                                        ::timeGetTime();
                                        {
                                            ::std::printf("[INFO] `::timeGetSystemTime(...);` OK & `::timeGetTime();` OK\n");
                                            {
                                                ::g_pMultiMediaFeatureThreadWorkerHandle = ::CreateThread(nullptr, { }, ::MultiMediaFeatureThreadWorkerFunction, nullptr, { }, nullptr);
                                                {
                                                    if (::g_pMultiMediaFeatureThreadWorkerHandle)
                                                    {
                                                        ::std::printf("[INFO] `::MultiMediaFeatureThreadWorkerFunction(...);` OK\n");
                                                    }

                                                    else
                                                    {
                                                        ::std::printf("[WARN] `::MultiMediaFeatureThreadWorkerFunction(...);` FAILED\n");
                                                    }
                                                }
                                            }
                                        }
                                    }

                                    else
                                    {
                                        ::timeGetTime();
                                        {
                                            ::std::printf("[WARN] `::timeGetSystemTime(...);` FAILED & `::timeGetTime();` OK\n");
                                            {
                                                ::g_pMultiMediaFeatureThreadWorkerHandle = ::CreateThread(nullptr, { }, ::MultiMediaFeatureThreadWorkerFunction, nullptr, { }, nullptr);
                                                {
                                                    if (::g_pMultiMediaFeatureThreadWorkerHandle)
                                                    {
                                                        ::std::printf("[INFO] `::MultiMediaFeatureThreadWorkerFunction(...);` OK\n");
                                                    }

                                                    else
                                                    {
                                                        ::std::printf("[WARN] `::MultiMediaFeatureThreadWorkerFunction(...);` FAILED\n");
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }

                        else
                        {
                            ::std::printf("[WARN] `::timeBeginPeriod(%d);` FAILED\n", ((int)(::g_uiMultiMediaFeatureTimerResolution)));
                        }
                    }
                }
            }

            else
            {
                ::std::printf("[WARN] `::timeGetDevCaps(...);` FAILED\n");
            }
        }
    }
}

#endif

#ifdef WIN32

void MultiMediaFeatureTryDetach() noexcept
{
    if (::g_bMultiMediaFeatureIsActive)
    {
        if (::g_pMultiMediaFeatureThreadWorkerHandle)
        {
            ::g_bMultiMediaFeatureShouldStopThreadWorker = true;
            {
                ::WaitForSingleObject(::g_pMultiMediaFeatureThreadWorkerHandle, 750UL);
                {
                    ::CloseHandle(::g_pMultiMediaFeatureThreadWorkerHandle);
                    {
                        ::g_pMultiMediaFeatureThreadWorkerHandle = nullptr;
                        {
                            ::g_bMultiMediaFeatureShouldStopThreadWorker = { };
                            {
                                ::std::printf("[INFO] `::MultiMediaFeatureThreadWorkerFunction(...);` CLOSED (OK)\n");
                            }
                        }
                    }
                }
            }
        }

        if (!::timeEndPeriod(::g_uiMultiMediaFeatureTimerResolution))
        {
            ::g_bMultiMediaFeatureIsActive = { };
            {
                ::std::printf("[INFO] `::timeEndPeriod(%d);` OK\n", ((int)(::g_uiMultiMediaFeatureTimerResolution)));
            }
        }

        else
        {
            ::g_bMultiMediaFeatureIsActive = { };
            {
                ::std::printf("[WARN] `::timeEndPeriod(%d);` FAILED\n", ((int)(::g_uiMultiMediaFeatureTimerResolution)));
            }
        }
    }
}

#endif
