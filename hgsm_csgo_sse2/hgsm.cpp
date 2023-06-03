
#include "hgsm.h"

SH_DECL_HOOK0(IServerGameDLL, GetTickInterval, const, false, float);
SH_DECL_HOOK3_void(IServerGameDLL, ServerActivate, SH_NOATTRIB, false, ::edict_t*, int, int);
SH_DECL_HOOK6(IServerGameDLL, LevelInit, SH_NOATTRIB, false, bool, const char*, const char*, const char*, const char*, bool, bool);
SH_DECL_HOOK0_void(IServerGameDLL, LevelShutdown, SH_NOATTRIB, false);
SH_DECL_HOOK1_void(IServerGameDLL, GameFrame, SH_NOATTRIB, false, bool);

SH_DECL_HOOK5(IServerGameClients, ClientConnect, SH_NOATTRIB, false, bool, ::edict_t*, const char*, const char*, char*, int);
SH_DECL_HOOK2_void(IServerGameClients, ClientPutInServer, SH_NOATTRIB, false, ::edict_t*, const char*);
SH_DECL_HOOK1_void(IServerGameClients, ClientDisconnect, SH_NOATTRIB, false, ::edict_t*);

::ICvar* icvar{ };

::IVEngineServer* engine{ };
::IGameEventManager2* gameevents{ };
::IServerPluginHelpers* helpers{ };

::IServerTools* servertools{ };
::IPlayerInfoManager* playerinfomanager{ };
::IServerGameClients* gameclients{ };
::IServerGameDLL* server{ };

::IFileSystem* filesystem{ };
::IBaseFileSystem* basefilesystem{ };

::CGlobalVars* gpGlobals{ };

::IServerPluginCallbacks* vsp_callbacks{ };

::CSharedEdictChangeInfo* g_pSharedChangeInfo{ };

::ISmmAPI* g_SMAPI{ };

::SourceHook::ISourceHook* g_SHPtr{ };

::ISmmPlugin* g_PLAPI{ };

::PluginId g_PLID{ };

::MySmmPlugin g_MySmmPlugin{ };

::ConVar g_cvVersion("hgsm_version", __DATE__, FCVAR_NOTIFY | FCVAR_SPONLY, xName " Version", { }, { }, { }, { });

::InterfaceReg __g_CreateISmmPluginISmmPlugin_reg(::__CreateISmmPluginISmmPlugin_interface, "ISmmPlugin");

::std::string g_strGameDir{ };

::std::string g_strWelcome_1{ };
::std::string g_strWelcome_2{ };
::std::string g_strWelcome_3{ };
::std::string g_strWelcome_4{ };
::std::string g_strWelcome_5{ };
::std::string g_strWelcome_6{ };

::std::string g_strNotify_1{ };
::std::string g_strNotify_2{ };

#if SOURCE_ENGINE == SE_CSGO

const ::google::protobuf::Message* g_pTextMsg{ };

#endif

#ifdef WIN32

::HWND__* g_pWnd{ };

#endif

bool g_bShow[80]{ };

bool g_bArmor[80]{ };

bool g_bHooked{ };
bool g_bEvHooked{ };
bool g_bNoReturn{ };

bool g_bRealScore{ };
bool g_bHideRadar{ };
bool g_bZeroMoney{ };
bool g_bRealScoreFriendly{ };
bool g_bFriendlyDecrease{ };
bool g_bFixScore{ };
bool g_bFixKills{ };
bool g_bFixDeaths{ };
bool g_bKeepHelmet{ };
bool g_bReliable{ };
bool g_bHourlyRestart{ };

int g_nDeaths[80]{ };
int g_nKills[80]{ };

int g_nTextMsg{ };

int g_nArmor{ };

int g_nTicks{ 50, };

float g_fIntervalPerTick{ };

int m_iDeaths{ }, m_iDeathsSize{ };
int m_iFrags{ }, m_iFragsSize{ };

int m_ArmorValue{ }, m_ArmorValueSize{ };

int m_bHasHelmet{ }, m_bHasHelmetSize{ };

int m_iAccount{ }, m_iAccountSize{ };

int m_flFlashMaxAlpha{ };
int m_flFlashDuration{ };

#ifdef WIN32

int x_nDataMap{ 11, };

#else

int x_nDataMap{ 12, };

#endif

int x_nSendProp{ 16, };

long long g_llMapChange{ };

#ifdef WIN32

unsigned long g_ulProc{ };

#endif

bool ::MySmmPlugin::Load(::PluginId pluginId, ::ISmmAPI* pSmmApi, char* pszError, unsigned int uMaxLen, bool bLateLoaded) noexcept
{
    static int nPlayer{ };
    static const char* pszGameDir{ };

    static ::edict_t* pEdict{ };
    static ::IPlayerInfo* pPlayerInfo{ };
    static ::CBaseEntity* pBaseEntity{ };
    static ::IServerEntity* pServerEntity{ };
    static ::IServerUnknown* pServerUnknown{ };
    static ::CreateInterfaceFn ifEngine{ }, ifServer{ }, ifFileSystem{ };

#ifdef WIN32

    static unsigned long ulProc{ };

    static ::HWND__* pWnd{ };

#endif

    if (!pSmmApi)
    {
        ::std::printf("'ISmmAPI' Failed\n");

        return false;
    }

    else
    {
        ::g_SMAPI = pSmmApi;
    }

    if (!(::g_SHPtr = static_cast <::SourceHook::ISourceHook*> (pSmmApi->MetaFactory("ISourceHook", { }, { }))))
    {
        ::std::printf("'ISourceHook' Failed\n");
        {
            if (pszError)
            {
                if (uMaxLen > 0)
                {
                    pSmmApi->Format(pszError, uMaxLen, "'ISourceHook' Failed");
                }
            }

            return false;
        }
    }

    if (!(::g_PLAPI = this))
    {
        ::std::printf("'ISmmPlugin' Failed\n");
        {
            if (pszError)
            {
                if (uMaxLen > 0)
                {
                    pSmmApi->Format(pszError, uMaxLen, "'ISmmPlugin' Failed");
                }
            }

            return false;
        }
    }

    if ((::g_PLID = pluginId) > -1)
    {
        {
            if (!(ifEngine = pSmmApi->GetEngineFactory()))
            {
                ::std::printf("'GetEngineFactory' Failed\n");
                {
                    if (pszError)
                    {
                        if (uMaxLen > 0)
                        {
                            pSmmApi->Format(pszError, uMaxLen, "'GetEngineFactory' Failed");
                        }
                    }

                    return false;
                }
            }

            if (!(ifServer = pSmmApi->GetServerFactory()))
            {
                ::std::printf("'GetServerFactory' Failed\n");
                {
                    if (pszError)
                    {
                        if (uMaxLen > 0)
                        {
                            pSmmApi->Format(pszError, uMaxLen, "'GetServerFactory' Failed");
                        }
                    }

                    return false;
                }
            }

            if (!(ifFileSystem = pSmmApi->GetFileSystemFactory()))
            {
                ::std::printf("'GetFileSystemFactory' Failed\n");
                {
                    if (pszError)
                    {
                        if (uMaxLen > 0)
                        {
                            pSmmApi->Format(pszError, uMaxLen, "'GetFileSystemFactory' Failed");
                        }
                    }

                    return false;
                }
            }
        }

        {
            if (!(::icvar = ((::ICvar*)(pSmmApi->VInterfaceMatch(ifEngine, CVAR_INTERFACE_VERSION)))))
            {
                ::std::printf("'%s' Failed\n", CVAR_INTERFACE_VERSION);
                {
                    if (pszError)
                    {
                        if (uMaxLen > 0)
                        {
                            pSmmApi->Format(pszError, uMaxLen, "'%s' Failed", CVAR_INTERFACE_VERSION);
                        }
                    }

                    return false;
                }
            }

            else
            {
                ::g_pCVar = ::icvar;
            }
        }

        {
            if (!(::engine = ((::IVEngineServer*)(pSmmApi->VInterfaceMatch(ifEngine, INTERFACEVERSION_VENGINESERVER)))))
            {
                ::std::printf("'%s' Failed\n", INTERFACEVERSION_VENGINESERVER);
                {
                    if (pszError)
                    {
                        if (uMaxLen > 0)
                        {
                            pSmmApi->Format(pszError, uMaxLen, "'%s' Failed", INTERFACEVERSION_VENGINESERVER);
                        }
                    }

                    return false;
                }
            }

            if (!(::gameevents = ((::IGameEventManager2*)(pSmmApi->VInterfaceMatch(ifEngine, INTERFACEVERSION_GAMEEVENTSMANAGER2)))))
            {
                ::std::printf("'%s' Failed\n", INTERFACEVERSION_GAMEEVENTSMANAGER2);
                {
                    if (pszError)
                    {
                        if (uMaxLen > 0)
                        {
                            pSmmApi->Format(pszError, uMaxLen, "'%s' Failed", INTERFACEVERSION_GAMEEVENTSMANAGER2);
                        }
                    }

                    return false;
                }
            }

            if (!(::helpers = ((::IServerPluginHelpers*)(pSmmApi->VInterfaceMatch(ifEngine, INTERFACEVERSION_ISERVERPLUGINHELPERS)))))
            {
                ::std::printf("'%s' Failed\n", INTERFACEVERSION_ISERVERPLUGINHELPERS);
                {
                    if (pszError)
                    {
                        if (uMaxLen > 0)
                        {
                            pSmmApi->Format(pszError, uMaxLen, "'%s' Failed", INTERFACEVERSION_ISERVERPLUGINHELPERS);
                        }
                    }

                    return false;
                }
            }
        }

        {
            if (!(::servertools = ((::IServerTools*)(pSmmApi->VInterfaceMatch(ifServer, VSERVERTOOLS_INTERFACE_VERSION)))))
            {
                ::std::printf("'%s' Failed\n", VSERVERTOOLS_INTERFACE_VERSION);
                {
                    if (pszError)
                    {
                        if (uMaxLen > 0)
                        {
                            pSmmApi->Format(pszError, uMaxLen, "'%s' Failed", VSERVERTOOLS_INTERFACE_VERSION);
                        }
                    }

                    return false;
                }
            }

            if (!(::playerinfomanager = ((::IPlayerInfoManager*)(pSmmApi->VInterfaceMatch(ifServer, INTERFACEVERSION_PLAYERINFOMANAGER)))))
            {
                ::std::printf("'%s' Failed\n", INTERFACEVERSION_PLAYERINFOMANAGER);
                {
                    if (pszError)
                    {
                        if (uMaxLen > 0)
                        {
                            pSmmApi->Format(pszError, uMaxLen, "'%s' Failed", INTERFACEVERSION_PLAYERINFOMANAGER);
                        }
                    }

                    return false;
                }
            }

            if (!(::gameclients = ((::IServerGameClients*)(pSmmApi->VInterfaceMatch(ifServer, INTERFACEVERSION_SERVERGAMECLIENTS)))))
            {
                ::std::printf("'%s' Failed\n", INTERFACEVERSION_SERVERGAMECLIENTS);
                {
                    if (pszError)
                    {
                        if (uMaxLen > 0)
                        {
                            pSmmApi->Format(pszError, uMaxLen, "'%s' Failed", INTERFACEVERSION_SERVERGAMECLIENTS);
                        }
                    }

                    return false;
                }
            }

            if (!(::server = ((::IServerGameDLL*)(pSmmApi->VInterfaceMatch(ifServer, INTERFACEVERSION_SERVERGAMEDLL)))))
            {
                ::std::printf("'%s' Failed\n", INTERFACEVERSION_SERVERGAMEDLL);
                {
                    if (pszError)
                    {
                        if (uMaxLen > 0)
                        {
                            pSmmApi->Format(pszError, uMaxLen, "'%s' Failed", INTERFACEVERSION_SERVERGAMEDLL);
                        }
                    }

                    return false;
                }
            }
        }

        {
            if (!(::filesystem = ((::IFileSystem*)(pSmmApi->VInterfaceMatch(ifFileSystem, FILESYSTEM_INTERFACE_VERSION)))))
            {
                ::std::printf("'%s' Failed\n", FILESYSTEM_INTERFACE_VERSION);
                {
                    if (pszError)
                    {
                        if (uMaxLen > 0)
                        {
                            pSmmApi->Format(pszError, uMaxLen, "'%s' Failed", FILESYSTEM_INTERFACE_VERSION);
                        }
                    }

                    return false;
                }
            }

            if (!(::basefilesystem = ((::IBaseFileSystem*)(pSmmApi->VInterfaceMatch(ifFileSystem, BASEFILESYSTEM_INTERFACE_VERSION)))))
            {
                ::std::printf("'%s' Failed\n", FILESYSTEM_INTERFACE_VERSION);
                {
                    if (pszError)
                    {
                        if (uMaxLen > 0)
                        {
                            pSmmApi->Format(pszError, uMaxLen, "'%s' Failed", FILESYSTEM_INTERFACE_VERSION);
                        }
                    }

                    return false;
                }
            }
        }
    }

    else
    {
        ::std::printf("'PluginId' Failed\n");
        {
            if (pszError)
            {
                if (uMaxLen > 0)
                {
                    pSmmApi->Format(pszError, uMaxLen, "'PluginId' Failed");
                }
            }

            return false;
        }
    }

    if (!(::gpGlobals = pSmmApi->GetCGlobals()))
    {
        ::std::printf("'GetCGlobals' Failed\n");
        {
            if (pszError)
            {
                if (uMaxLen > 0)
                {
                    pSmmApi->Format(pszError, uMaxLen, "'GetCGlobals' Failed");
                }
            }

            return false;
        }
    }

    if (pszGameDir = pSmmApi->GetBaseDir())
    {
        ::g_strGameDir.assign(pszGameDir);
        {
            if (::g_strGameDir.empty())
            {
                ::std::printf("'GetBaseDir' Failed\n");
                {
                    if (pszError)
                    {
                        if (uMaxLen > 0)
                        {
                            pSmmApi->Format(pszError, uMaxLen, "'GetBaseDir' Failed");
                        }
                    }

                    return false;
                }
            }
        }
    }

    else
    {
        ::std::printf("'GetBaseDir' Failed\n");
        {
            if (pszError)
            {
                if (uMaxLen > 0)
                {
                    pSmmApi->Format(pszError, uMaxLen, "'GetBaseDir' Failed");
                }
            }

            return false;
        }
    }

    if (!(::vsp_callbacks = pSmmApi->GetVSPInfo(nullptr)))
    {
        pSmmApi->AddListener(this, this);

        pSmmApi->EnableVSPListener();
    }

    if (!::g_pSharedChangeInfo)
    {
        ::g_pSharedChangeInfo = ::engine->GetSharedEdictChangeInfo();
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
        if (::FileExists(::g_strGameDir + "/cfg/gamemode_competitive.cfg"))
        {
            ::FileRemove(::g_strGameDir + "/cfg/gamemode_competitive.cfg");
        }

        ::FileCopy(::g_strGameDir + "/addons/hgsm/gamemode_competitive.cfg", ::g_strGameDir + "/cfg/gamemode_competitive.cfg");
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
        if (::FileExists(::g_strGameDir + "/cfg/valve.rc"))
        {
            ::FileRemove(::g_strGameDir + "/cfg/valve.rc");
        }

        ::FileCopy(::g_strGameDir + "/addons/hgsm/valve.rc", ::g_strGameDir + "/cfg/valve.rc");
    }

    if (::FileExists(::g_strGameDir + "/addons/hgsm/game.cfg"))
    {
        if (::FileExists(::g_strGameDir + "/cfg/game.cfg"))
        {
            ::FileRemove(::g_strGameDir + "/cfg/game.cfg");
        }

        ::FileCopy(::g_strGameDir + "/addons/hgsm/game.cfg", ::g_strGameDir + "/cfg/game.cfg");
    }

    if (::FileExists(::g_strGameDir + "/addons/hgsm/skill.cfg"))
    {
        if (::FileExists(::g_strGameDir + "/cfg/skill.cfg"))
        {
            ::FileRemove(::g_strGameDir + "/cfg/skill.cfg");
        }

        ::FileCopy(::g_strGameDir + "/addons/hgsm/skill.cfg", ::g_strGameDir + "/cfg/skill.cfg");
    }

    if (::FileExists(::g_strGameDir + "/addons/hgsm/skill1.cfg"))
    {
        if (::FileExists(::g_strGameDir + "/cfg/skill1.cfg"))
        {
            ::FileRemove(::g_strGameDir + "/cfg/skill1.cfg");
        }

        ::FileCopy(::g_strGameDir + "/addons/hgsm/skill1.cfg", ::g_strGameDir + "/cfg/skill1.cfg");
    }

    if (::FileExists(::g_strGameDir + "/addons/hgsm/config.cfg"))
    {
        if (::FileExists(::g_strGameDir + "/cfg/config.cfg"))
        {
            ::FileRemove(::g_strGameDir + "/cfg/config.cfg");
        }

        ::FileCopy(::g_strGameDir + "/addons/hgsm/config.cfg", ::g_strGameDir + "/cfg/config.cfg");
    }

    if (::FileExists(::g_strGameDir + "/addons/hgsm/listenserver.cfg"))
    {
        if (::FileExists(::g_strGameDir + "/cfg/listenserver.cfg"))
        {
            ::FileRemove(::g_strGameDir + "/cfg/listenserver.cfg");
        }

        ::FileCopy(::g_strGameDir + "/addons/hgsm/listenserver.cfg", ::g_strGameDir + "/cfg/listenserver.cfg");
    }

    if (::FileExists(::g_strGameDir + "/addons/hgsm/autoexec.cfg"))
    {
        if (::FileExists(::g_strGameDir + "/cfg/autoexec.cfg"))
        {
            ::FileRemove(::g_strGameDir + "/cfg/autoexec.cfg");
        }

        ::FileCopy(::g_strGameDir + "/addons/hgsm/autoexec.cfg", ::g_strGameDir + "/cfg/autoexec.cfg");
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

    if (!::g_bHooked)
    {
        {
            SH_ADD_HOOK_MEMFUNC(IServerGameDLL, GetTickInterval, ::server, this, &::MySmmPlugin::Hook_GetTickInterval, { });
            SH_ADD_HOOK_MEMFUNC(IServerGameDLL, LevelInit, ::server, this, &::MySmmPlugin::Hook_LevelInit, { });
            SH_ADD_HOOK_MEMFUNC(IServerGameDLL, LevelShutdown, ::server, this, &::MySmmPlugin::Hook_LevelShutdown, { });
        }

        {
            SH_ADD_HOOK_MEMFUNC(IServerGameClients, ClientConnect, ::gameclients, this, &::MySmmPlugin::Hook_ClientConnect, { });
            SH_ADD_HOOK_MEMFUNC(IServerGameClients, ClientDisconnect, ::gameclients, this, &::MySmmPlugin::Hook_ClientDisconnect, { });
        }

        {
            SH_ADD_HOOK_MEMFUNC(IServerGameDLL, ServerActivate, ::server, this, &::MySmmPlugin::Hook_ServerActivate_Post, true);
            SH_ADD_HOOK_MEMFUNC(IServerGameDLL, LevelInit, ::server, this, &::MySmmPlugin::Hook_LevelInit_Post, true);
            SH_ADD_HOOK_MEMFUNC(IServerGameDLL, LevelShutdown, ::server, this, &::MySmmPlugin::Hook_LevelShutdown_Post, true);
            SH_ADD_HOOK_MEMFUNC(IServerGameDLL, GameFrame, ::server, this, &::MySmmPlugin::Hook_GameFrame_Post, true);
            SH_ADD_HOOK_MEMFUNC(IServerGameDLL, GetTickInterval, ::server, this, &::MySmmPlugin::Hook_GetTickInterval_Post, true);
        }

        {
            SH_ADD_HOOK_MEMFUNC(IServerGameClients, ClientConnect, ::gameclients, this, &::MySmmPlugin::Hook_ClientConnect_Post, true);
            SH_ADD_HOOK_MEMFUNC(IServerGameClients, ClientPutInServer, ::gameclients, this, &::MySmmPlugin::Hook_ClientPutInServer_Post, true);
            SH_ADD_HOOK_MEMFUNC(IServerGameClients, ClientDisconnect, ::gameclients, this, &::MySmmPlugin::Hook_ClientDisconnect_Post, true);
        }

        ::g_bHooked = true;
    }

    if (!::g_bEvHooked)
    {
        if (::gameevents->AddListener(this, "player_spawn", true))
        {
            ::gameevents->AddListener(this, "player_death", true);
            {
                ::g_bEvHooked = true;
            }
        }
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

    if (!::icvar->FindVar(::g_cvVersion.GetName()))
    {
        ::ConVar_Register(FCVAR_NOTIFY | FCVAR_SPONLY, this);
    }

    if (bLateLoaded)
    {
        if (!::g_bNoReturn)
        {
            ::g_bNoReturn = true;

            {
                Hook_GetTickInterval();
                {
                    Hook_GetTickInterval_Post();
                }

                Hook_ServerActivate_Post(nullptr, { }, { });
                {
                    Hook_LevelInit(nullptr, nullptr, nullptr, nullptr, { }, { });
                    {
                        Hook_LevelInit_Post(nullptr, nullptr, nullptr, nullptr, { }, { });
                    }
                }

                for (nPlayer = xTo(true, int); nPlayer <= ::gpGlobals->maxClients; nPlayer++)
                {

#if SOURCE_ENGINE != SE_CSGO

                    if (pEdict = ::engine->PEntityOfEntIndex(nPlayer))

#else

                    if (pEdict = ::gpGlobals->pEdicts + nPlayer)

#endif

                    {
                        if (!(pEdict->m_fStateFlags & FL_EDICT_FREE))
                        {
                            if (!(pEdict->m_fStateFlags & FL_EDICT_DONTSEND))
                            {
                                if (pEdict->m_NetworkSerialNumber)
                                {
                                    if (pEdict->m_pNetworkable)
                                    {
                                        if (pServerEntity = pEdict->GetIServerEntity())
                                        {
                                            if (pServerUnknown = pEdict->GetUnknown())
                                            {
                                                if (pBaseEntity = pServerEntity->GetBaseEntity())
                                                {
                                                    if (pPlayerInfo = ::playerinfomanager->GetPlayerInfo(pEdict))
                                                    {
                                                        if (pPlayerInfo->IsPlayer())
                                                        {
                                                            if (pPlayerInfo->IsConnected())
                                                            {
                                                                Hook_ClientConnect(pEdict, pPlayerInfo->GetName(), nullptr, nullptr, { });
                                                                {
                                                                    Hook_ClientConnect_Post(pEdict, pPlayerInfo->GetName(), nullptr, nullptr, { });
                                                                    {
                                                                        Hook_ClientPutInServer_Post(pEdict, pPlayerInfo->GetName());
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

            ::g_bNoReturn = { };
        }
    }

#ifdef WIN32

    ::MultiMediaFeatureTryAttach();

#endif

    return true;
}

bool ::MySmmPlugin::Unload(char*, unsigned int) noexcept
{
    if (::g_bHooked)
    {
        {
            SH_REMOVE_HOOK_MEMFUNC(IServerGameDLL, GetTickInterval, ::server, this, &::MySmmPlugin::Hook_GetTickInterval, { });
            SH_REMOVE_HOOK_MEMFUNC(IServerGameDLL, LevelInit, ::server, this, &::MySmmPlugin::Hook_LevelInit, { });
            SH_REMOVE_HOOK_MEMFUNC(IServerGameDLL, LevelShutdown, ::server, this, &::MySmmPlugin::Hook_LevelShutdown, { });
        }

        {
            SH_REMOVE_HOOK_MEMFUNC(IServerGameClients, ClientConnect, ::gameclients, this, &::MySmmPlugin::Hook_ClientConnect, { });
            SH_REMOVE_HOOK_MEMFUNC(IServerGameClients, ClientDisconnect, ::gameclients, this, &::MySmmPlugin::Hook_ClientDisconnect, { });
        }

        {
            SH_REMOVE_HOOK_MEMFUNC(IServerGameDLL, ServerActivate, ::server, this, &::MySmmPlugin::Hook_ServerActivate_Post, true);
            SH_REMOVE_HOOK_MEMFUNC(IServerGameDLL, LevelInit, ::server, this, &::MySmmPlugin::Hook_LevelInit_Post, true);
            SH_REMOVE_HOOK_MEMFUNC(IServerGameDLL, LevelShutdown, ::server, this, &::MySmmPlugin::Hook_LevelShutdown_Post, true);
            SH_REMOVE_HOOK_MEMFUNC(IServerGameDLL, GameFrame, ::server, this, &::MySmmPlugin::Hook_GameFrame_Post, true);
            SH_REMOVE_HOOK_MEMFUNC(IServerGameDLL, GetTickInterval, ::server, this, &::MySmmPlugin::Hook_GetTickInterval_Post, true);
        }

        {
            SH_REMOVE_HOOK_MEMFUNC(IServerGameClients, ClientConnect, ::gameclients, this, &::MySmmPlugin::Hook_ClientConnect_Post, true);
            SH_REMOVE_HOOK_MEMFUNC(IServerGameClients, ClientPutInServer, ::gameclients, this, &::MySmmPlugin::Hook_ClientPutInServer_Post, true);
            SH_REMOVE_HOOK_MEMFUNC(IServerGameClients, ClientDisconnect, ::gameclients, this, &::MySmmPlugin::Hook_ClientDisconnect_Post, true);
        }

        ::g_bHooked = { };
    }

    if (::g_bEvHooked)
    {
        ::gameevents->RemoveListener(this);
        {
            ::g_bEvHooked = { };
        }
    }

    if (::icvar->FindVar(::g_cvVersion.GetName()))
    {
        ::ConVar_Unregister();
    }

#ifdef WIN32

    ::MultiMediaFeatureTryDetach();

#endif

    return true;
}

bool ::MySmmPlugin::Pause(char*, unsigned int) noexcept
{
    if (::g_bHooked)
    {
        {
            SH_REMOVE_HOOK_MEMFUNC(IServerGameDLL, GetTickInterval, ::server, this, &::MySmmPlugin::Hook_GetTickInterval, { });
            SH_REMOVE_HOOK_MEMFUNC(IServerGameDLL, LevelInit, ::server, this, &::MySmmPlugin::Hook_LevelInit, { });
            SH_REMOVE_HOOK_MEMFUNC(IServerGameDLL, LevelShutdown, ::server, this, &::MySmmPlugin::Hook_LevelShutdown, { });
        }

        {
            SH_REMOVE_HOOK_MEMFUNC(IServerGameClients, ClientConnect, ::gameclients, this, &::MySmmPlugin::Hook_ClientConnect, { });
            SH_REMOVE_HOOK_MEMFUNC(IServerGameClients, ClientDisconnect, ::gameclients, this, &::MySmmPlugin::Hook_ClientDisconnect, { });
        }

        {
            SH_REMOVE_HOOK_MEMFUNC(IServerGameDLL, ServerActivate, ::server, this, &::MySmmPlugin::Hook_ServerActivate_Post, true);
            SH_REMOVE_HOOK_MEMFUNC(IServerGameDLL, LevelInit, ::server, this, &::MySmmPlugin::Hook_LevelInit_Post, true);
            SH_REMOVE_HOOK_MEMFUNC(IServerGameDLL, LevelShutdown, ::server, this, &::MySmmPlugin::Hook_LevelShutdown_Post, true);
            SH_REMOVE_HOOK_MEMFUNC(IServerGameDLL, GameFrame, ::server, this, &::MySmmPlugin::Hook_GameFrame_Post, true);
            SH_REMOVE_HOOK_MEMFUNC(IServerGameDLL, GetTickInterval, ::server, this, &::MySmmPlugin::Hook_GetTickInterval_Post, true);
        }

        {
            SH_REMOVE_HOOK_MEMFUNC(IServerGameClients, ClientConnect, ::gameclients, this, &::MySmmPlugin::Hook_ClientConnect_Post, true);
            SH_REMOVE_HOOK_MEMFUNC(IServerGameClients, ClientPutInServer, ::gameclients, this, &::MySmmPlugin::Hook_ClientPutInServer_Post, true);
            SH_REMOVE_HOOK_MEMFUNC(IServerGameClients, ClientDisconnect, ::gameclients, this, &::MySmmPlugin::Hook_ClientDisconnect_Post, true);
        }

        ::g_bHooked = { };
    }

    if (::g_bEvHooked)
    {
        ::gameevents->RemoveListener(this);
        {
            ::g_bEvHooked = { };
        }
    }

    return true;
}

bool ::MySmmPlugin::Unpause(char*, unsigned int) noexcept
{
    if (!::g_bHooked)
    {
        {
            SH_ADD_HOOK_MEMFUNC(IServerGameDLL, GetTickInterval, ::server, this, &::MySmmPlugin::Hook_GetTickInterval, { });
            SH_ADD_HOOK_MEMFUNC(IServerGameDLL, LevelInit, ::server, this, &::MySmmPlugin::Hook_LevelInit, { });
            SH_ADD_HOOK_MEMFUNC(IServerGameDLL, LevelShutdown, ::server, this, &::MySmmPlugin::Hook_LevelShutdown, { });
        }

        {
            SH_ADD_HOOK_MEMFUNC(IServerGameClients, ClientConnect, ::gameclients, this, &::MySmmPlugin::Hook_ClientConnect, { });
            SH_ADD_HOOK_MEMFUNC(IServerGameClients, ClientDisconnect, ::gameclients, this, &::MySmmPlugin::Hook_ClientDisconnect, { });
        }

        {
            SH_ADD_HOOK_MEMFUNC(IServerGameDLL, ServerActivate, ::server, this, &::MySmmPlugin::Hook_ServerActivate_Post, true);
            SH_ADD_HOOK_MEMFUNC(IServerGameDLL, LevelInit, ::server, this, &::MySmmPlugin::Hook_LevelInit_Post, true);
            SH_ADD_HOOK_MEMFUNC(IServerGameDLL, LevelShutdown, ::server, this, &::MySmmPlugin::Hook_LevelShutdown_Post, true);
            SH_ADD_HOOK_MEMFUNC(IServerGameDLL, GameFrame, ::server, this, &::MySmmPlugin::Hook_GameFrame_Post, true);
            SH_ADD_HOOK_MEMFUNC(IServerGameDLL, GetTickInterval, ::server, this, &::MySmmPlugin::Hook_GetTickInterval_Post, true);
        }

        {
            SH_ADD_HOOK_MEMFUNC(IServerGameClients, ClientConnect, ::gameclients, this, &::MySmmPlugin::Hook_ClientConnect_Post, true);
            SH_ADD_HOOK_MEMFUNC(IServerGameClients, ClientPutInServer, ::gameclients, this, &::MySmmPlugin::Hook_ClientPutInServer_Post, true);
            SH_ADD_HOOK_MEMFUNC(IServerGameClients, ClientDisconnect, ::gameclients, this, &::MySmmPlugin::Hook_ClientDisconnect_Post, true);
        }

        ::g_bHooked = true;
    }

    if (!::g_bEvHooked)
    {
        if (::gameevents->AddListener(this, "player_spawn", true))
        {
            ::gameevents->AddListener(this, "player_death", true);
            {
                ::g_bEvHooked = true;
            }
        }
    }

    return true;
}

void ::MySmmPlugin::AllPluginsLoaded() noexcept
{
    if (::g_bHooked)
    {
        if (!::g_bEvHooked)
        {
            if (::gameevents->AddListener(this, "player_spawn", true))
            {
                ::gameevents->AddListener(this, "player_death", true);
                {
                    ::g_bEvHooked = true;
                }
            }
        }
    }
}

void ::MySmmPlugin::OnVSPListening(::IServerPluginCallbacks* iface) noexcept
{
    ::vsp_callbacks = iface;
}

float ::MySmmPlugin::Hook_GetTickInterval() const noexcept
{
    static ::std::string strPath{ };
    static ::std::ifstream inFile{ };
    static ::nlohmann::json jsTree{ };

    static int nPrio{ };
    static float fTickRate{ };

#ifdef WIN32

    static int nDisablePrioBoost{ };

    static bool bDisablePrioBoost{ };

#endif

    {
        ::g_fIntervalPerTick = { };
    }

    strPath.assign(::g_strGameDir + "/addons/hgsm/offs.cfg");

    if (!::FileExists(strPath))
    {
        ::std::printf("Failed To Find '%s'\n", strPath.c_str());
        {
            ::std::printf("'GetDataDescMap' Is Now '%d'\n", ::x_nDataMap);
            ::std::printf("'CSendPropExtra_UtlVector::m_Offset' Is Now '%d'\n", ::x_nSendProp);
        }
    }

    else
    {
        inFile.open(strPath);

        if (!inFile.is_open())
        {
            ::std::printf("Failed To Open '%s'\n", strPath.c_str());
            {
                inFile.clear();
                {
                    ::std::printf("'GetDataDescMap' Is Now '%d'\n", ::x_nDataMap);
                    ::std::printf("'CSendPropExtra_UtlVector::m_Offset' Is Now '%d'\n", ::x_nSendProp);
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
                ::std::printf("'%s' Has INVALID 'JSON'\n", strPath.c_str());
                {
                    jsTree.clear();
                    {
                        ::std::printf("'GetDataDescMap' Is Now '%d'\n", ::x_nDataMap);
                        ::std::printf("'CSendPropExtra_UtlVector::m_Offset' Is Now '%d'\n", ::x_nSendProp);
                    }
                }
            }

            else
            {
                if (jsTree.empty())
                {
                    ::std::printf("Loaded '%s'\n", strPath.c_str());
                    {
                        jsTree.clear();
                        {
                            ::std::printf("'GetDataDescMap' Is Now '%d'\n", ::x_nDataMap);
                            ::std::printf("'CSendPropExtra_UtlVector::m_Offset' Is Now '%d'\n", ::x_nSendProp);
                        }
                    }
                }

                else
                {
                    if (!jsTree.is_object())
                    {
                        ::std::printf("Loaded '%s'\n", strPath.c_str());
                        {
                            jsTree.clear();
                            {
                                ::std::printf("'GetDataDescMap' Is Now '%d'\n", ::x_nDataMap);
                                ::std::printf("'CSendPropExtra_UtlVector::m_Offset' Is Now '%d'\n", ::x_nSendProp);
                            }
                        }
                    }

                    else
                    {

#ifdef WIN32

                        if (jsTree["Windows->GetDataDescMap"].empty())
                        {
                            ::std::printf("'GetDataDescMap' Is Now '%d'\n", ::x_nDataMap);
                        }

                        else if (!jsTree["Windows->GetDataDescMap"].is_number())
                        {
                            ::std::printf("'GetDataDescMap' Is Now '%d'\n", ::x_nDataMap);
                        }

                        else
                        {
                            ::x_nDataMap = jsTree["Windows->GetDataDescMap"].get < int >();
                            {
                                ::std::printf("'GetDataDescMap' Is Now '%d'\n", ::x_nDataMap);
                            }
                        }

                        if (jsTree["Windows->CSendPropExtra_UtlVector::m_Offset"].empty())
                        {
                            ::std::printf("'CSendPropExtra_UtlVector::m_Offset' Is Now '%d'\n", ::x_nSendProp);
                        }

                        else if (!jsTree["Windows->CSendPropExtra_UtlVector::m_Offset"].is_number())
                        {
                            ::std::printf("'CSendPropExtra_UtlVector::m_Offset' Is Now '%d'\n", ::x_nSendProp);
                        }

                        else
                        {
                            ::x_nSendProp = jsTree["Windows->CSendPropExtra_UtlVector::m_Offset"].get < int >();
                            {
                                ::std::printf("'CSendPropExtra_UtlVector::m_Offset' Is Now '%d'\n", ::x_nSendProp);
                            }
                        }

#else

                        if (jsTree["Linux->GetDataDescMap"].empty())
                        {
                            ::std::printf("'GetDataDescMap' Is Now '%d'\n", ::x_nDataMap);
                        }

                        else if (!jsTree["Linux->GetDataDescMap"].is_number())
                        {
                            ::std::printf("'GetDataDescMap' Is Now '%d'\n", ::x_nDataMap);
                        }

                        else
                        {
                            ::x_nDataMap = jsTree["Linux->GetDataDescMap"].get < int >();
                            {
                                ::std::printf("'GetDataDescMap' Is Now '%d'\n", ::x_nDataMap);
                            }
                        }

                        if (jsTree["Linux->CSendPropExtra_UtlVector::m_Offset"].empty())
                        {
                            ::std::printf("'CSendPropExtra_UtlVector::m_Offset' Is Now '%d'\n", ::x_nSendProp);
                        }

                        else if (!jsTree["Linux->CSendPropExtra_UtlVector::m_Offset"].is_number())
                        {
                            ::std::printf("'CSendPropExtra_UtlVector::m_Offset' Is Now '%d'\n", ::x_nSendProp);
                        }

                        else
                        {
                            ::x_nSendProp = jsTree["Linux->CSendPropExtra_UtlVector::m_Offset"].get < int >();
                            {
                                ::std::printf("'CSendPropExtra_UtlVector::m_Offset' Is Now '%d'\n", ::x_nSendProp);
                            }
                        }

#endif

                        jsTree.clear();
                        {
                            ::std::printf("Loaded '%s'\n", strPath.c_str());
                        }
                    }
                }
            }
        }
    }

    strPath.assign(::g_strGameDir + "/addons/hgsm/server_tick_rate_and_priority.cfg");

    if (!::FileExists(strPath))
    {
        ::std::printf("Failed To Find '%s'\n", strPath.c_str());
        {
            ::std::printf("'Ticks Loop' Is Now '%d'\n", ::g_nTicks);
            ::std::printf("'Keep Helmet' Is Now '%s'\n", ::g_bKeepHelmet ? "TRUE" : "FALSE");
            ::std::printf("'Reliable' Is Now '%s'\n", ::g_bReliable ? "TRUE" : "FALSE");
            ::std::printf("'Hourly Restart' Is Now '%s'\n", ::g_bHourlyRestart ? "TRUE" : "FALSE");
            ::std::printf("'Fix Score' Is Now '%s'\n", ::g_bFixScore ? "TRUE" : "FALSE");
            ::std::printf("'Fix Kills' Is Now '%s'\n", ::g_bFixKills ? "TRUE" : "FALSE");
            ::std::printf("'Fix Deaths' Is Now '%s'\n", ::g_bFixDeaths ? "TRUE" : "FALSE");
            ::std::printf("'Zero Money' Is Now '%s'\n", ::g_bZeroMoney ? "TRUE" : "FALSE");
            ::std::printf("'Hide Radar' Is Now '%s'\n", ::g_bHideRadar ? "TRUE" : "FALSE");
            ::std::printf("'Real Score' Is Now '%s'\n", ::g_bRealScore ? "TRUE" : "FALSE");
            ::std::printf("'Real Score Friendly' Is Now '%s'\n", ::g_bRealScoreFriendly ? "TRUE" : "FALSE");
            ::std::printf("'Friendly Decrease' Is Now '%s'\n", ::g_bFriendlyDecrease ? "TRUE" : "FALSE");
            ::std::printf("'Give Armor In Spawn' Is Now '%d'\n", ::g_nArmor);
            ::std::printf("'Welcome Console 1' Is Now '%s'\n", ::g_strWelcome_1.c_str());
            ::std::printf("'Welcome Console 2' Is Now '%s'\n", ::g_strWelcome_2.c_str());
            ::std::printf("'Welcome Console 3' Is Now '%s'\n", ::g_strWelcome_3.c_str());
            ::std::printf("'Welcome Console 4' Is Now '%s'\n", ::g_strWelcome_4.c_str());
            ::std::printf("'Welcome Console 5' Is Now '%s'\n", ::g_strWelcome_5.c_str());
            ::std::printf("'Welcome Console 6' Is Now '%s'\n", ::g_strWelcome_6.c_str());
            ::std::printf("'Welcome Notify 1' Is Now '%s'\n", ::g_strNotify_1.c_str());
            ::std::printf("'Welcome Notify 2' Is Now '%s'\n", ::g_strNotify_2.c_str());

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

            ::std::printf("'Tick Rate' Is Now '%f' (~ %f Interval Per Tick)\n", 1.f / ::gpGlobals->interval_per_tick, ::gpGlobals->interval_per_tick);
        }
    }

    else
    {
        inFile.open(strPath);

        if (!inFile.is_open())
        {
            ::std::printf("Failed To Open '%s'\n", strPath.c_str());
            {
                inFile.clear();
                {
                    ::std::printf("'Ticks Loop' Is Now '%d'\n", ::g_nTicks);
                    ::std::printf("'Keep Helmet' Is Now '%s'\n", ::g_bKeepHelmet ? "TRUE" : "FALSE");
                    ::std::printf("'Reliable' Is Now '%s'\n", ::g_bReliable ? "TRUE" : "FALSE");
                    ::std::printf("'Hourly Restart' Is Now '%s'\n", ::g_bHourlyRestart ? "TRUE" : "FALSE");
                    ::std::printf("'Fix Score' Is Now '%s'\n", ::g_bFixScore ? "TRUE" : "FALSE");
                    ::std::printf("'Fix Kills' Is Now '%s'\n", ::g_bFixKills ? "TRUE" : "FALSE");
                    ::std::printf("'Fix Deaths' Is Now '%s'\n", ::g_bFixDeaths ? "TRUE" : "FALSE");
                    ::std::printf("'Zero Money' Is Now '%s'\n", ::g_bZeroMoney ? "TRUE" : "FALSE");
                    ::std::printf("'Hide Radar' Is Now '%s'\n", ::g_bHideRadar ? "TRUE" : "FALSE");
                    ::std::printf("'Real Score' Is Now '%s'\n", ::g_bRealScore ? "TRUE" : "FALSE");
                    ::std::printf("'Real Score Friendly' Is Now '%s'\n", ::g_bRealScoreFriendly ? "TRUE" : "FALSE");
                    ::std::printf("'Friendly Decrease' Is Now '%s'\n", ::g_bFriendlyDecrease ? "TRUE" : "FALSE");
                    ::std::printf("'Give Armor In Spawn' Is Now '%d'\n", ::g_nArmor);
                    ::std::printf("'Welcome Console 1' Is Now '%s'\n", ::g_strWelcome_1.c_str());
                    ::std::printf("'Welcome Console 2' Is Now '%s'\n", ::g_strWelcome_2.c_str());
                    ::std::printf("'Welcome Console 3' Is Now '%s'\n", ::g_strWelcome_3.c_str());
                    ::std::printf("'Welcome Console 4' Is Now '%s'\n", ::g_strWelcome_4.c_str());
                    ::std::printf("'Welcome Console 5' Is Now '%s'\n", ::g_strWelcome_5.c_str());
                    ::std::printf("'Welcome Console 6' Is Now '%s'\n", ::g_strWelcome_6.c_str());
                    ::std::printf("'Welcome Notify 1' Is Now '%s'\n", ::g_strNotify_1.c_str());
                    ::std::printf("'Welcome Notify 2' Is Now '%s'\n", ::g_strNotify_2.c_str());

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

                    ::std::printf("'Tick Rate' Is Now '%f' (~ %f Interval Per Tick)\n", 1.f / ::gpGlobals->interval_per_tick, ::gpGlobals->interval_per_tick);
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
                ::std::printf("'%s' Has INVALID 'JSON'\n", strPath.c_str());
                {
                    jsTree.clear();
                    {
                        ::std::printf("'Ticks Loop' Is Now '%d'\n", ::g_nTicks);
                        ::std::printf("'Keep Helmet' Is Now '%s'\n", ::g_bKeepHelmet ? "TRUE" : "FALSE");
                        ::std::printf("'Reliable' Is Now '%s'\n", ::g_bReliable ? "TRUE" : "FALSE");
                        ::std::printf("'Hourly Restart' Is Now '%s'\n", ::g_bHourlyRestart ? "TRUE" : "FALSE");
                        ::std::printf("'Fix Score' Is Now '%s'\n", ::g_bFixScore ? "TRUE" : "FALSE");
                        ::std::printf("'Fix Kills' Is Now '%s'\n", ::g_bFixKills ? "TRUE" : "FALSE");
                        ::std::printf("'Fix Deaths' Is Now '%s'\n", ::g_bFixDeaths ? "TRUE" : "FALSE");
                        ::std::printf("'Zero Money' Is Now '%s'\n", ::g_bZeroMoney ? "TRUE" : "FALSE");
                        ::std::printf("'Hide Radar' Is Now '%s'\n", ::g_bHideRadar ? "TRUE" : "FALSE");
                        ::std::printf("'Real Score' Is Now '%s'\n", ::g_bRealScore ? "TRUE" : "FALSE");
                        ::std::printf("'Real Score Friendly' Is Now '%s'\n", ::g_bRealScoreFriendly ? "TRUE" : "FALSE");
                        ::std::printf("'Friendly Decrease' Is Now '%s'\n", ::g_bFriendlyDecrease ? "TRUE" : "FALSE");
                        ::std::printf("'Give Armor In Spawn' Is Now '%d'\n", ::g_nArmor);
                        ::std::printf("'Welcome Console 1' Is Now '%s'\n", ::g_strWelcome_1.c_str());
                        ::std::printf("'Welcome Console 2' Is Now '%s'\n", ::g_strWelcome_2.c_str());
                        ::std::printf("'Welcome Console 3' Is Now '%s'\n", ::g_strWelcome_3.c_str());
                        ::std::printf("'Welcome Console 4' Is Now '%s'\n", ::g_strWelcome_4.c_str());
                        ::std::printf("'Welcome Console 5' Is Now '%s'\n", ::g_strWelcome_5.c_str());
                        ::std::printf("'Welcome Console 6' Is Now '%s'\n", ::g_strWelcome_6.c_str());
                        ::std::printf("'Welcome Notify 1' Is Now '%s'\n", ::g_strNotify_1.c_str());
                        ::std::printf("'Welcome Notify 2' Is Now '%s'\n", ::g_strNotify_2.c_str());

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

                        ::std::printf("'Tick Rate' Is Now '%f' (~ %f Interval Per Tick)\n", 1.f / ::gpGlobals->interval_per_tick, ::gpGlobals->interval_per_tick);
                    }
                }
            }

            else
            {
                if (jsTree.empty())
                {
                    ::std::printf("Loaded '%s'\n", strPath.c_str());
                    {
                        jsTree.clear();
                        {
                            ::std::printf("'Ticks Loop' Is Now '%d'\n", ::g_nTicks);
                            ::std::printf("'Keep Helmet' Is Now '%s'\n", ::g_bKeepHelmet ? "TRUE" : "FALSE");
                            ::std::printf("'Reliable' Is Now '%s'\n", ::g_bReliable ? "TRUE" : "FALSE");
                            ::std::printf("'Hourly Restart' Is Now '%s'\n", ::g_bHourlyRestart ? "TRUE" : "FALSE");
                            ::std::printf("'Fix Score' Is Now '%s'\n", ::g_bFixScore ? "TRUE" : "FALSE");
                            ::std::printf("'Fix Kills' Is Now '%s'\n", ::g_bFixKills ? "TRUE" : "FALSE");
                            ::std::printf("'Fix Deaths' Is Now '%s'\n", ::g_bFixDeaths ? "TRUE" : "FALSE");
                            ::std::printf("'Zero Money' Is Now '%s'\n", ::g_bZeroMoney ? "TRUE" : "FALSE");
                            ::std::printf("'Hide Radar' Is Now '%s'\n", ::g_bHideRadar ? "TRUE" : "FALSE");
                            ::std::printf("'Real Score' Is Now '%s'\n", ::g_bRealScore ? "TRUE" : "FALSE");
                            ::std::printf("'Real Score Friendly' Is Now '%s'\n", ::g_bRealScoreFriendly ? "TRUE" : "FALSE");
                            ::std::printf("'Friendly Decrease' Is Now '%s'\n", ::g_bFriendlyDecrease ? "TRUE" : "FALSE");
                            ::std::printf("'Give Armor In Spawn' Is Now '%d'\n", ::g_nArmor);
                            ::std::printf("'Welcome Console 1' Is Now '%s'\n", ::g_strWelcome_1.c_str());
                            ::std::printf("'Welcome Console 2' Is Now '%s'\n", ::g_strWelcome_2.c_str());
                            ::std::printf("'Welcome Console 3' Is Now '%s'\n", ::g_strWelcome_3.c_str());
                            ::std::printf("'Welcome Console 4' Is Now '%s'\n", ::g_strWelcome_4.c_str());
                            ::std::printf("'Welcome Console 5' Is Now '%s'\n", ::g_strWelcome_5.c_str());
                            ::std::printf("'Welcome Console 6' Is Now '%s'\n", ::g_strWelcome_6.c_str());
                            ::std::printf("'Welcome Notify 1' Is Now '%s'\n", ::g_strNotify_1.c_str());
                            ::std::printf("'Welcome Notify 2' Is Now '%s'\n", ::g_strNotify_2.c_str());

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

                            ::std::printf("'Tick Rate' Is Now '%f' (~ %f Interval Per Tick)\n", 1.f / ::gpGlobals->interval_per_tick, ::gpGlobals->interval_per_tick);
                        }
                    }
                }

                else
                {
                    if (!jsTree.is_object())
                    {
                        ::std::printf("Loaded '%s'\n", strPath.c_str());
                        {
                            jsTree.clear();
                            {
                                ::std::printf("'Ticks Loop' Is Now '%d'\n", ::g_nTicks);
                                ::std::printf("'Keep Helmet' Is Now '%s'\n", ::g_bKeepHelmet ? "TRUE" : "FALSE");
                                ::std::printf("'Reliable' Is Now '%s'\n", ::g_bReliable ? "TRUE" : "FALSE");
                                ::std::printf("'Hourly Restart' Is Now '%s'\n", ::g_bHourlyRestart ? "TRUE" : "FALSE");
                                ::std::printf("'Fix Score' Is Now '%s'\n", ::g_bFixScore ? "TRUE" : "FALSE");
                                ::std::printf("'Fix Kills' Is Now '%s'\n", ::g_bFixKills ? "TRUE" : "FALSE");
                                ::std::printf("'Fix Deaths' Is Now '%s'\n", ::g_bFixDeaths ? "TRUE" : "FALSE");
                                ::std::printf("'Zero Money' Is Now '%s'\n", ::g_bZeroMoney ? "TRUE" : "FALSE");
                                ::std::printf("'Hide Radar' Is Now '%s'\n", ::g_bHideRadar ? "TRUE" : "FALSE");
                                ::std::printf("'Real Score' Is Now '%s'\n", ::g_bRealScore ? "TRUE" : "FALSE");
                                ::std::printf("'Real Score Friendly' Is Now '%s'\n", ::g_bRealScoreFriendly ? "TRUE" : "FALSE");
                                ::std::printf("'Friendly Decrease' Is Now '%s'\n", ::g_bFriendlyDecrease ? "TRUE" : "FALSE");
                                ::std::printf("'Give Armor In Spawn' Is Now '%d'\n", ::g_nArmor);
                                ::std::printf("'Welcome Console 1' Is Now '%s'\n", ::g_strWelcome_1.c_str());
                                ::std::printf("'Welcome Console 2' Is Now '%s'\n", ::g_strWelcome_2.c_str());
                                ::std::printf("'Welcome Console 3' Is Now '%s'\n", ::g_strWelcome_3.c_str());
                                ::std::printf("'Welcome Console 4' Is Now '%s'\n", ::g_strWelcome_4.c_str());
                                ::std::printf("'Welcome Console 5' Is Now '%s'\n", ::g_strWelcome_5.c_str());
                                ::std::printf("'Welcome Console 6' Is Now '%s'\n", ::g_strWelcome_6.c_str());
                                ::std::printf("'Welcome Notify 1' Is Now '%s'\n", ::g_strNotify_1.c_str());
                                ::std::printf("'Welcome Notify 2' Is Now '%s'\n", ::g_strNotify_2.c_str());

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

                                ::std::printf("'Tick Rate' Is Now '%f' (~ %f Interval Per Tick)\n", 1.f / ::gpGlobals->interval_per_tick, ::gpGlobals->interval_per_tick);
                            }
                        }
                    }

                    else
                    {
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

                        if (jsTree["Reliable"].empty())
                        {
                            ::std::printf("'Reliable' Is Now '%s'\n", ::g_bReliable ? "TRUE" : "FALSE");
                        }

                        else if (!jsTree["Reliable"].is_boolean())
                        {
                            ::std::printf("'Reliable' Is Now '%s'\n", ::g_bReliable ? "TRUE" : "FALSE");
                        }

                        else
                        {
                            ::g_bReliable = jsTree["Reliable"].get < bool >();
                            {
                                ::std::printf("'Reliable' Is Now '%s'\n", ::g_bReliable ? "TRUE" : "FALSE");
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

                        if (jsTree["Friendly Decrease"].empty())
                        {
                            ::std::printf("'Friendly Decrease' Is Now '%s'\n", ::g_bFriendlyDecrease ? "TRUE" : "FALSE");
                        }

                        else if (!jsTree["Friendly Decrease"].is_boolean())
                        {
                            ::std::printf("'Friendly Decrease' Is Now '%s'\n", ::g_bFriendlyDecrease ? "TRUE" : "FALSE");
                        }

                        else
                        {
                            ::g_bFriendlyDecrease = jsTree["Friendly Decrease"].get < bool >();
                            {
                                ::std::printf("'Friendly Decrease' Is Now '%s'\n", ::g_bFriendlyDecrease ? "TRUE" : "FALSE");
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

                        if (jsTree["Tick Rate"].empty())
                        {
                            ::std::printf("'Tick Rate' Is Now '%f' (~ %f Interval Per Tick)\n", 1.f / ::gpGlobals->interval_per_tick, ::gpGlobals->interval_per_tick);
                        }

                        else if (!jsTree["Tick Rate"].is_number())
                        {
                            ::std::printf("'Tick Rate' Is Now '%f' (~ %f Interval Per Tick)\n", 1.f / ::gpGlobals->interval_per_tick, ::gpGlobals->interval_per_tick);
                        }

                        else if ((fTickRate = jsTree["Tick Rate"].get < float >()) <= 0.f)
                        {
                            ::std::printf("'Tick Rate' Is Now '%f' (~ %f Interval Per Tick)\n", 1.f / ::gpGlobals->interval_per_tick, ::gpGlobals->interval_per_tick);
                        }

                        else
                        {
                            ::g_fIntervalPerTick = 1.f / fTickRate;

                            ::std::printf("'Tick Rate' Is Now '%f' (~ %f Interval Per Tick)\n", fTickRate, ::g_fIntervalPerTick);
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

    if (!::g_bNoReturn)
    {
        if (::g_fIntervalPerTick)
        {
            do
            {
                ::g_SHPtr->SetRes(::MRES_SUPERCEDE);

                return ::g_fIntervalPerTick;
            }

            while (false);
        }

        else
        {
            do
            {
                ::g_SHPtr->SetRes(::MRES_IGNORED);

                if (::g_SHPtr->GetOrigRet())
                {
                    return (*(float*) ::g_SHPtr->GetOrigRet());
                }

                return { };
            }

            while (false);
        }
    }

    else
    {
        return { };
    }

    return { };
}

bool ::MySmmPlugin::Hook_LevelInit(const char*, const char*, const char*, const char*, bool, bool) noexcept
{
    ::g_llMapChange = ::std::time(nullptr);

    if (!::g_bNoReturn)
    {
        do
        {
            ::g_SHPtr->SetRes(::MRES_IGNORED);

            if (::g_SHPtr->GetOrigRet())
            {
                return (*(bool*) ::g_SHPtr->GetOrigRet());
            }

            return { };
        }

        while (false);
    }

    else
    {
        return { };
    }

    return { };
}

void ::MySmmPlugin::Hook_LevelShutdown() noexcept
{
    static int nPlayer{ };

    static ::edict_t* pEdict{ };
    static ::CBaseEntity* pBaseEntity{ };
    static ::IServerUnknown* pServerUnknown{ };
    static ::IServerEntity* pServerEntity{ };
    static ::IPlayerInfo* pPlayerInfo{ };

    ::g_llMapChange = ::std::time(nullptr);

    if (!::g_bNoReturn)
    {
        ::g_bNoReturn = true;

        {
            for (nPlayer = xTo(true, int); nPlayer <= ::gpGlobals->maxClients; nPlayer++)
            {

#if SOURCE_ENGINE != SE_CSGO

                if (pEdict = ::engine->PEntityOfEntIndex(nPlayer))

#else

                if (pEdict = ::gpGlobals->pEdicts + nPlayer)

#endif

                {
                    if (!(pEdict->m_fStateFlags & FL_EDICT_FREE))
                    {
                        if (!(pEdict->m_fStateFlags & FL_EDICT_DONTSEND))
                        {
                            if (pEdict->m_NetworkSerialNumber)
                            {
                                if (pEdict->m_pNetworkable)
                                {
                                    if (pServerEntity = pEdict->GetIServerEntity())
                                    {
                                        if (pServerUnknown = pEdict->GetUnknown())
                                        {
                                            if (pBaseEntity = pServerEntity->GetBaseEntity())
                                            {
                                                if (pPlayerInfo = ::playerinfomanager->GetPlayerInfo(pEdict))
                                                {
                                                    if (pPlayerInfo->IsPlayer())
                                                    {
                                                        Hook_ClientDisconnect(pEdict);
                                                        {
                                                            Hook_ClientDisconnect_Post(pEdict);
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

        ::g_bNoReturn = { };
    }

    if (!::g_bNoReturn)
    {
        do
        {
            ::g_SHPtr->SetRes(::MRES_IGNORED);

            return;
        }

        while (false);
    }

    else
    {
        return;
    }
}

bool ::MySmmPlugin::Hook_ClientConnect(::edict_t* pEdict, const char*, const char*, char*, int) noexcept
{
    static int nPlayer{ };

#if SOURCE_ENGINE != SE_CSGO

    static long long llNow{ };

    static ::std::string strCmd{ };

#endif

    if (pEdict)
    {

#if SOURCE_ENGINE != SE_CSGO

        if ((nPlayer = ::engine->IndexOfEdict(pEdict)) > 0)

#else

        if ((nPlayer = pEdict - ::gpGlobals->pEdicts) > 0)

#endif

        {
            if (::g_nArmor)
            {
                ::g_bArmor[nPlayer] = { };
            }

            if (::g_bRealScore)
            {
                ::g_nDeaths[nPlayer] = { };
                {
                    ::g_nKills[nPlayer] = { };
                }
            }

            if (!::g_strWelcome_1.empty())
            {
                if (::g_strWelcome_1.length() <= 190)
                {
                    if (::g_strWelcome_1.size() <= 190)
                    {
                        if (::g_nTextMsg > 0)
                        {
                            ::g_bShow[nPlayer] = { };
                        }
                    }
                }
            }

#if SOURCE_ENGINE != SE_CSGO

            if (::gpGlobals->mapname.ToCStr())
            {
                if (::g_llMapChange)
                {
                    if (!::numUsersConnected(nPlayer))
                    {
                        if (((llNow = ::std::time(nullptr)) - ::g_llMapChange) >= 60)
                        {
                            strCmd.assign("changelevel");
                            {
                                strCmd.append(" ");
                                {
                                    strCmd.append("\"");
                                    {
                                        strCmd.append(::gpGlobals->mapname.ToCStr());
                                        {
                                            strCmd.append("\"");
                                            {
                                                strCmd.append("\n");
                                                {
                                                    strCmd.shrink_to_fit();
                                                    {
                                                        ::engine->ServerCommand(strCmd.c_str());
                                                        {
                                                            strCmd.clear();
                                                            {
                                                                strCmd.shrink_to_fit();
                                                                {
                                                                    ::g_llMapChange = llNow;
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

#endif

        }
    }

    if (!::g_bNoReturn)
    {
        do
        {
            ::g_SHPtr->SetRes(::MRES_IGNORED);

            if (::g_SHPtr->GetOrigRet())
            {
                return (*(bool*) ::g_SHPtr->GetOrigRet());
            }

            return { };
        }

        while (false);
    }

    else
    {
        return { };
    }

    return { };
}

void ::MySmmPlugin::Hook_ClientDisconnect(::edict_t* pEdict) noexcept
{
    static int nPlayer{ };

    if (pEdict)
    {

#if SOURCE_ENGINE != SE_CSGO

        if ((nPlayer = ::engine->IndexOfEdict(pEdict)) > 0)

#else

        if ((nPlayer = pEdict - ::gpGlobals->pEdicts) > 0)

#endif

        {
            if (::g_nArmor)
            {
                ::g_bArmor[nPlayer] = { };
            }

            if (::g_bRealScore)
            {
                ::g_nDeaths[nPlayer] = { };
                {
                    ::g_nKills[nPlayer] = { };
                }
            }

            if (!::g_strWelcome_1.empty())
            {
                if (::g_strWelcome_1.length() <= 190)
                {
                    if (::g_strWelcome_1.size() <= 190)
                    {
                        if (::g_nTextMsg > 0)
                        {
                            ::g_bShow[nPlayer] = { };
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
            ::g_SHPtr->SetRes(::MRES_IGNORED);

            return;
        }

        while (false);
    }

    else
    {
        return;
    }
}

void ::MySmmPlugin::Hook_ServerActivate_Post(::edict_t*, int, int) noexcept
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

#if SOURCE_ENGINE == SE_CSGO

    static ::std::string strHelp{ }, strDef{ }, strName{ };
    static ::std::vector < ::CustomConVar > vecCustomConVars{ };

    static ::FILE* pFile{ };
    static ::ConCommandBase* pConCommandBase{ };

#ifndef WIN32

    static ::CustomConVar customConVar{ };

#endif

    static unsigned int uPos{ };
    static int nSize{ }, nFlags{ };
    static const char* pszName{ };
    static bool bDump{ };

#endif

    static ::nlohmann::json jsTree{ };
    static ::std::string strPath{ }, strConVar{ }, strCmd{ }, strVal{ };
    static ::std::ifstream inFile{ };

    static ::ConVar* pConVar{ }, * pCfgConVar{ };
    static ::FILE* pISys{ };

    static int nIter{ }, nLen{ };
    static float fMin{ }, fMax{ };
    static bool bHasMin{ }, bHasMax{ }, bFlags{ }, bMsgs{ };
    static const char* pszVal{ }, * pszHelp{ }, * pszDef{ };
    static char szBuffer[2048]{ }, szTrimmed[2048]{ }, szSys[256]{ };

    ::g_llMapChange = ::std::time(nullptr);

#if SOURCE_ENGINE == SE_CSGO

    {
        if (!::g_bNoReturn)
        {
            ::g_bNoReturn = true;

            {
                Hook_GetTickInterval();
                {
                    Hook_GetTickInterval_Post();
                }
            }

            ::g_bNoReturn = { };
        }
    }

#endif

    if (!bMsgs)
    {
        if (!::g_strWelcome_1.empty())
        {
            if (::g_strWelcome_1.length() <= 190)
            {
                if (::g_strWelcome_1.size() <= 190)
                {
                    if (!::g_nTextMsg)
                    {

#if SOURCE_ENGINE == SE_CSGO

                        if ((::g_nTextMsg = ::g_Cstrike15UsermessageHelpers.GetIndex("TextMsg")) < 1)
                        {
                            if ((::g_nTextMsg = ::g_SMAPI->FindUserMessage("TextMsg", nullptr)) > 0)
                            {
                                if (!(::g_pTextMsg = ::g_Cstrike15UsermessageHelpers.GetPrototype(::g_nTextMsg)))
                                {
                                    ::g_nTextMsg = { };
                                }
                            }
                        }

                        else
                        {
                            if (!(::g_pTextMsg = ::g_Cstrike15UsermessageHelpers.GetPrototype(::g_nTextMsg)))
                            {
                                ::g_nTextMsg = { };
                            }
                        }

#else

                        ::g_nTextMsg = ::g_SMAPI->FindUserMessage("TextMsg", nullptr);

#endif

                    }
                }
            }
        }

        bMsgs = true;
    }

    if (::g_bHooked)
    {
        if (!::g_bEvHooked)
        {
            if (::gameevents->AddListener(this, "player_spawn", true))
            {
                ::gameevents->AddListener(this, "player_death", true);
                {
                    ::g_bEvHooked = true;
                }
            }
        }
    }

    ::g_SMAPI->Format(szSys, sizeof szSys, "%s/addons/hgsm/sys.cfg", ::g_strGameDir.c_str());

#ifndef WIN32

    ::g_SMAPI->Format(szCpu, sizeof szCpu, "%s/addons/hgsm/cpu.txt", ::g_strGameDir.c_str());
    ::g_SMAPI->Format(szMem, sizeof szMem, "%s/addons/hgsm/mem.txt", ::g_strGameDir.c_str());

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

    if (!pCfgConVar)
    {
        for (auto& cvItem : arConVars)
        {
            if (pCfgConVar = ::icvar->FindVar(cvItem))
            {
                if (pszVal = pCfgConVar->GetString())
                {
                    if (*pszVal)
                    {
                        break;
                    }
                }
            }
        }
    }

    if (pCfgConVar)
    {
        if (pszVal = pCfgConVar->GetString())
        {
            if (*pszVal)
            {
                strVal.assign(pszVal);

                strPath.assign(::g_strGameDir + "/cfg/" + strVal + ".cfg");

                if (::FileExists(strPath))
                {
                    strCmd.assign("exec \"" + strVal + ".cfg\"\n");
                    {
                        ::engine->ServerCommand(strCmd.c_str());
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

#if SOURCE_ENGINE == SE_CSGO

    if (!bDump)
    {
        if (::FileExists(strPath = ::g_strGameDir + "/addons/hgsm/console_variables.txt"))
        {
            if (::FileRemove(strPath))
            {
                ::std::printf("Deleted '%s'\n", strPath.c_str());
            }

            else
            {
                ::std::printf("Failed to delete '%s'\n", strPath.c_str());
            }
        }

        if (!::FileExists(strPath))
        {
            if (pFile = ::std::fopen(strPath.c_str(), "w"))
            {
                ::ICvar::Iterator cvIter(::g_pCVar);
                {
                    vecCustomConVars.reserve(xTo(xConVarsTotalRounded, unsigned int));
                    {
                        ::std::fprintf(pFile, "%-64s | %-16s | %-16s | %-14s | %-14s | %s\n\n", "Console Variable Name", "Actual Value", "Default Value", "Minimum", "Maximum", "Help");
                    }
                }

                for (cvIter.SetFirst(); cvIter.IsValid(); cvIter.Next())
                {
                    if (pConCommandBase = cvIter.Get())
                    {
                        if (!pConCommandBase->IsCommand())
                        {
                            if (pConVar = (static_cast <::ConVar*> (pConCommandBase)))
                            {
                                pszName = pConVar->GetName();
                                pszHelp = pConVar->GetHelpText();
                                pszVal = pConVar->GetString();
                                pszDef = pConVar->GetDefault();

                                if (!pszName)
                                {
                                    pszName = "";
                                }

                                if (!pszHelp)
                                {
                                    pszHelp = "";
                                }

                                if (!pszVal)
                                {
                                    pszVal = "";
                                }

                                if (!pszDef)
                                {
                                    pszDef = "";
                                }

                                strHelp = pszHelp;
                                {
                                    while ((uPos = strHelp.find('\n')) != ::std::string::npos)
                                    {
                                        strHelp[uPos] = ' ';
                                    }

                                    while ((uPos = strHelp.find('\r')) != ::std::string::npos)
                                    {
                                        strHelp[uPos] = ' ';
                                    }

                                    while ((uPos = strHelp.find('\t')) != ::std::string::npos)
                                    {
                                        strHelp[uPos] = ' ';
                                    }

                                    while ((uPos = strHelp.find('\f')) != ::std::string::npos)
                                    {
                                        strHelp[uPos] = ' ';
                                    }

                                    while ((uPos = strHelp.find('\v')) != ::std::string::npos)
                                    {
                                        strHelp[uPos] = ' ';
                                    }

                                    while ((uPos = strHelp.find('\a')) != ::std::string::npos)
                                    {
                                        strHelp[uPos] = ' ';
                                    }

                                    while ((uPos = strHelp.find('\b')) != ::std::string::npos)
                                    {
                                        strHelp[uPos] = ' ';
                                    }

                                    /*

                                    uPos = { };
                                    {
                                        while ((uPos = strHelp.find('%', uPos)) != ::std::string::npos)
                                        {
                                            if (!uPos)
                                            {
                                                if (strHelp[uPos + 1] != '%')
                                                {
                                                    strHelp.insert(uPos, "%");
                                                    {
                                                        uPos += xTo(2, unsigned int);
                                                    }
                                                }
                                            }

                                            else
                                            {
                                                if (strHelp[uPos + 1] != '%')
                                                {
                                                    if (strHelp[uPos - 1] != '%')
                                                    {
                                                        strHelp.insert(uPos, "%");
                                                        {
                                                            uPos += xTo(2, unsigned int);
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }

                                    */

                                    while ((uPos = strHelp.find("  ")) != ::std::string::npos)
                                    {
                                        strHelp.replace(uPos, xTo(2, unsigned int), " ");
                                    }
                                }

                                strVal = pszVal;
                                {
                                    while ((uPos = strVal.find('\n')) != ::std::string::npos)
                                    {
                                        strVal[uPos] = ' ';
                                    }

                                    while ((uPos = strVal.find('\r')) != ::std::string::npos)
                                    {
                                        strVal[uPos] = ' ';
                                    }

                                    while ((uPos = strVal.find('\t')) != ::std::string::npos)
                                    {
                                        strVal[uPos] = ' ';
                                    }

                                    while ((uPos = strVal.find('\f')) != ::std::string::npos)
                                    {
                                        strVal[uPos] = ' ';
                                    }

                                    while ((uPos = strVal.find('\v')) != ::std::string::npos)
                                    {
                                        strVal[uPos] = ' ';
                                    }

                                    while ((uPos = strVal.find('\a')) != ::std::string::npos)
                                    {
                                        strVal[uPos] = ' ';
                                    }

                                    while ((uPos = strVal.find('\b')) != ::std::string::npos)
                                    {
                                        strVal[uPos] = ' ';
                                    }

                                    /*

                                    uPos = { };
                                    {
                                        while ((uPos = strVal.find('%', uPos)) != ::std::string::npos)
                                        {
                                            if (!uPos)
                                            {
                                                if (strVal[uPos + 1] != '%')
                                                {
                                                    strVal.insert(uPos, "%");
                                                    {
                                                        uPos += xTo(2, unsigned int);
                                                    }
                                                }
                                            }

                                            else
                                            {
                                                if (strVal[uPos + 1] != '%')
                                                {
                                                    if (strVal[uPos - 1] != '%')
                                                    {
                                                        strVal.insert(uPos, "%");
                                                        {
                                                            uPos += xTo(2, unsigned int);
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }

                                    */

                                    while ((uPos = strVal.find("  ")) != ::std::string::npos)
                                    {
                                        strVal.replace(uPos, xTo(2, unsigned int), " ");
                                    }

                                    if (strVal.length() > 16)
                                    {
                                        strVal[::SafeErasePos(strVal, 16)] = { };
                                        {
                                            strVal.erase(::SafeErasePos(strVal, 16));
                                            {
                                                strVal.shrink_to_fit();
                                            }
                                        }
                                    }

                                    else if (strVal.size() > 16)
                                    {
                                        strVal[::SafeErasePos(strVal, 16)] = { };
                                        {
                                            strVal.erase(::SafeErasePos(strVal, 16));
                                            {
                                                strVal.shrink_to_fit();
                                            }
                                        }
                                    }

                                    else
                                    {

                                    }
                                }

                                strDef = pszDef;
                                {
                                    while ((uPos = strDef.find('\n')) != ::std::string::npos)
                                    {
                                        strDef[uPos] = ' ';
                                    }

                                    while ((uPos = strDef.find('\r')) != ::std::string::npos)
                                    {
                                        strDef[uPos] = ' ';
                                    }

                                    while ((uPos = strDef.find('\t')) != ::std::string::npos)
                                    {
                                        strDef[uPos] = ' ';
                                    }

                                    while ((uPos = strDef.find('\f')) != ::std::string::npos)
                                    {
                                        strDef[uPos] = ' ';
                                    }

                                    while ((uPos = strDef.find('\v')) != ::std::string::npos)
                                    {
                                        strDef[uPos] = ' ';
                                    }

                                    while ((uPos = strDef.find('\a')) != ::std::string::npos)
                                    {
                                        strDef[uPos] = ' ';
                                    }

                                    while ((uPos = strDef.find('\b')) != ::std::string::npos)
                                    {
                                        strDef[uPos] = ' ';
                                    }

                                    /*

                                    uPos = { };
                                    {
                                        while ((uPos = strDef.find('%', uPos)) != ::std::string::npos)
                                        {
                                            if (!uPos)
                                            {
                                                if (strDef[uPos + 1] != '%')
                                                {
                                                    strDef.insert(uPos, "%");
                                                    {
                                                        uPos += xTo(2, unsigned int);
                                                    }
                                                }
                                            }

                                            else
                                            {
                                                if (strDef[uPos + 1] != '%')
                                                {
                                                    if (strDef[uPos - 1] != '%')
                                                    {
                                                        strDef.insert(uPos, "%");
                                                        {
                                                            uPos += xTo(2, unsigned int);
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }

                                    */

                                    while ((uPos = strDef.find("  ")) != ::std::string::npos)
                                    {
                                        strDef.replace(uPos, xTo(2, unsigned int), " ");
                                    }

                                    if (strDef.length() > 16)
                                    {
                                        strDef[::SafeErasePos(strDef, 16)] = { };
                                        {
                                            strDef.erase(::SafeErasePos(strDef, 16));
                                            {
                                                strDef.shrink_to_fit();
                                            }
                                        }
                                    }

                                    else if (strDef.size() > 16)
                                    {
                                        strDef[::SafeErasePos(strDef, 16)] = { };
                                        {
                                            strDef.erase(::SafeErasePos(strDef, 16));
                                            {
                                                strDef.shrink_to_fit();
                                            }
                                        }
                                    }

                                    else
                                    {

                                    }
                                }

                                strName = pszName;
                                {
                                    while ((uPos = strName.find('\n')) != ::std::string::npos)
                                    {
                                        strName[uPos] = ' ';
                                    }

                                    while ((uPos = strName.find('\r')) != ::std::string::npos)
                                    {
                                        strName[uPos] = ' ';
                                    }

                                    while ((uPos = strName.find('\t')) != ::std::string::npos)
                                    {
                                        strName[uPos] = ' ';
                                    }

                                    while ((uPos = strName.find('\f')) != ::std::string::npos)
                                    {
                                        strName[uPos] = ' ';
                                    }

                                    while ((uPos = strName.find('\v')) != ::std::string::npos)
                                    {
                                        strName[uPos] = ' ';
                                    }

                                    while ((uPos = strName.find('\a')) != ::std::string::npos)
                                    {
                                        strName[uPos] = ' ';
                                    }

                                    while ((uPos = strName.find('\b')) != ::std::string::npos)
                                    {
                                        strName[uPos] = ' ';
                                    }

                                    /*

                                    uPos = { };
                                    {
                                        while ((uPos = strName.find('%', uPos)) != ::std::string::npos)
                                        {
                                            if (!uPos)
                                            {
                                                if (strName[uPos + 1] != '%')
                                                {
                                                    strName.insert(uPos, "%");
                                                    {
                                                        uPos += xTo(2, unsigned int);
                                                    }
                                                }
                                            }

                                            else
                                            {
                                                if (strName[uPos + 1] != '%')
                                                {
                                                    if (strName[uPos - 1] != '%')
                                                    {
                                                        strName.insert(uPos, "%");
                                                        {
                                                            uPos += xTo(2, unsigned int);
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }

                                    */

                                    while ((uPos = strName.find("  ")) != ::std::string::npos)
                                    {
                                        strName.replace(uPos, xTo(2, unsigned int), " ");
                                    }

                                    if (strName.length() > 64)
                                    {
                                        strName[::SafeErasePos(strName, 64)] = { };
                                        {
                                            strName.erase(::SafeErasePos(strName, 64));
                                            {
                                                strName.shrink_to_fit();
                                            }
                                        }
                                    }

                                    else if (strName.size() > 64)
                                    {
                                        strName[::SafeErasePos(strName, 64)] = { };
                                        {
                                            strName.erase(::SafeErasePos(strName, 64));
                                            {
                                                strName.shrink_to_fit();
                                            }
                                        }
                                    }

                                    else
                                    {

                                    }
                                }

                                bHasMin = pConVar->GetMin(fMin);
                                bHasMax = pConVar->GetMax(fMax);

#ifdef WIN32

                                vecCustomConVars.push_back({ strName, strVal, strDef, bHasMin ? ((::std::to_string(fMin)).c_str()) : "", bHasMax ? ((::std::to_string(fMax)).c_str()) : "", strHelp, });

#else

                                customConVar.strName = strName;
                                {
                                    customConVar.strVal = strVal;
                                    {
                                        customConVar.strDef = strDef;
                                        {
                                            customConVar.strMin = bHasMin ? ((::std::to_string(fMin)).c_str()) : "";
                                            {
                                                customConVar.strMax = bHasMax ? ((::std::to_string(fMax)).c_str()) : "";
                                                {
                                                    customConVar.strHelp = strHelp;
                                                    {
                                                        vecCustomConVars.push_back(customConVar);
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }

#endif

                                {
                                    ::std::fprintf(pFile, "%-64s | %-16s | %-16s | %-14s | %-14s | %s\n", strName.c_str(), strVal.c_str(), strDef.c_str(), vecCustomConVars.back().strMin.c_str(), vecCustomConVars.back().strMax.c_str(), strHelp.c_str());
                                }
                            }
                        }
                    }
                }

                ::std::fclose(pFile);
                {
                    ::std::printf("Created '%s'\n", strPath.c_str());
                    {
                        ::std::printf("Total Console Variables '%d'\n", vecCustomConVars.size());
                        {
                            vecCustomConVars.shrink_to_fit();
                        }
                    }
                }
            }

            else
            {
                ::std::printf("Failed to create '%s'\n", strPath.c_str());
            }
        }

        else
        {
            ::std::printf("Failed to create '%s'\n", strPath.c_str());
        }

        if (::FileExists(strPath = ::g_strGameDir + "/addons/hgsm/console_variables_sorted.txt"))
        {
            if (::FileRemove(strPath))
            {
                ::std::printf("Deleted '%s'\n", strPath.c_str());
            }

            else
            {
                ::std::printf("Failed to delete '%s'\n", strPath.c_str());
            }
        }

        if (!::FileExists(strPath))
        {
            if (pFile = ::std::fopen(strPath.c_str(), "w"))
            {
                ::std::fprintf(pFile, "%-64s | %-16s | %-16s | %-14s | %-14s | %s\n\n", "Console Variable Name", "Actual Value", "Default Value", "Minimum", "Maximum", "Help");
                {

#ifdef WIN32

                    ::std::ranges::sort(vecCustomConVars, { }, &::CustomConVar::strName);

#else

                    ::std::sort(vecCustomConVars.begin(), vecCustomConVars.end());

#endif

                    {
                        for (auto& vecItem : vecCustomConVars)
                        {
                            ::std::fprintf(pFile, "%-64s | %-16s | %-16s | %-14s | %-14s | %s\n", vecItem.strName.c_str(), vecItem.strVal.c_str(), vecItem.strDef.c_str(), vecItem.strMin.c_str(), vecItem.strMax.c_str(), vecItem.strHelp.c_str());
                        }
                    }
                }

                ::std::fclose(pFile);
                {
                    ::std::printf("Created '%s'\n", strPath.c_str());
                }
            }

            else
            {
                ::std::printf("Failed to create '%s'\n", strPath.c_str());
            }
        }

        else
        {
            ::std::printf("Failed to create '%s'\n", strPath.c_str());
        }

        vecCustomConVars.clear();
        {
            vecCustomConVars.shrink_to_fit();
            {
                bDump = true;
            }
        }
    }

#endif

    if (!::FileExists(strPath = ::g_strGameDir + "/addons/hgsm/reveal_console_variables.cfg"))
    {
        ::std::printf("Failed To Find '%s'\n", strPath.c_str());
    }

    else
    {
        inFile.open(strPath);

        if (!inFile.is_open())
        {
            ::std::printf("Failed To Open '%s'\n", strPath.c_str());
            {
                inFile.clear();
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
                ::std::printf("'%s' Has INVALID 'JSON'\n", strPath.c_str());
                {
                    jsTree.clear();
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
                                            if (pConVar = ::icvar->FindVar(strConVar.c_str()))
                                            {
                                                ::std::printf("'%s' Has Value '%s' (~ %f)\n", strConVar.c_str(), pConVar->GetString(), pConVar->GetFloat());

                                                if (bHasMin = pConVar->GetMin(fMin))
                                                {
                                                    ::std::printf("'%s' Has Min '%f'\n", strConVar.c_str(), fMin);
                                                }

                                                else
                                                {
                                                    ::std::printf("'%s' Has No Min\n", strConVar.c_str());
                                                }

                                                if (bHasMax = pConVar->GetMax(fMax))
                                                {
                                                    ::std::printf("'%s' Has Max '%f'\n", strConVar.c_str(), fMax);
                                                }

                                                else
                                                {
                                                    ::std::printf("'%s' Has No Max\n", strConVar.c_str());
                                                }

#if SOURCE_ENGINE != SE_CSGO

                                                bFlags = { };

                                                if (pConVar->IsFlagSet(FCVAR_CHEAT))
                                                {
                                                    ::std::printf("'%s' Has 'FCVAR_CHEAT'\n", strConVar.c_str());

                                                    bFlags = true;
                                                }

                                                if (pConVar->IsFlagSet(FCVAR_HIDDEN))
                                                {
                                                    ::std::printf("'%s' Has 'FCVAR_HIDDEN'\n", strConVar.c_str());

                                                    bFlags = true;
                                                }

                                                if (pConVar->IsFlagSet(FCVAR_REPLICATED))
                                                {
                                                    ::std::printf("'%s' Has 'FCVAR_REPLICATED'\n", strConVar.c_str());

                                                    bFlags = true;
                                                }

                                                if (pConVar->IsFlagSet(FCVAR_CLIENTDLL))
                                                {
                                                    ::std::printf("'%s' Has 'FCVAR_CLIENTDLL'\n", strConVar.c_str());

                                                    bFlags = true;
                                                }

                                                if (pConVar->IsFlagSet(FCVAR_GAMEDLL))
                                                {
                                                    ::std::printf("'%s' Has 'FCVAR_GAMEDLL'\n", strConVar.c_str());

                                                    bFlags = true;
                                                }

                                                if (pConVar->IsFlagSet(FCVAR_NOTIFY))
                                                {
                                                    ::std::printf("'%s' Has 'FCVAR_NOTIFY'\n", strConVar.c_str());

                                                    bFlags = true;
                                                }

                                                if (pConVar->IsFlagSet(FCVAR_DEVELOPMENTONLY))
                                                {
                                                    ::std::printf("'%s' Has 'FCVAR_DEVELOPMENTONLY'\n", strConVar.c_str());

                                                    bFlags = true;
                                                }

                                                if (pConVar->IsFlagSet(FCVAR_PROTECTED))
                                                {
                                                    ::std::printf("'%s' Has 'FCVAR_PROTECTED'\n", strConVar.c_str());

                                                    bFlags = true;
                                                }

                                                if (pConVar->IsFlagSet(FCVAR_ARCHIVE))
                                                {
                                                    ::std::printf("'%s' Has 'FCVAR_ARCHIVE'\n", strConVar.c_str());

                                                    bFlags = true;
                                                }

                                                if (pConVar->IsFlagSet(FCVAR_CLIENTCMD_CAN_EXECUTE))
                                                {
                                                    ::std::printf("'%s' Has 'FCVAR_CLIENTCMD_CAN_EXECUTE'\n", strConVar.c_str());

                                                    bFlags = true;
                                                }

                                                if (pConVar->IsFlagSet(FCVAR_DEMO))
                                                {
                                                    ::std::printf("'%s' Has 'FCVAR_DEMO'\n", strConVar.c_str());

                                                    bFlags = true;
                                                }

                                                if (pConVar->IsFlagSet(FCVAR_DONTRECORD))
                                                {
                                                    ::std::printf("'%s' Has 'FCVAR_DONTRECORD'\n", strConVar.c_str());

                                                    bFlags = true;
                                                }

                                                if (pConVar->IsFlagSet(FCVAR_NOT_CONNECTED))
                                                {
                                                    ::std::printf("'%s' Has 'FCVAR_NOT_CONNECTED'\n", strConVar.c_str());

                                                    bFlags = true;
                                                }

                                                if (pConVar->IsFlagSet(FCVAR_PRINTABLEONLY))
                                                {
                                                    ::std::printf("'%s' Has 'FCVAR_PRINTABLEONLY'\n", strConVar.c_str());

                                                    bFlags = true;
                                                }

                                                if (pConVar->IsFlagSet(FCVAR_SERVER_CANNOT_QUERY))
                                                {
                                                    ::std::printf("'%s' Has 'FCVAR_SERVER_CANNOT_QUERY'\n", strConVar.c_str());

                                                    bFlags = true;
                                                }

                                                if (pConVar->IsFlagSet(FCVAR_SERVER_CAN_EXECUTE))
                                                {
                                                    ::std::printf("'%s' Has 'FCVAR_SERVER_CAN_EXECUTE'\n", strConVar.c_str());

                                                    bFlags = true;
                                                }

                                                if (pConVar->IsFlagSet(FCVAR_SPONLY))
                                                {
                                                    ::std::printf("'%s' Has 'FCVAR_SPONLY'\n", strConVar.c_str());

                                                    bFlags = true;
                                                }

                                                if (pConVar->IsFlagSet(FCVAR_UNLOGGED))
                                                {
                                                    ::std::printf("'%s' Has 'FCVAR_UNLOGGED'\n", strConVar.c_str());

                                                    bFlags = true;
                                                }

                                                if (pConVar->IsFlagSet(FCVAR_UNREGISTERED))
                                                {
                                                    ::std::printf("'%s' Has 'FCVAR_UNREGISTERED'\n", strConVar.c_str());

                                                    bFlags = true;
                                                }

                                                if (pConVar->IsFlagSet(FCVAR_USERINFO))
                                                {
                                                    ::std::printf("'%s' Has 'FCVAR_USERINFO'\n", strConVar.c_str());

                                                    bFlags = true;
                                                }

                                                if (!bFlags)
                                                {
                                                    ::std::printf("'%s' Has No Flags Or Has Useless Flags\n", strConVar.c_str());
                                                }

#else

                                                if (nFlags = pConVar->GetFlags())
                                                {
                                                    bFlags = { };

                                                    if (nFlags & FCVAR_CHEAT)
                                                    {
                                                        ::std::printf("'%s' Has 'FCVAR_CHEAT'\n", strConVar.c_str());

                                                        bFlags = true;
                                                    }

                                                    if (nFlags & FCVAR_HIDDEN)
                                                    {
                                                        ::std::printf("'%s' Has 'FCVAR_HIDDEN'\n", strConVar.c_str());

                                                        bFlags = true;
                                                    }

                                                    if (nFlags & FCVAR_REPLICATED)
                                                    {
                                                        ::std::printf("'%s' Has 'FCVAR_REPLICATED'\n", strConVar.c_str());

                                                        bFlags = true;
                                                    }

                                                    if (nFlags & FCVAR_CLIENTDLL)
                                                    {
                                                        ::std::printf("'%s' Has 'FCVAR_CLIENTDLL'\n", strConVar.c_str());

                                                        bFlags = true;
                                                    }

                                                    if (nFlags & FCVAR_GAMEDLL)
                                                    {
                                                        ::std::printf("'%s' Has 'FCVAR_GAMEDLL'\n", strConVar.c_str());

                                                        bFlags = true;
                                                    }

                                                    if (nFlags & FCVAR_NOTIFY)
                                                    {
                                                        ::std::printf("'%s' Has 'FCVAR_NOTIFY'\n", strConVar.c_str());

                                                        bFlags = true;
                                                    }

                                                    if (nFlags & FCVAR_DEVELOPMENTONLY)
                                                    {
                                                        ::std::printf("'%s' Has 'FCVAR_DEVELOPMENTONLY'\n", strConVar.c_str());

                                                        bFlags = true;
                                                    }

                                                    if (nFlags & FCVAR_PROTECTED)
                                                    {
                                                        ::std::printf("'%s' Has 'FCVAR_PROTECTED'\n", strConVar.c_str());

                                                        bFlags = true;
                                                    }

                                                    if (nFlags & FCVAR_ARCHIVE)
                                                    {
                                                        ::std::printf("'%s' Has 'FCVAR_ARCHIVE'\n", strConVar.c_str());

                                                        bFlags = true;
                                                    }

                                                    if (nFlags & FCVAR_CLIENTCMD_CAN_EXECUTE)
                                                    {
                                                        ::std::printf("'%s' Has 'FCVAR_CLIENTCMD_CAN_EXECUTE'\n", strConVar.c_str());

                                                        bFlags = true;
                                                    }

                                                    if (nFlags & FCVAR_DEMO)
                                                    {
                                                        ::std::printf("'%s' Has 'FCVAR_DEMO'\n", strConVar.c_str());

                                                        bFlags = true;
                                                    }

                                                    if (nFlags & FCVAR_DONTRECORD)
                                                    {
                                                        ::std::printf("'%s' Has 'FCVAR_DONTRECORD'\n", strConVar.c_str());

                                                        bFlags = true;
                                                    }

                                                    if (nFlags & FCVAR_NOT_CONNECTED)
                                                    {
                                                        ::std::printf("'%s' Has 'FCVAR_NOT_CONNECTED'\n", strConVar.c_str());

                                                        bFlags = true;
                                                    }

                                                    if (nFlags & FCVAR_PRINTABLEONLY)
                                                    {
                                                        ::std::printf("'%s' Has 'FCVAR_PRINTABLEONLY'\n", strConVar.c_str());

                                                        bFlags = true;
                                                    }

                                                    if (nFlags & FCVAR_RELEASE)
                                                    {
                                                        ::std::printf("'%s' Has 'FCVAR_RELEASE'\n", strConVar.c_str());

                                                        bFlags = true;
                                                    }

                                                    if (nFlags & FCVAR_SERVER_CANNOT_QUERY)
                                                    {
                                                        ::std::printf("'%s' Has 'FCVAR_SERVER_CANNOT_QUERY'\n", strConVar.c_str());

                                                        bFlags = true;
                                                    }

                                                    if (nFlags & FCVAR_SERVER_CAN_EXECUTE)
                                                    {
                                                        ::std::printf("'%s' Has 'FCVAR_SERVER_CAN_EXECUTE'\n", strConVar.c_str());

                                                        bFlags = true;
                                                    }

                                                    if (nFlags & FCVAR_SPONLY)
                                                    {
                                                        ::std::printf("'%s' Has 'FCVAR_SPONLY'\n", strConVar.c_str());

                                                        bFlags = true;
                                                    }

                                                    if (nFlags & FCVAR_UNLOGGED)
                                                    {
                                                        ::std::printf("'%s' Has 'FCVAR_UNLOGGED'\n", strConVar.c_str());

                                                        bFlags = true;
                                                    }

                                                    if (nFlags & FCVAR_UNREGISTERED)
                                                    {
                                                        ::std::printf("'%s' Has 'FCVAR_UNREGISTERED'\n", strConVar.c_str());

                                                        bFlags = true;
                                                    }

                                                    if (nFlags & FCVAR_USERINFO)
                                                    {
                                                        ::std::printf("'%s' Has 'FCVAR_USERINFO'\n", strConVar.c_str());

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

                                                if (nSize = pConVar->GetChangeCallbackCount())
                                                {
                                                    ::std::printf("'%s' Has Change Callbacks '%d'\n", strConVar.c_str(), nSize);
                                                }

                                                else
                                                {
                                                    ::std::printf("'%s' Has No Change Callbacks\n", strConVar.c_str());
                                                }

#endif

                                                if (pszDef = pConVar->GetDefault())
                                                {
                                                    if (*pszDef)
                                                    {
                                                        ::std::printf("'%s' Has Default '%s'\n", strConVar.c_str(), pszDef);
                                                    }

                                                    else
                                                    {
                                                        ::std::printf("'%s' Has No Default\n", strConVar.c_str());
                                                    }
                                                }

                                                else
                                                {
                                                    ::std::printf("'%s' Has No Default\n", strConVar.c_str());
                                                }

                                                if (pszHelp = pConVar->GetHelpText())
                                                {
                                                    if (*pszHelp)
                                                    {
                                                        ::std::printf("'%s' Has Help '%s'\n", strConVar.c_str(), pszHelp);
                                                    }

                                                    else
                                                    {
                                                        ::std::printf("'%s' Has No Help\n", strConVar.c_str());
                                                    }
                                                }

                                                else
                                                {
                                                    ::std::printf("'%s' Has No Help\n", strConVar.c_str());
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
    }

    else
    {
        inFile.open(strPath);

        if (!inFile.is_open())
        {
            ::std::printf("Failed To Open '%s'\n", strPath.c_str());
            {
                inFile.clear();
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
                ::std::printf("'%s' Has INVALID 'JSON'\n", strPath.c_str());
                {
                    jsTree.clear();
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
                                                    if (pConVar = ::icvar->FindVar(strConVar.c_str()))
                                                    {
                                                        pConVar->SetValue(jsItem["String Value"].get < ::std::string >().c_str());
                                                        {
                                                            ::std::printf("'%s' Is Now '%s' (~ %f)\n", strConVar.c_str(), pConVar->GetString(), pConVar->GetFloat());
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
        }
    }

    if (!::g_bNoReturn)
    {
        do
        {
            ::g_SHPtr->SetRes(::MRES_IGNORED);

            return;
        }

        while (false);
    }

    else
    {
        return;
    }
}

bool ::MySmmPlugin::Hook_LevelInit_Post(const char*, const char*, const char*, const char*, bool, bool) noexcept
{
    ::g_llMapChange = ::std::time(nullptr);

    if (!::g_bNoReturn)
    {
        do
        {
            ::g_SHPtr->SetRes(::MRES_IGNORED);

            if (::g_SHPtr->GetOrigRet())
            {
                return (*(bool*) ::g_SHPtr->GetOrigRet());
            }

            return { };
        }

        while (false);
    }

    else
    {
        return { };
    }

    return { };
}

void ::MySmmPlugin::Hook_LevelShutdown_Post() noexcept
{
    static int nPlayer{ };

    static ::edict_t* pEdict{ };
    static ::CBaseEntity* pBaseEntity{ };
    static ::IServerUnknown* pServerUnknown{ };
    static ::IServerEntity* pServerEntity{ };
    static ::IPlayerInfo* pPlayerInfo{ };

    ::g_llMapChange = ::std::time(nullptr);

    if (!::g_bNoReturn)
    {
        ::g_bNoReturn = true;

        {
            for (nPlayer = xTo(true, int); nPlayer <= ::gpGlobals->maxClients; nPlayer++)
            {

#if SOURCE_ENGINE != SE_CSGO

                if (pEdict = ::engine->PEntityOfEntIndex(nPlayer))

#else

                if (pEdict = ::gpGlobals->pEdicts + nPlayer)

#endif

                {
                    if (!(pEdict->m_fStateFlags & FL_EDICT_FREE))
                    {
                        if (!(pEdict->m_fStateFlags & FL_EDICT_DONTSEND))
                        {
                            if (pEdict->m_NetworkSerialNumber)
                            {
                                if (pEdict->m_pNetworkable)
                                {
                                    if (pServerEntity = pEdict->GetIServerEntity())
                                    {
                                        if (pServerUnknown = pEdict->GetUnknown())
                                        {
                                            if (pBaseEntity = pServerEntity->GetBaseEntity())
                                            {
                                                if (pPlayerInfo = ::playerinfomanager->GetPlayerInfo(pEdict))
                                                {
                                                    if (pPlayerInfo->IsPlayer())
                                                    {
                                                        Hook_ClientDisconnect(pEdict);
                                                        {
                                                            Hook_ClientDisconnect_Post(pEdict);
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

        ::g_bNoReturn = { };
    }

    if (!::g_bNoReturn)
    {
        do
        {
            ::g_SHPtr->SetRes(::MRES_IGNORED);

            return;
        }

        while (false);
    }

    else
    {
        return;
    }
}

void ::MySmmPlugin::Hook_GameFrame_Post(bool) noexcept
{
    static bool bOffs{ }, bUpdate{ }, bMsgs{ };
    static int nPlayer{ }, nIter{ }, nHealth{ }, nFrags{ };
    static const char* pszServerClassName{ };
    static long long llTime{ }, llStamp{ };

    static ::std::string strCmd{ };

    static ::RecipientFilter recipientFilter{ };
    static ::SendPropInfo sendPropInfo{ };
    static ::DataTableInfo dataTableInfo{ };

    static ::edict_t* pEdict{ };
    static ::CBaseEntity* pBaseEntity{ };
    static ::IPlayerInfo* pPlayerInfo{ };
    static ::datamap_t* pDataMap{ };
    static ::IServerUnknown* pServerUnknown{ };
    static ::IServerEntity* pServerEntity{ };
    static ::ServerClass* pServerClass{ };

#if SOURCE_ENGINE == SE_CSGO

    static ::google::protobuf::Message* pMsg{ };

#else

    static ::bf_write* pMsg{ };

#endif

    if (++nIter == ::g_nTicks)
    {
        if (!bMsgs)
        {
            if (!::g_strWelcome_1.empty())
            {
                if (::g_strWelcome_1.length() <= 190)
                {
                    if (::g_strWelcome_1.size() <= 190)
                    {
                        if (!::g_nTextMsg)
                        {

#if SOURCE_ENGINE == SE_CSGO

                            if ((::g_nTextMsg = ::g_Cstrike15UsermessageHelpers.GetIndex("TextMsg")) < 1)
                            {
                                if ((::g_nTextMsg = ::g_SMAPI->FindUserMessage("TextMsg", nullptr)) > 0)
                                {
                                    if (!(::g_pTextMsg = ::g_Cstrike15UsermessageHelpers.GetPrototype(::g_nTextMsg)))
                                    {
                                        ::g_nTextMsg = { };
                                    }
                                }
                            }

                            else
                            {
                                if (!(::g_pTextMsg = ::g_Cstrike15UsermessageHelpers.GetPrototype(::g_nTextMsg)))
                                {
                                    ::g_nTextMsg = { };
                                }
                            }

#else

                            ::g_nTextMsg = ::g_SMAPI->FindUserMessage("TextMsg", nullptr);

#endif

                        }
                    }
                }
            }

            bMsgs = true;
        }

        if (::g_bHooked)
        {
            if (!::g_bEvHooked)
            {
                if (::gameevents->AddListener(this, "player_spawn", true))
                {
                    ::gameevents->AddListener(this, "player_death", true);
                    {
                        ::g_bEvHooked = true;
                    }
                }
            }
        }

        for (nPlayer = xTo(true, int); nPlayer <= ::gpGlobals->maxClients; nPlayer++)
        {

#if SOURCE_ENGINE != SE_CSGO

            if (pEdict = ::engine->PEntityOfEntIndex(nPlayer))

#else

            if (pEdict = ::gpGlobals->pEdicts + nPlayer)

#endif

            {
                if (!(pEdict->m_fStateFlags & FL_EDICT_FREE))
                {
                    if (!(pEdict->m_fStateFlags & FL_EDICT_DONTSEND))
                    {
                        if (pEdict->m_NetworkSerialNumber)
                        {
                            if (pEdict->m_pNetworkable)
                            {
                                if (pServerEntity = pEdict->GetIServerEntity())
                                {
                                    if (pServerUnknown = pEdict->GetUnknown())
                                    {
                                        if (pBaseEntity = pServerEntity->GetBaseEntity())
                                        {
                                            if (pPlayerInfo = ::playerinfomanager->GetPlayerInfo(pEdict))
                                            {
                                                if (pPlayerInfo->IsPlayer())
                                                {
                                                    if (pPlayerInfo->IsConnected())
                                                    {
                                                        if (!bOffs)
                                                        {
                                                            if (pDataMap = ::FindDataMap(pBaseEntity))
                                                            {
                                                                if (::FindDataMapInfo(pDataMap, "m_iDeaths", &dataTableInfo))
                                                                {
                                                                    if (dataTableInfo.pTypeDescription)
                                                                    {
                                                                        if ((::m_iDeathsSize = ::DataSizeByFieldType(dataTableInfo.pTypeDescription->fieldType, dataTableInfo.pTypeDescription->flags)) > 0)
                                                                        {
                                                                            ::m_iDeaths = dataTableInfo.uOffs;
                                                                        }
                                                                    }
                                                                }

                                                                if (::FindDataMapInfo(pDataMap, "m_iFrags", &dataTableInfo))
                                                                {
                                                                    if (dataTableInfo.pTypeDescription)
                                                                    {
                                                                        if ((::m_iFragsSize = ::DataSizeByFieldType(dataTableInfo.pTypeDescription->fieldType, dataTableInfo.pTypeDescription->flags)) > 0)
                                                                        {
                                                                            ::m_iFrags = dataTableInfo.uOffs;
                                                                        }
                                                                    }
                                                                }
                                                            }

                                                            if (pServerClass = pEdict->m_pNetworkable->GetServerClass())
                                                            {
                                                                if (pszServerClassName = pServerClass->GetName())
                                                                {
                                                                    if (::FindSendPropInfo(pszServerClassName, "m_iAccount", &sendPropInfo))
                                                                    {
                                                                        if (sendPropInfo.pSendProp)
                                                                        {
                                                                            ::m_iAccount = sendPropInfo.uOffs;
                                                                            {
                                                                                ::m_iAccountSize = sendPropInfo.pSendProp->m_nBits;
                                                                                {
                                                                                    if (sendPropInfo.pSendProp->GetFlags() & SPROP_VARINT)
                                                                                    {
                                                                                        ::m_iAccountSize = 32;
                                                                                    }
                                                                                }
                                                                            }
                                                                        }
                                                                    }

                                                                    if (::FindSendPropInfo(pszServerClassName, "m_bHasHelmet", &sendPropInfo))
                                                                    {
                                                                        if (sendPropInfo.pSendProp)
                                                                        {
                                                                            ::m_bHasHelmet = sendPropInfo.uOffs;
                                                                            {
                                                                                ::m_bHasHelmetSize = sendPropInfo.pSendProp->m_nBits;
                                                                                {
                                                                                    if (sendPropInfo.pSendProp->GetFlags() & SPROP_VARINT)
                                                                                    {
                                                                                        ::m_bHasHelmetSize = 32;
                                                                                    }
                                                                                }
                                                                            }
                                                                        }
                                                                    }

                                                                    if (::FindSendPropInfo(pszServerClassName, "m_ArmorValue", &sendPropInfo))
                                                                    {
                                                                        if (sendPropInfo.pSendProp)
                                                                        {
                                                                            ::m_ArmorValue = sendPropInfo.uOffs;
                                                                            {
                                                                                ::m_ArmorValueSize = sendPropInfo.pSendProp->m_nBits;
                                                                                {
                                                                                    if (sendPropInfo.pSendProp->GetFlags() & SPROP_VARINT)
                                                                                    {
                                                                                        ::m_ArmorValueSize = 32;
                                                                                    }
                                                                                }
                                                                            }
                                                                        }
                                                                    }

                                                                    if (::FindSendPropInfo(pszServerClassName, "m_flFlashDuration", &sendPropInfo))
                                                                    {
                                                                        if (sendPropInfo.pSendProp)
                                                                        {
                                                                            ::m_flFlashDuration = sendPropInfo.uOffs;
                                                                        }
                                                                    }

                                                                    if (::FindSendPropInfo(pszServerClassName, "m_flFlashMaxAlpha", &sendPropInfo))
                                                                    {
                                                                        if (sendPropInfo.pSendProp)
                                                                        {
                                                                            ::m_flFlashMaxAlpha = sendPropInfo.uOffs;
                                                                        }
                                                                    }
                                                                }
                                                            }

                                                            bOffs = true;
                                                        }

                                                        if (::g_bFixScore)
                                                        {
                                                            if (::m_iFrags)
                                                            {
                                                                if (pPlayerInfo->GetFragCount() < 0)
                                                                {
                                                                    if (::m_iFragsSize >= 17)
                                                                    {
                                                                        if (*(signed int*)((unsigned char*)pBaseEntity + ::m_iFrags))
                                                                        {
                                                                            *(signed int*)((unsigned char*)pBaseEntity + ::m_iFrags) = { };
                                                                            {
                                                                                ::g_nKills[nPlayer] = { };
                                                                            }
                                                                        }
                                                                    }

                                                                    else
                                                                    {
                                                                        if (*(signed short*)((unsigned char*)pBaseEntity + ::m_iFrags))
                                                                        {
                                                                            *(signed short*)((unsigned char*)pBaseEntity + ::m_iFrags) = { };
                                                                            {
                                                                                ::g_nKills[nPlayer] = { };
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }

                                                            if (::m_iDeaths)
                                                            {
                                                                if (pPlayerInfo->GetDeathCount() > 0)
                                                                {
                                                                    if (pPlayerInfo->GetFragCount() < 1)
                                                                    {
                                                                        if (::m_iDeathsSize >= 17)
                                                                        {
                                                                            if (*(signed int*)((unsigned char*)pBaseEntity + ::m_iDeaths))
                                                                            {
                                                                                *(signed int*)((unsigned char*)pBaseEntity + ::m_iDeaths) = { };
                                                                                {
                                                                                    ::g_nDeaths[nPlayer] = { };
                                                                                }
                                                                            }
                                                                        }

                                                                        else
                                                                        {
                                                                            if (*(signed short*)((unsigned char*)pBaseEntity + ::m_iDeaths))
                                                                            {
                                                                                *(signed short*)((unsigned char*)pBaseEntity + ::m_iDeaths) = { };
                                                                                {
                                                                                    ::g_nDeaths[nPlayer] = { };
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                                                                }

                                                                if (pPlayerInfo->GetDeathCount() > (nFrags = pPlayerInfo->GetFragCount()))
                                                                {
                                                                    if (::m_iDeathsSize >= 17)
                                                                    {
                                                                        if (*(signed int*)((unsigned char*)pBaseEntity + ::m_iDeaths))
                                                                        {
                                                                            *(signed int*)((unsigned char*)pBaseEntity + ::m_iDeaths) = (::std::max)(xTo(false, int), nFrags);
                                                                            {
                                                                                ::g_nDeaths[nPlayer] = (::std::max)(xTo(false, int), nFrags);
                                                                            }
                                                                        }
                                                                    }

                                                                    else
                                                                    {
                                                                        if (*(signed short*)((unsigned char*)pBaseEntity + ::m_iDeaths))
                                                                        {
                                                                            *(signed short*)((unsigned char*)pBaseEntity + ::m_iDeaths) = (::std::max)(xTo(false, int), nFrags);
                                                                            {
                                                                                ::g_nDeaths[nPlayer] = (::std::max)(xTo(false, int), nFrags);
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }

                                                        else if (::g_bFixKills)
                                                        {
                                                            if (::m_iFrags)
                                                            {
                                                                if (pPlayerInfo->GetFragCount() < 0)
                                                                {
                                                                    if (::m_iFragsSize >= 17)
                                                                    {
                                                                        if (*(signed int*)((unsigned char*)pBaseEntity + ::m_iFrags))
                                                                        {
                                                                            *(signed int*)((unsigned char*)pBaseEntity + ::m_iFrags) = { };
                                                                            {
                                                                                ::g_nKills[nPlayer] = { };
                                                                            }
                                                                        }
                                                                    }

                                                                    else
                                                                    {
                                                                        if (*(signed short*)((unsigned char*)pBaseEntity + ::m_iFrags))
                                                                        {
                                                                            *(signed short*)((unsigned char*)pBaseEntity + ::m_iFrags) = { };
                                                                            {
                                                                                ::g_nKills[nPlayer] = { };
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }

                                                        else if (::g_bFixDeaths)
                                                        {
                                                            if (::m_iDeaths)
                                                            {
                                                                if (pPlayerInfo->GetDeathCount() > 0)
                                                                {
                                                                    if (pPlayerInfo->GetFragCount() < 1)
                                                                    {
                                                                        if (::m_iDeathsSize >= 17)
                                                                        {
                                                                            if (*(signed int*)((unsigned char*)pBaseEntity + ::m_iDeaths))
                                                                            {
                                                                                *(signed int*)((unsigned char*)pBaseEntity + ::m_iDeaths) = { };
                                                                                {
                                                                                    ::g_nDeaths[nPlayer] = { };
                                                                                }
                                                                            }
                                                                        }

                                                                        else
                                                                        {
                                                                            if (*(signed short*)((unsigned char*)pBaseEntity + ::m_iDeaths))
                                                                            {
                                                                                *(signed short*)((unsigned char*)pBaseEntity + ::m_iDeaths) = { };
                                                                                {
                                                                                    ::g_nDeaths[nPlayer] = { };
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                                                                }

                                                                if (pPlayerInfo->GetDeathCount() > (nFrags = pPlayerInfo->GetFragCount()))
                                                                {
                                                                    if (::m_iDeathsSize >= 17)
                                                                    {
                                                                        if (*(signed int*)((unsigned char*)pBaseEntity + ::m_iDeaths))
                                                                        {
                                                                            *(signed int*)((unsigned char*)pBaseEntity + ::m_iDeaths) = (::std::max)(xTo(false, int), nFrags);
                                                                            {
                                                                                ::g_nDeaths[nPlayer] = (::std::max)(xTo(false, int), nFrags);
                                                                            }
                                                                        }
                                                                    }

                                                                    else
                                                                    {
                                                                        if (*(signed short*)((unsigned char*)pBaseEntity + ::m_iDeaths))
                                                                        {
                                                                            *(signed short*)((unsigned char*)pBaseEntity + ::m_iDeaths) = (::std::max)(xTo(false, int), nFrags);
                                                                            {
                                                                                ::g_nDeaths[nPlayer] = (::std::max)(xTo(false, int), nFrags);
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }

                                                        if (::g_bRealScore)
                                                        {
                                                            if (::g_nDeaths[nPlayer] || ::g_nKills[nPlayer])
                                                            {
                                                                if (!pPlayerInfo->GetFragCount())
                                                                {
                                                                    if (!pPlayerInfo->GetDeathCount())
                                                                    {
                                                                        ::g_nDeaths[nPlayer] = { };
                                                                        {
                                                                            ::g_nKills[nPlayer] = { };
                                                                        }
                                                                    }
                                                                }
                                                            }

                                                            if (::m_iFrags)
                                                            {
                                                                if (pPlayerInfo->GetFragCount() != ::g_nKills[nPlayer])
                                                                {
                                                                    if (::m_iFragsSize >= 17)
                                                                    {
                                                                        if (*(signed int*)((unsigned char*)pBaseEntity + ::m_iFrags) != ::g_nKills[nPlayer])
                                                                        {
                                                                            *(signed int*)((unsigned char*)pBaseEntity + ::m_iFrags) = ::g_nKills[nPlayer];
                                                                        }
                                                                    }

                                                                    else
                                                                    {
                                                                        if (*(signed short*)((unsigned char*)pBaseEntity + ::m_iFrags) != ::g_nKills[nPlayer])
                                                                        {
                                                                            *(signed short*)((unsigned char*)pBaseEntity + ::m_iFrags) = ::g_nKills[nPlayer];
                                                                        }
                                                                    }
                                                                }
                                                            }

                                                            if (::m_iDeaths)
                                                            {
                                                                if (pPlayerInfo->GetDeathCount() != ::g_nDeaths[nPlayer])
                                                                {
                                                                    if (::m_iDeathsSize >= 17)
                                                                    {
                                                                        if (*(signed int*)((unsigned char*)pBaseEntity + ::m_iDeaths) != ::g_nDeaths[nPlayer])
                                                                        {
                                                                            *(signed int*)((unsigned char*)pBaseEntity + ::m_iDeaths) = ::g_nDeaths[nPlayer];
                                                                        }
                                                                    }

                                                                    else
                                                                    {
                                                                        if (*(signed short*)((unsigned char*)pBaseEntity + ::m_iDeaths) != ::g_nDeaths[nPlayer])
                                                                        {
                                                                            *(signed short*)((unsigned char*)pBaseEntity + ::m_iDeaths) = ::g_nDeaths[nPlayer];
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }

                                                        if (::g_bHideRadar)
                                                        {
                                                            if (::m_flFlashDuration)
                                                            {
                                                                if (*(float*)((unsigned char*)pBaseEntity + ::m_flFlashDuration) != 100000.f)
                                                                {
                                                                    *(float*)((unsigned char*)pBaseEntity + ::m_flFlashDuration) = 100000.f;
                                                                    {
                                                                        if (::g_pSharedChangeInfo)
                                                                        {
                                                                            pEdict->StateChanged(xTo(::m_flFlashDuration, unsigned short));
                                                                        }

                                                                        else
                                                                        {
                                                                            pEdict->m_fStateFlags |= FL_EDICT_CHANGED;
                                                                        }
                                                                    }
                                                                }
                                                            }

                                                            if (::m_flFlashMaxAlpha)
                                                            {
                                                                if (*(float*)((unsigned char*)pBaseEntity + ::m_flFlashMaxAlpha) != .000001f)
                                                                {
                                                                    *(float*)((unsigned char*)pBaseEntity + ::m_flFlashMaxAlpha) = .000001f;
                                                                    {
                                                                        if (::g_pSharedChangeInfo)
                                                                        {
                                                                            pEdict->StateChanged(xTo(::m_flFlashMaxAlpha, unsigned short));
                                                                        }

                                                                        else
                                                                        {
                                                                            pEdict->m_fStateFlags |= FL_EDICT_CHANGED;
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }

                                                        if (::g_bZeroMoney)
                                                        {
                                                            if (::m_iAccount)
                                                            {
                                                                bUpdate = { };

                                                                if (::m_iAccountSize >= 17)
                                                                {
                                                                    if (*(signed int*)((unsigned char*)pBaseEntity + ::m_iAccount))
                                                                    {
                                                                        *(signed int*)((unsigned char*)pBaseEntity + ::m_iAccount) = { };
                                                                        {
                                                                            bUpdate = true;
                                                                        }
                                                                    }
                                                                }

                                                                else if (::m_iAccountSize >= 9)
                                                                {
                                                                    if (*(signed short*)((unsigned char*)pBaseEntity + ::m_iAccount))
                                                                    {
                                                                        *(signed short*)((unsigned char*)pBaseEntity + ::m_iAccount) = { };
                                                                        {
                                                                            bUpdate = true;
                                                                        }
                                                                    }
                                                                }

                                                                else
                                                                {
                                                                    if (*(signed char*)((unsigned char*)pBaseEntity + ::m_iAccount))
                                                                    {
                                                                        *(signed char*)((unsigned char*)pBaseEntity + ::m_iAccount) = { };
                                                                        {
                                                                            bUpdate = true;
                                                                        }
                                                                    }
                                                                }

                                                                if (bUpdate)
                                                                {
                                                                    if (::g_pSharedChangeInfo)
                                                                    {
                                                                        pEdict->StateChanged(xTo(::m_iAccount, unsigned short));
                                                                    }

                                                                    else
                                                                    {
                                                                        pEdict->m_fStateFlags |= FL_EDICT_CHANGED;
                                                                    }
                                                                }
                                                            }
                                                        }

                                                        if (pPlayerInfo->GetTeamIndex() > 0)
                                                        {
                                                            if (::g_nTextMsg > 0)
                                                            {
                                                                if (!::g_strWelcome_1.empty())
                                                                {
                                                                    if (::g_strWelcome_1.length() <= 190)
                                                                    {
                                                                        if (::g_strWelcome_1.size() <= 190)
                                                                        {
                                                                            if (!::g_bShow[nPlayer])
                                                                            {

#if SOURCE_ENGINE == SE_CSGO

                                                                                if (pMsg = ::g_pTextMsg->New())
                                                                                {
                                                                                    ((::CCSUsrMsg_TextMsg*)(pMsg))->set_msg_dst(HUD_PRINTCONSOLE);
                                                                                    {
                                                                                        ((::CCSUsrMsg_TextMsg*)(pMsg))->add_params(::g_strWelcome_1.c_str());
                                                                                        {
                                                                                            ((::CCSUsrMsg_TextMsg*)(pMsg))->add_params("");
                                                                                            ((::CCSUsrMsg_TextMsg*)(pMsg))->add_params("");
                                                                                            ((::CCSUsrMsg_TextMsg*)(pMsg))->add_params("");
                                                                                            ((::CCSUsrMsg_TextMsg*)(pMsg))->add_params("");
                                                                                            {
                                                                                                recipientFilter.Zero();
                                                                                                {
                                                                                                    recipientFilter.Add(nPlayer);
                                                                                                    {
                                                                                                        recipientFilter.Set(::g_bReliable, { });
                                                                                                        {
                                                                                                            ::engine->SendUserMessage(recipientFilter, ::g_nTextMsg, *pMsg);
                                                                                                            {
                                                                                                                if (pMsg)
                                                                                                                {
                                                                                                                    delete pMsg;
                                                                                                                }
                                                                                                            }
                                                                                                        }
                                                                                                    }
                                                                                                }
                                                                                            }
                                                                                        }
                                                                                    }
                                                                                }

#else

                                                                                recipientFilter.Zero();
                                                                                {
                                                                                    recipientFilter.Add(nPlayer);
                                                                                    {
                                                                                        recipientFilter.Set(::g_bReliable, { });
                                                                                        {
                                                                                            if (pMsg = ::engine->UserMessageBegin(&recipientFilter, ::g_nTextMsg))
                                                                                            {
                                                                                                pMsg->WriteByte(HUD_PRINTCONSOLE);
                                                                                                {
                                                                                                    pMsg->WriteString(::g_strWelcome_1.c_str());
                                                                                                    {
                                                                                                        pMsg->WriteString("");
                                                                                                        pMsg->WriteString("");
                                                                                                        pMsg->WriteString("");
                                                                                                        pMsg->WriteString("");
                                                                                                        {
                                                                                                            ::engine->MessageEnd();
                                                                                                        }
                                                                                                    }
                                                                                                }
                                                                                            }
                                                                                        }
                                                                                    }
                                                                                }

#endif

                                                                                if (!::g_strWelcome_2.empty())
                                                                                {
                                                                                    if (::g_strWelcome_2.length() <= 190)
                                                                                    {
                                                                                        if (::g_strWelcome_2.size() <= 190)
                                                                                        {

#if SOURCE_ENGINE == SE_CSGO

                                                                                            if (pMsg = ::g_pTextMsg->New())
                                                                                            {
                                                                                                ((::CCSUsrMsg_TextMsg*)(pMsg))->set_msg_dst(HUD_PRINTCONSOLE);
                                                                                                {
                                                                                                    ((::CCSUsrMsg_TextMsg*)(pMsg))->add_params(::g_strWelcome_2.c_str());
                                                                                                    {
                                                                                                        ((::CCSUsrMsg_TextMsg*)(pMsg))->add_params("");
                                                                                                        ((::CCSUsrMsg_TextMsg*)(pMsg))->add_params("");
                                                                                                        ((::CCSUsrMsg_TextMsg*)(pMsg))->add_params("");
                                                                                                        ((::CCSUsrMsg_TextMsg*)(pMsg))->add_params("");
                                                                                                        {
                                                                                                            recipientFilter.Zero();
                                                                                                            {
                                                                                                                recipientFilter.Add(nPlayer);
                                                                                                                {
                                                                                                                    recipientFilter.Set(::g_bReliable, { });
                                                                                                                    {
                                                                                                                        ::engine->SendUserMessage(recipientFilter, ::g_nTextMsg, *pMsg);
                                                                                                                        {
                                                                                                                            if (pMsg)
                                                                                                                            {
                                                                                                                                delete pMsg;
                                                                                                                            }
                                                                                                                        }
                                                                                                                    }
                                                                                                                }
                                                                                                            }
                                                                                                        }
                                                                                                    }
                                                                                                }
                                                                                            }

#else

                                                                                            recipientFilter.Zero();
                                                                                            {
                                                                                                recipientFilter.Add(nPlayer);
                                                                                                {
                                                                                                    recipientFilter.Set(::g_bReliable, { });
                                                                                                    {
                                                                                                        if (pMsg = ::engine->UserMessageBegin(&recipientFilter, ::g_nTextMsg))
                                                                                                        {
                                                                                                            pMsg->WriteByte(HUD_PRINTCONSOLE);
                                                                                                            {
                                                                                                                pMsg->WriteString(::g_strWelcome_2.c_str());
                                                                                                                {
                                                                                                                    pMsg->WriteString("");
                                                                                                                    pMsg->WriteString("");
                                                                                                                    pMsg->WriteString("");
                                                                                                                    pMsg->WriteString("");
                                                                                                                    {
                                                                                                                        ::engine->MessageEnd();
                                                                                                                    }
                                                                                                                }
                                                                                                            }
                                                                                                        }
                                                                                                    }
                                                                                                }
                                                                                            }

#endif

                                                                                        }
                                                                                    }
                                                                                }

                                                                                if (!::g_strWelcome_3.empty())
                                                                                {
                                                                                    if (::g_strWelcome_3.length() <= 190)
                                                                                    {
                                                                                        if (::g_strWelcome_3.size() <= 190)
                                                                                        {

#if SOURCE_ENGINE == SE_CSGO

                                                                                            if (pMsg = ::g_pTextMsg->New())
                                                                                            {
                                                                                                ((::CCSUsrMsg_TextMsg*)(pMsg))->set_msg_dst(HUD_PRINTCONSOLE);
                                                                                                {
                                                                                                    ((::CCSUsrMsg_TextMsg*)(pMsg))->add_params(::g_strWelcome_3.c_str());
                                                                                                    {
                                                                                                        ((::CCSUsrMsg_TextMsg*)(pMsg))->add_params("");
                                                                                                        ((::CCSUsrMsg_TextMsg*)(pMsg))->add_params("");
                                                                                                        ((::CCSUsrMsg_TextMsg*)(pMsg))->add_params("");
                                                                                                        ((::CCSUsrMsg_TextMsg*)(pMsg))->add_params("");
                                                                                                        {
                                                                                                            recipientFilter.Zero();
                                                                                                            {
                                                                                                                recipientFilter.Add(nPlayer);
                                                                                                                {
                                                                                                                    recipientFilter.Set(::g_bReliable, { });
                                                                                                                    {
                                                                                                                        ::engine->SendUserMessage(recipientFilter, ::g_nTextMsg, *pMsg);
                                                                                                                        {
                                                                                                                            if (pMsg)
                                                                                                                            {
                                                                                                                                delete pMsg;
                                                                                                                            }
                                                                                                                        }
                                                                                                                    }
                                                                                                                }
                                                                                                            }
                                                                                                        }
                                                                                                    }
                                                                                                }
                                                                                            }

#else

                                                                                            recipientFilter.Zero();
                                                                                            {
                                                                                                recipientFilter.Add(nPlayer);
                                                                                                {
                                                                                                    recipientFilter.Set(::g_bReliable, { });
                                                                                                    {
                                                                                                        if (pMsg = ::engine->UserMessageBegin(&recipientFilter, ::g_nTextMsg))
                                                                                                        {
                                                                                                            pMsg->WriteByte(HUD_PRINTCONSOLE);
                                                                                                            {
                                                                                                                pMsg->WriteString(::g_strWelcome_3.c_str());
                                                                                                                {
                                                                                                                    pMsg->WriteString("");
                                                                                                                    pMsg->WriteString("");
                                                                                                                    pMsg->WriteString("");
                                                                                                                    pMsg->WriteString("");
                                                                                                                    {
                                                                                                                        ::engine->MessageEnd();
                                                                                                                    }
                                                                                                                }
                                                                                                            }
                                                                                                        }
                                                                                                    }
                                                                                                }
                                                                                            }

#endif

                                                                                        }
                                                                                    }
                                                                                }

                                                                                if (!::g_strWelcome_4.empty())
                                                                                {
                                                                                    if (::g_strWelcome_4.length() <= 190)
                                                                                    {
                                                                                        if (::g_strWelcome_4.size() <= 190)
                                                                                        {

#if SOURCE_ENGINE == SE_CSGO

                                                                                            if (pMsg = ::g_pTextMsg->New())
                                                                                            {
                                                                                                ((::CCSUsrMsg_TextMsg*)(pMsg))->set_msg_dst(HUD_PRINTCONSOLE);
                                                                                                {
                                                                                                    ((::CCSUsrMsg_TextMsg*)(pMsg))->add_params(::g_strWelcome_4.c_str());
                                                                                                    {
                                                                                                        ((::CCSUsrMsg_TextMsg*)(pMsg))->add_params("");
                                                                                                        ((::CCSUsrMsg_TextMsg*)(pMsg))->add_params("");
                                                                                                        ((::CCSUsrMsg_TextMsg*)(pMsg))->add_params("");
                                                                                                        ((::CCSUsrMsg_TextMsg*)(pMsg))->add_params("");
                                                                                                        {
                                                                                                            recipientFilter.Zero();
                                                                                                            {
                                                                                                                recipientFilter.Add(nPlayer);
                                                                                                                {
                                                                                                                    recipientFilter.Set(::g_bReliable, { });
                                                                                                                    {
                                                                                                                        ::engine->SendUserMessage(recipientFilter, ::g_nTextMsg, *pMsg);
                                                                                                                        {
                                                                                                                            if (pMsg)
                                                                                                                            {
                                                                                                                                delete pMsg;
                                                                                                                            }
                                                                                                                        }
                                                                                                                    }
                                                                                                                }
                                                                                                            }
                                                                                                        }
                                                                                                    }
                                                                                                }
                                                                                            }

#else

                                                                                            recipientFilter.Zero();
                                                                                            {
                                                                                                recipientFilter.Add(nPlayer);
                                                                                                {
                                                                                                    recipientFilter.Set(::g_bReliable, { });
                                                                                                    {
                                                                                                        if (pMsg = ::engine->UserMessageBegin(&recipientFilter, ::g_nTextMsg))
                                                                                                        {
                                                                                                            pMsg->WriteByte(HUD_PRINTCONSOLE);
                                                                                                            {
                                                                                                                pMsg->WriteString(::g_strWelcome_4.c_str());
                                                                                                                {
                                                                                                                    pMsg->WriteString("");
                                                                                                                    pMsg->WriteString("");
                                                                                                                    pMsg->WriteString("");
                                                                                                                    pMsg->WriteString("");
                                                                                                                    {
                                                                                                                        ::engine->MessageEnd();
                                                                                                                    }
                                                                                                                }
                                                                                                            }
                                                                                                        }
                                                                                                    }
                                                                                                }
                                                                                            }

#endif

                                                                                        }
                                                                                    }
                                                                                }

                                                                                if (!::g_strWelcome_5.empty())
                                                                                {
                                                                                    if (::g_strWelcome_5.length() <= 190)
                                                                                    {
                                                                                        if (::g_strWelcome_5.size() <= 190)
                                                                                        {

#if SOURCE_ENGINE == SE_CSGO

                                                                                            if (pMsg = ::g_pTextMsg->New())
                                                                                            {
                                                                                                ((::CCSUsrMsg_TextMsg*)(pMsg))->set_msg_dst(HUD_PRINTCONSOLE);
                                                                                                {
                                                                                                    ((::CCSUsrMsg_TextMsg*)(pMsg))->add_params(::g_strWelcome_5.c_str());
                                                                                                    {
                                                                                                        ((::CCSUsrMsg_TextMsg*)(pMsg))->add_params("");
                                                                                                        ((::CCSUsrMsg_TextMsg*)(pMsg))->add_params("");
                                                                                                        ((::CCSUsrMsg_TextMsg*)(pMsg))->add_params("");
                                                                                                        ((::CCSUsrMsg_TextMsg*)(pMsg))->add_params("");
                                                                                                        {
                                                                                                            recipientFilter.Zero();
                                                                                                            {
                                                                                                                recipientFilter.Add(nPlayer);
                                                                                                                {
                                                                                                                    recipientFilter.Set(::g_bReliable, { });
                                                                                                                    {
                                                                                                                        ::engine->SendUserMessage(recipientFilter, ::g_nTextMsg, *pMsg);
                                                                                                                        {
                                                                                                                            if (pMsg)
                                                                                                                            {
                                                                                                                                delete pMsg;
                                                                                                                            }
                                                                                                                        }
                                                                                                                    }
                                                                                                                }
                                                                                                            }
                                                                                                        }
                                                                                                    }
                                                                                                }
                                                                                            }

#else

                                                                                            recipientFilter.Zero();
                                                                                            {
                                                                                                recipientFilter.Add(nPlayer);
                                                                                                {
                                                                                                    recipientFilter.Set(::g_bReliable, { });
                                                                                                    {
                                                                                                        if (pMsg = ::engine->UserMessageBegin(&recipientFilter, ::g_nTextMsg))
                                                                                                        {
                                                                                                            pMsg->WriteByte(HUD_PRINTCONSOLE);
                                                                                                            {
                                                                                                                pMsg->WriteString(::g_strWelcome_5.c_str());
                                                                                                                {
                                                                                                                    pMsg->WriteString("");
                                                                                                                    pMsg->WriteString("");
                                                                                                                    pMsg->WriteString("");
                                                                                                                    pMsg->WriteString("");
                                                                                                                    {
                                                                                                                        ::engine->MessageEnd();
                                                                                                                    }
                                                                                                                }
                                                                                                            }
                                                                                                        }
                                                                                                    }
                                                                                                }
                                                                                            }

#endif

                                                                                        }
                                                                                    }
                                                                                }

                                                                                if (!::g_strWelcome_6.empty())
                                                                                {
                                                                                    if (::g_strWelcome_6.length() <= 190)
                                                                                    {
                                                                                        if (::g_strWelcome_6.size() <= 190)
                                                                                        {

#if SOURCE_ENGINE == SE_CSGO

                                                                                            if (pMsg = ::g_pTextMsg->New())
                                                                                            {
                                                                                                ((::CCSUsrMsg_TextMsg*)(pMsg))->set_msg_dst(HUD_PRINTCONSOLE);
                                                                                                {
                                                                                                    ((::CCSUsrMsg_TextMsg*)(pMsg))->add_params(::g_strWelcome_6.c_str());
                                                                                                    {
                                                                                                        ((::CCSUsrMsg_TextMsg*)(pMsg))->add_params("");
                                                                                                        ((::CCSUsrMsg_TextMsg*)(pMsg))->add_params("");
                                                                                                        ((::CCSUsrMsg_TextMsg*)(pMsg))->add_params("");
                                                                                                        ((::CCSUsrMsg_TextMsg*)(pMsg))->add_params("");
                                                                                                        {
                                                                                                            recipientFilter.Zero();
                                                                                                            {
                                                                                                                recipientFilter.Add(nPlayer);
                                                                                                                {
                                                                                                                    recipientFilter.Set(::g_bReliable, { });
                                                                                                                    {
                                                                                                                        ::engine->SendUserMessage(recipientFilter, ::g_nTextMsg, *pMsg);
                                                                                                                        {
                                                                                                                            if (pMsg)
                                                                                                                            {
                                                                                                                                delete pMsg;
                                                                                                                            }
                                                                                                                        }
                                                                                                                    }
                                                                                                                }
                                                                                                            }
                                                                                                        }
                                                                                                    }
                                                                                                }
                                                                                            }

#else

                                                                                            recipientFilter.Zero();
                                                                                            {
                                                                                                recipientFilter.Add(nPlayer);
                                                                                                {
                                                                                                    recipientFilter.Set(::g_bReliable, { });
                                                                                                    {
                                                                                                        if (pMsg = ::engine->UserMessageBegin(&recipientFilter, ::g_nTextMsg))
                                                                                                        {
                                                                                                            pMsg->WriteByte(HUD_PRINTCONSOLE);
                                                                                                            {
                                                                                                                pMsg->WriteString(::g_strWelcome_6.c_str());
                                                                                                                {
                                                                                                                    pMsg->WriteString("");
                                                                                                                    pMsg->WriteString("");
                                                                                                                    pMsg->WriteString("");
                                                                                                                    pMsg->WriteString("");
                                                                                                                    {
                                                                                                                        ::engine->MessageEnd();
                                                                                                                    }
                                                                                                                }
                                                                                                            }
                                                                                                        }
                                                                                                    }
                                                                                                }
                                                                                            }

#endif

                                                                                        }
                                                                                    }
                                                                                }

                                                                                if (!::g_strNotify_1.empty())
                                                                                {
                                                                                    if (::g_strNotify_1.length() <= 190)
                                                                                    {
                                                                                        if (::g_strNotify_1.size() <= 190)
                                                                                        {

#if SOURCE_ENGINE == SE_CSGO

                                                                                            if (pMsg = ::g_pTextMsg->New())
                                                                                            {
                                                                                                ((::CCSUsrMsg_TextMsg*)(pMsg))->set_msg_dst(HUD_PRINTTALK);
                                                                                                {
                                                                                                    ((::CCSUsrMsg_TextMsg*)(pMsg))->add_params(::g_strNotify_1.c_str());
                                                                                                    {
                                                                                                        ((::CCSUsrMsg_TextMsg*)(pMsg))->add_params("");
                                                                                                        ((::CCSUsrMsg_TextMsg*)(pMsg))->add_params("");
                                                                                                        ((::CCSUsrMsg_TextMsg*)(pMsg))->add_params("");
                                                                                                        ((::CCSUsrMsg_TextMsg*)(pMsg))->add_params("");
                                                                                                        {
                                                                                                            recipientFilter.Zero();
                                                                                                            {
                                                                                                                recipientFilter.Add(nPlayer);
                                                                                                                {
                                                                                                                    recipientFilter.Set(::g_bReliable, { });
                                                                                                                    {
                                                                                                                        ::engine->SendUserMessage(recipientFilter, ::g_nTextMsg, *pMsg);
                                                                                                                        {
                                                                                                                            if (pMsg)
                                                                                                                            {
                                                                                                                                delete pMsg;
                                                                                                                            }
                                                                                                                        }
                                                                                                                    }
                                                                                                                }
                                                                                                            }
                                                                                                        }
                                                                                                    }
                                                                                                }
                                                                                            }

#else

                                                                                            recipientFilter.Zero();
                                                                                            {
                                                                                                recipientFilter.Add(nPlayer);
                                                                                                {
                                                                                                    recipientFilter.Set(::g_bReliable, { });
                                                                                                    {
                                                                                                        if (pMsg = ::engine->UserMessageBegin(&recipientFilter, ::g_nTextMsg))
                                                                                                        {
                                                                                                            pMsg->WriteByte(HUD_PRINTTALK);
                                                                                                            {
                                                                                                                pMsg->WriteString(::g_strNotify_1.c_str());
                                                                                                                {
                                                                                                                    pMsg->WriteString("");
                                                                                                                    pMsg->WriteString("");
                                                                                                                    pMsg->WriteString("");
                                                                                                                    pMsg->WriteString("");
                                                                                                                    {
                                                                                                                        ::engine->MessageEnd();
                                                                                                                    }
                                                                                                                }
                                                                                                            }
                                                                                                        }
                                                                                                    }
                                                                                                }
                                                                                            }

#endif

                                                                                        }
                                                                                    }
                                                                                }

                                                                                if (!::g_strNotify_2.empty())
                                                                                {
                                                                                    if (::g_strNotify_2.length() <= 190)
                                                                                    {
                                                                                        if (::g_strNotify_2.size() <= 190)
                                                                                        {

#if SOURCE_ENGINE == SE_CSGO

                                                                                            if (pMsg = ::g_pTextMsg->New())
                                                                                            {
                                                                                                ((::CCSUsrMsg_TextMsg*)(pMsg))->set_msg_dst(HUD_PRINTTALK);
                                                                                                {
                                                                                                    ((::CCSUsrMsg_TextMsg*)(pMsg))->add_params(::g_strNotify_2.c_str());
                                                                                                    {
                                                                                                        ((::CCSUsrMsg_TextMsg*)(pMsg))->add_params("");
                                                                                                        ((::CCSUsrMsg_TextMsg*)(pMsg))->add_params("");
                                                                                                        ((::CCSUsrMsg_TextMsg*)(pMsg))->add_params("");
                                                                                                        ((::CCSUsrMsg_TextMsg*)(pMsg))->add_params("");
                                                                                                        {
                                                                                                            recipientFilter.Zero();
                                                                                                            {
                                                                                                                recipientFilter.Add(nPlayer);
                                                                                                                {
                                                                                                                    recipientFilter.Set(::g_bReliable, { });
                                                                                                                    {
                                                                                                                        ::engine->SendUserMessage(recipientFilter, ::g_nTextMsg, *pMsg);
                                                                                                                        {
                                                                                                                            if (pMsg)
                                                                                                                            {
                                                                                                                                delete pMsg;
                                                                                                                            }
                                                                                                                        }
                                                                                                                    }
                                                                                                                }
                                                                                                            }
                                                                                                        }
                                                                                                    }
                                                                                                }
                                                                                            }

#else

                                                                                            recipientFilter.Zero();
                                                                                            {
                                                                                                recipientFilter.Add(nPlayer);
                                                                                                {
                                                                                                    recipientFilter.Set(::g_bReliable, { });
                                                                                                    {
                                                                                                        if (pMsg = ::engine->UserMessageBegin(&recipientFilter, ::g_nTextMsg))
                                                                                                        {
                                                                                                            pMsg->WriteByte(HUD_PRINTTALK);
                                                                                                            {
                                                                                                                pMsg->WriteString(::g_strNotify_2.c_str());
                                                                                                                {
                                                                                                                    pMsg->WriteString("");
                                                                                                                    pMsg->WriteString("");
                                                                                                                    pMsg->WriteString("");
                                                                                                                    pMsg->WriteString("");
                                                                                                                    {
                                                                                                                        ::engine->MessageEnd();
                                                                                                                    }
                                                                                                                }
                                                                                                            }
                                                                                                        }
                                                                                                    }
                                                                                                }
                                                                                            }

#endif

                                                                                        }
                                                                                    }
                                                                                }

                                                                                ::g_bShow[nPlayer] = true;
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }

                                                            if (!pPlayerInfo->IsDead())
                                                            {
                                                                if ((nHealth = pPlayerInfo->GetHealth()) >= 100)
                                                                {
                                                                    if (pPlayerInfo->GetArmorValue() < 100)
                                                                    {
                                                                        if (::g_nArmor)
                                                                        {
                                                                            if (::m_ArmorValue)
                                                                            {
                                                                                if (!::g_bArmor[nPlayer])
                                                                                {
                                                                                    if (!::g_bKeepHelmet)
                                                                                    {
                                                                                        bUpdate = { };

                                                                                        if (::m_ArmorValueSize >= 17)
                                                                                        {
                                                                                            if (*(signed int*)((unsigned char*)pBaseEntity + ::m_ArmorValue) != 100)
                                                                                            {
                                                                                                *(signed int*)((unsigned char*)pBaseEntity + ::m_ArmorValue) = 100;
                                                                                                {
                                                                                                    bUpdate = true;
                                                                                                }
                                                                                            }
                                                                                        }

                                                                                        else if (::m_ArmorValueSize >= 9)
                                                                                        {
                                                                                            if (*(signed short*)((unsigned char*)pBaseEntity + ::m_ArmorValue) != 100)
                                                                                            {
                                                                                                *(signed short*)((unsigned char*)pBaseEntity + ::m_ArmorValue) = 100;
                                                                                                {
                                                                                                    bUpdate = true;
                                                                                                }
                                                                                            }
                                                                                        }

                                                                                        else
                                                                                        {
                                                                                            if (*(signed char*)((unsigned char*)pBaseEntity + ::m_ArmorValue) != 100)
                                                                                            {
                                                                                                *(signed char*)((unsigned char*)pBaseEntity + ::m_ArmorValue) = 100;
                                                                                                {
                                                                                                    bUpdate = true;
                                                                                                }
                                                                                            }
                                                                                        }

                                                                                        if (bUpdate)
                                                                                        {
                                                                                            if (::g_pSharedChangeInfo)
                                                                                            {
                                                                                                pEdict->StateChanged(xTo(::m_ArmorValue, unsigned short));
                                                                                            }

                                                                                            else
                                                                                            {
                                                                                                pEdict->m_fStateFlags |= FL_EDICT_CHANGED;
                                                                                            }
                                                                                        }

                                                                                        if (::m_bHasHelmet)
                                                                                        {
                                                                                            bUpdate = { };

                                                                                            if (::g_nArmor < 2)
                                                                                            {
                                                                                                if (::m_bHasHelmetSize >= 17)
                                                                                                {
                                                                                                    if (*(signed int*)((unsigned char*)pBaseEntity + ::m_bHasHelmet))
                                                                                                    {
                                                                                                        *(signed int*)((unsigned char*)pBaseEntity + ::m_bHasHelmet) = xTo(false, signed int);
                                                                                                        {
                                                                                                            bUpdate = true;
                                                                                                        }
                                                                                                    }
                                                                                                }

                                                                                                else if (::m_bHasHelmetSize >= 9)
                                                                                                {
                                                                                                    if (*(signed short*)((unsigned char*)pBaseEntity + ::m_bHasHelmet))
                                                                                                    {
                                                                                                        *(signed short*)((unsigned char*)pBaseEntity + ::m_bHasHelmet) = xTo(false, signed short);
                                                                                                        {
                                                                                                            bUpdate = true;
                                                                                                        }
                                                                                                    }
                                                                                                }

                                                                                                else if (::m_bHasHelmetSize >= 2)
                                                                                                {
                                                                                                    if (*(signed char*)((unsigned char*)pBaseEntity + ::m_bHasHelmet))
                                                                                                    {
                                                                                                        *(signed char*)((unsigned char*)pBaseEntity + ::m_bHasHelmet) = xTo(false, signed char);
                                                                                                        {
                                                                                                            bUpdate = true;
                                                                                                        }
                                                                                                    }
                                                                                                }

                                                                                                else
                                                                                                {
                                                                                                    if (*(bool*)((unsigned char*)pBaseEntity + ::m_bHasHelmet))
                                                                                                    {
                                                                                                        *(bool*)((unsigned char*)pBaseEntity + ::m_bHasHelmet) = false;
                                                                                                        {
                                                                                                            bUpdate = true;
                                                                                                        }
                                                                                                    }
                                                                                                }
                                                                                            }

                                                                                            else
                                                                                            {
                                                                                                if (::m_bHasHelmetSize >= 17)
                                                                                                {
                                                                                                    if (!(*(signed int*)((unsigned char*)pBaseEntity + ::m_bHasHelmet)))
                                                                                                    {
                                                                                                        *(signed int*)((unsigned char*)pBaseEntity + ::m_bHasHelmet) = xTo(true, signed int);
                                                                                                        {
                                                                                                            bUpdate = true;
                                                                                                        }
                                                                                                    }
                                                                                                }

                                                                                                else if (::m_bHasHelmetSize >= 9)
                                                                                                {
                                                                                                    if (!(*(signed short*)((unsigned char*)pBaseEntity + ::m_bHasHelmet)))
                                                                                                    {
                                                                                                        *(signed short*)((unsigned char*)pBaseEntity + ::m_bHasHelmet) = xTo(true, signed short);
                                                                                                        {
                                                                                                            bUpdate = true;
                                                                                                        }
                                                                                                    }
                                                                                                }

                                                                                                else if (::m_bHasHelmetSize >= 2)
                                                                                                {
                                                                                                    if (!(*(signed char*)((unsigned char*)pBaseEntity + ::m_bHasHelmet)))
                                                                                                    {
                                                                                                        *(signed char*)((unsigned char*)pBaseEntity + ::m_bHasHelmet) = xTo(true, signed char);
                                                                                                        {
                                                                                                            bUpdate = true;
                                                                                                        }
                                                                                                    }
                                                                                                }

                                                                                                else
                                                                                                {
                                                                                                    if (!(*(bool*)((unsigned char*)pBaseEntity + ::m_bHasHelmet)))
                                                                                                    {
                                                                                                        *(bool*)((unsigned char*)pBaseEntity + ::m_bHasHelmet) = true;
                                                                                                        {
                                                                                                            bUpdate = true;
                                                                                                        }
                                                                                                    }
                                                                                                }
                                                                                            }

                                                                                            if (bUpdate)
                                                                                            {
                                                                                                if (::g_pSharedChangeInfo)
                                                                                                {
                                                                                                    pEdict->StateChanged(xTo(::m_bHasHelmet, unsigned short));
                                                                                                }

                                                                                                else
                                                                                                {
                                                                                                    pEdict->m_fStateFlags |= FL_EDICT_CHANGED;
                                                                                                }
                                                                                            }
                                                                                        }
                                                                                    }

                                                                                    else if (::g_nArmor > 1)
                                                                                    {
                                                                                        bUpdate = { };

                                                                                        if (::m_ArmorValueSize >= 17)
                                                                                        {
                                                                                            if (*(signed int*)((unsigned char*)pBaseEntity + ::m_ArmorValue) != 100)
                                                                                            {
                                                                                                *(signed int*)((unsigned char*)pBaseEntity + ::m_ArmorValue) = 100;
                                                                                                {
                                                                                                    bUpdate = true;
                                                                                                }
                                                                                            }
                                                                                        }

                                                                                        else if (::m_ArmorValueSize >= 9)
                                                                                        {
                                                                                            if (*(signed short*)((unsigned char*)pBaseEntity + ::m_ArmorValue) != 100)
                                                                                            {
                                                                                                *(signed short*)((unsigned char*)pBaseEntity + ::m_ArmorValue) = 100;
                                                                                                {
                                                                                                    bUpdate = true;
                                                                                                }
                                                                                            }
                                                                                        }

                                                                                        else
                                                                                        {
                                                                                            if (*(signed char*)((unsigned char*)pBaseEntity + ::m_ArmorValue) != 100)
                                                                                            {
                                                                                                *(signed char*)((unsigned char*)pBaseEntity + ::m_ArmorValue) = 100;
                                                                                                {
                                                                                                    bUpdate = true;
                                                                                                }
                                                                                            }
                                                                                        }

                                                                                        if (bUpdate)
                                                                                        {
                                                                                            if (::g_pSharedChangeInfo)
                                                                                            {
                                                                                                pEdict->StateChanged(xTo(::m_ArmorValue, unsigned short));
                                                                                            }

                                                                                            else
                                                                                            {
                                                                                                pEdict->m_fStateFlags |= FL_EDICT_CHANGED;
                                                                                            }
                                                                                        }

                                                                                        if (::m_bHasHelmet)
                                                                                        {
                                                                                            bUpdate = { };

                                                                                            if (::m_bHasHelmetSize >= 17)
                                                                                            {
                                                                                                if (!(*(signed int*)((unsigned char*)pBaseEntity + ::m_bHasHelmet)))
                                                                                                {
                                                                                                    *(signed int*)((unsigned char*)pBaseEntity + ::m_bHasHelmet) = xTo(true, signed int);
                                                                                                    {
                                                                                                        bUpdate = true;
                                                                                                    }
                                                                                                }
                                                                                            }

                                                                                            else if (::m_bHasHelmetSize >= 9)
                                                                                            {
                                                                                                if (!(*(signed short*)((unsigned char*)pBaseEntity + ::m_bHasHelmet)))
                                                                                                {
                                                                                                    *(signed short*)((unsigned char*)pBaseEntity + ::m_bHasHelmet) = xTo(true, signed short);
                                                                                                    {
                                                                                                        bUpdate = true;
                                                                                                    }
                                                                                                }
                                                                                            }

                                                                                            else if (::m_bHasHelmetSize >= 2)
                                                                                            {
                                                                                                if (!(*(signed char*)((unsigned char*)pBaseEntity + ::m_bHasHelmet)))
                                                                                                {
                                                                                                    *(signed char*)((unsigned char*)pBaseEntity + ::m_bHasHelmet) = xTo(true, signed char);
                                                                                                    {
                                                                                                        bUpdate = true;
                                                                                                    }
                                                                                                }
                                                                                            }

                                                                                            else
                                                                                            {
                                                                                                if (!(*(bool*)((unsigned char*)pBaseEntity + ::m_bHasHelmet)))
                                                                                                {
                                                                                                    *(bool*)((unsigned char*)pBaseEntity + ::m_bHasHelmet) = true;
                                                                                                    {
                                                                                                        bUpdate = true;
                                                                                                    }
                                                                                                }
                                                                                            }

                                                                                            if (bUpdate)
                                                                                            {
                                                                                                if (::g_pSharedChangeInfo)
                                                                                                {
                                                                                                    pEdict->StateChanged(xTo(::m_bHasHelmet, unsigned short));
                                                                                                }

                                                                                                else
                                                                                                {
                                                                                                    pEdict->m_fStateFlags |= FL_EDICT_CHANGED;
                                                                                                }
                                                                                            }
                                                                                        }
                                                                                    }

                                                                                    else
                                                                                    {
                                                                                        bUpdate = { };

                                                                                        if (::m_ArmorValueSize >= 17)
                                                                                        {
                                                                                            if (*(signed int*)((unsigned char*)pBaseEntity + ::m_ArmorValue) != 100)
                                                                                            {
                                                                                                *(signed int*)((unsigned char*)pBaseEntity + ::m_ArmorValue) = 100;
                                                                                                {
                                                                                                    bUpdate = true;
                                                                                                }
                                                                                            }
                                                                                        }

                                                                                        else if (::m_ArmorValueSize >= 9)
                                                                                        {
                                                                                            if (*(signed short*)((unsigned char*)pBaseEntity + ::m_ArmorValue) != 100)
                                                                                            {
                                                                                                *(signed short*)((unsigned char*)pBaseEntity + ::m_ArmorValue) = 100;
                                                                                                {
                                                                                                    bUpdate = true;
                                                                                                }
                                                                                            }
                                                                                        }

                                                                                        else
                                                                                        {
                                                                                            if (*(signed char*)((unsigned char*)pBaseEntity + ::m_ArmorValue) != 100)
                                                                                            {
                                                                                                *(signed char*)((unsigned char*)pBaseEntity + ::m_ArmorValue) = 100;
                                                                                                {
                                                                                                    bUpdate = true;
                                                                                                }
                                                                                            }
                                                                                        }

                                                                                        if (bUpdate)
                                                                                        {
                                                                                            if (::g_pSharedChangeInfo)
                                                                                            {
                                                                                                pEdict->StateChanged(xTo(::m_ArmorValue, unsigned short));
                                                                                            }

                                                                                            else
                                                                                            {
                                                                                                pEdict->m_fStateFlags |= FL_EDICT_CHANGED;
                                                                                            }
                                                                                        }
                                                                                    }

                                                                                    ::g_bArmor[nPlayer] = true;
                                                                                }
                                                                            }

                                                                            else
                                                                            {
                                                                                ::g_bArmor[nPlayer] = { };
                                                                            }
                                                                        }

                                                                        else
                                                                        {
                                                                            ::g_bArmor[nPlayer] = { };
                                                                        }
                                                                    }

                                                                    else
                                                                    {
                                                                        if (::g_nArmor)
                                                                        {
                                                                            ::g_bArmor[nPlayer] = true;
                                                                        }
                                                                    }
                                                                }

                                                                else
                                                                {
                                                                    if (nHealth < 1)
                                                                    {
                                                                        if (::g_nArmor)
                                                                        {
                                                                            ::g_bArmor[nPlayer] = { };
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
                                                                    ::g_bArmor[nPlayer] = { };
                                                                }
                                                            }
                                                        }

                                                        else
                                                        {
                                                            if (::g_nArmor)
                                                            {
                                                                ::g_bArmor[nPlayer] = { };
                                                            }
                                                        }
                                                    }

                                                    else
                                                    {
                                                        if (::g_nArmor)
                                                        {
                                                            ::g_bArmor[nPlayer] = { };
                                                        }
                                                    }
                                                }

                                                else
                                                {
                                                    if (::g_nArmor)
                                                    {
                                                        ::g_bArmor[nPlayer] = { };
                                                    }
                                                }
                                            }

                                            else
                                            {
                                                if (::g_nArmor)
                                                {
                                                    ::g_bArmor[nPlayer] = { };
                                                }
                                            }
                                        }

                                        else
                                        {
                                            if (::g_nArmor)
                                            {
                                                ::g_bArmor[nPlayer] = { };
                                            }
                                        }
                                    }

                                    else
                                    {
                                        if (::g_nArmor)
                                        {
                                            ::g_bArmor[nPlayer] = { };
                                        }
                                    }
                                }

                                else
                                {
                                    if (::g_nArmor)
                                    {
                                        ::g_bArmor[nPlayer] = { };
                                    }
                                }
                            }

                            else
                            {
                                if (::g_nArmor)
                                {
                                    ::g_bArmor[nPlayer] = { };
                                }
                            }
                        }

                        else
                        {
                            if (::g_nArmor)
                            {
                                ::g_bArmor[nPlayer] = { };
                            }
                        }
                    }

                    else
                    {
                        if (::g_nArmor)
                        {
                            ::g_bArmor[nPlayer] = { };
                        }
                    }
                }

                else
                {
                    if (::g_nArmor)
                    {
                        ::g_bArmor[nPlayer] = { };
                    }
                }
            }

            else
            {
                if (::g_nArmor)
                {
                    ::g_bArmor[nPlayer] = { };
                }
            }
        }

        if (::g_bHourlyRestart)
        {
            llTime = ::std::time(nullptr);
            {
                if (llTime - llStamp > 0)
                {
                    if (!((llTime + 5) % 3600))
                    {
                        if (::numUsersPlaying() < 1)
                        {
                            ::engine->ServerCommand("say Restarting The Map!\n");
                            {
                                llStamp = llTime;
                            }
                        }
                    }

                    else if (!(llTime % 3600))
                    {
                        if (::numUsersPlaying() < 1)
                        {
                            strCmd.assign("changelevel");
                            {
                                strCmd.append(" ");
                                {
                                    strCmd.append("\"");
                                    {
                                        strCmd.append(::gpGlobals->mapname.ToCStr());
                                        {
                                            strCmd.append("\"");
                                            {
                                                strCmd.append("\n");
                                                {
                                                    strCmd.shrink_to_fit();
                                                    {
                                                        ::engine->ServerCommand(strCmd.c_str());
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
        }

        nIter = { };
    }

    if (!::g_bNoReturn)
    {
        do
        {
            ::g_SHPtr->SetRes(::MRES_IGNORED);

            return;
        }

        while (false);
    }

    else
    {
        return;
    }
}

float ::MySmmPlugin::Hook_GetTickInterval_Post() const noexcept
{
    if (!::g_bNoReturn)
    {
        if (::g_fIntervalPerTick)
        {
            do
            {
                ::g_SHPtr->SetRes(::MRES_SUPERCEDE);

                return ::g_fIntervalPerTick;
            }

            while (false);
        }

        else
        {
            do
            {
                ::g_SHPtr->SetRes(::MRES_IGNORED);

                if (::g_SHPtr->GetOrigRet())
                {
                    return (*(float*) ::g_SHPtr->GetOrigRet());
                }

                return { };
            }

            while (false);
        }
    }

    else
    {
        return { };
    }

    return { };
}

bool ::MySmmPlugin::Hook_ClientConnect_Post(::edict_t* pEdict, const char*, const char*, char*, int) noexcept
{
    static int nPlayer{ };

    if (pEdict)
    {

#if SOURCE_ENGINE != SE_CSGO

        if ((nPlayer = ::engine->IndexOfEdict(pEdict)) > 0)

#else

        if ((nPlayer = pEdict - ::gpGlobals->pEdicts) > 0)

#endif

        {
            if (::g_nArmor)
            {
                ::g_bArmor[nPlayer] = { };
            }

            if (::g_bRealScore)
            {
                ::g_nDeaths[nPlayer] = { };
                {
                    ::g_nKills[nPlayer] = { };
                }
            }

            if (!::g_strWelcome_1.empty())
            {
                if (::g_strWelcome_1.length() <= 190)
                {
                    if (::g_strWelcome_1.size() <= 190)
                    {
                        if (::g_nTextMsg > 0)
                        {
                            ::g_bShow[nPlayer] = { };
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
            ::g_SHPtr->SetRes(::MRES_IGNORED);

            if (::g_SHPtr->GetOrigRet())
            {
                return (*(bool*) ::g_SHPtr->GetOrigRet());
            }

            return { };
        }

        while (false);
    }

    else
    {
        return { };
    }

    return { };
}

void ::MySmmPlugin::Hook_ClientPutInServer_Post(::edict_t* pEdict, const char*) noexcept
{
    static int nPlayer{ };

    if (pEdict)
    {

#if SOURCE_ENGINE != SE_CSGO

        if ((nPlayer = ::engine->IndexOfEdict(pEdict)) > 0)

#else

        if ((nPlayer = pEdict - ::gpGlobals->pEdicts) > 0)

#endif

        {
            if (::g_nArmor)
            {
                ::g_bArmor[nPlayer] = { };
            }

            if (::g_bRealScore)
            {
                ::g_nDeaths[nPlayer] = { };
                {
                    ::g_nKills[nPlayer] = { };
                }
            }

            if (!::g_strWelcome_1.empty())
            {
                if (::g_strWelcome_1.length() <= 190)
                {
                    if (::g_strWelcome_1.size() <= 190)
                    {
                        if (::g_nTextMsg > 0)
                        {
                            ::g_bShow[nPlayer] = { };
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
            ::g_SHPtr->SetRes(::MRES_IGNORED);

            return;
        }

        while (false);
    }

    else
    {
        return;
    }
}

void ::MySmmPlugin::Hook_ClientDisconnect_Post(::edict_t* pEdict) noexcept
{
    static int nPlayer{ };

    if (pEdict)
    {

#if SOURCE_ENGINE != SE_CSGO

        if ((nPlayer = ::engine->IndexOfEdict(pEdict)) > 0)

#else

        if ((nPlayer = pEdict - ::gpGlobals->pEdicts) > 0)

#endif

        {
            if (::g_nArmor)
            {
                ::g_bArmor[nPlayer] = { };
            }

            if (::g_bRealScore)
            {
                ::g_nDeaths[nPlayer] = { };
                {
                    ::g_nKills[nPlayer] = { };
                }
            }

            if (!::g_strWelcome_1.empty())
            {
                if (::g_strWelcome_1.length() <= 190)
                {
                    if (::g_strWelcome_1.size() <= 190)
                    {
                        if (::g_nTextMsg > 0)
                        {
                            ::g_bShow[nPlayer] = { };
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
            ::g_SHPtr->SetRes(::MRES_IGNORED);

            return;
        }

        while (false);
    }

    else
    {
        return;
    }
}

const char* ::MySmmPlugin::GetAuthor() noexcept
{
    return "Hattrick HKS";
}

const char* ::MySmmPlugin::GetName() noexcept
{
    return xName;
}

const char* ::MySmmPlugin::GetDescription() noexcept
{
    return "Helps Owners Manage Their Game Server";
}

const char* ::MySmmPlugin::GetURL() noexcept
{
    return "https://hattrick.go.ro/";
}

const char* ::MySmmPlugin::GetLicense() noexcept
{
    return "MIT";
}

const char* ::MySmmPlugin::GetVersion() noexcept
{
    return __DATE__;
}

const char* ::MySmmPlugin::GetDate() noexcept
{
    return __DATE__;
}

const char* ::MySmmPlugin::GetLogTag() noexcept
{
    return "HGSM";
}

#ifdef WIN32

constexpr bool ::MySmmPlugin::RegisterConCommandBase(::ConCommandBase* pConCommandBase) noexcept

#else

bool ::MySmmPlugin::RegisterConCommandBase(::ConCommandBase* pConCommandBase) noexcept

#endif

{
    return ::g_SMAPI->RegisterConCommandBase(::g_PLAPI, pConCommandBase);
}

void ::MySmmPlugin::FireGameEvent(::IGameEvent* pGameEvent) noexcept
{
    static const char* pszGameEventName{ };
    static int nUserId{ }, nPlayer{ }, nUserIdAttacker{ }, nUserIdActual{ }, nAttacker{ };
    static bool bConVars{ }, bValidAttacker{ }, bValidVictim{ };

    static ::ConVar* pFriendlyFire{ };
    static ::edict_t* pEdict{ };
    static ::CBaseEntity* pBaseEntity{ };
    static ::IPlayerInfo* pPlayerInfo{ }, * pPlayerInfoVictim{ }, * pPlayerInfoAttacker{ };
    static ::IServerUnknown* pServerUnknown{ };
    static ::IServerEntity* pServerEntity{ };

    if (::g_bHooked)
    {
        if (::g_bEvHooked)
        {
            if (pGameEvent)
            {
                if (pszGameEventName = pGameEvent->GetName())
                {
                    if (*pszGameEventName)
                    {
                        if (!::std::strcmp(pszGameEventName, "player_spawn"))
                        {
                            if ((nUserId = pGameEvent->GetInt("userid", -1)) > -1)
                            {
                                for (nPlayer = xTo(true, int); nPlayer <= ::gpGlobals->maxClients; nPlayer++)
                                {
                                    if (::g_nArmor)
                                    {
                                        if (::g_bArmor[nPlayer])
                                        {

#if SOURCE_ENGINE != SE_CSGO

                                            if (pEdict = ::engine->PEntityOfEntIndex(nPlayer))

#else

                                            if (pEdict = ::gpGlobals->pEdicts + nPlayer)

#endif

                                            {
                                                if (!(pEdict->m_fStateFlags & FL_EDICT_FREE))
                                                {
                                                    if (!(pEdict->m_fStateFlags & FL_EDICT_DONTSEND))
                                                    {
                                                        if (pEdict->m_NetworkSerialNumber)
                                                        {
                                                            if (pEdict->m_pNetworkable)
                                                            {
                                                                if (pServerEntity = pEdict->GetIServerEntity())
                                                                {
                                                                    if (pServerUnknown = pEdict->GetUnknown())
                                                                    {
                                                                        if (pBaseEntity = pServerEntity->GetBaseEntity())
                                                                        {
                                                                            if (pPlayerInfo = ::playerinfomanager->GetPlayerInfo(pEdict))
                                                                            {
                                                                                if (pPlayerInfo->IsPlayer())
                                                                                {
                                                                                    if (pPlayerInfo->IsConnected())
                                                                                    {
                                                                                        if (pPlayerInfo->GetTeamIndex() > 0)
                                                                                        {
                                                                                            if (!pPlayerInfo->IsDead())
                                                                                            {
                                                                                                if (pPlayerInfo->GetUserID() == nUserId)
                                                                                                {
                                                                                                    ::g_bArmor[nPlayer] = { };
                                                                                                }

                                                                                                else
                                                                                                {

                                                                                                }
                                                                                            }

                                                                                            else
                                                                                            {
                                                                                                ::g_bArmor[nPlayer] = { };
                                                                                            }
                                                                                        }

                                                                                        else
                                                                                        {
                                                                                            ::g_bArmor[nPlayer] = { };
                                                                                        }
                                                                                    }

                                                                                    else
                                                                                    {
                                                                                        ::g_bArmor[nPlayer] = { };
                                                                                    }
                                                                                }

                                                                                else
                                                                                {
                                                                                    ::g_bArmor[nPlayer] = { };
                                                                                }
                                                                            }

                                                                            else
                                                                            {
                                                                                ::g_bArmor[nPlayer] = { };
                                                                            }
                                                                        }

                                                                        else
                                                                        {
                                                                            ::g_bArmor[nPlayer] = { };
                                                                        }
                                                                    }

                                                                    else
                                                                    {
                                                                        ::g_bArmor[nPlayer] = { };
                                                                    }
                                                                }

                                                                else
                                                                {
                                                                    ::g_bArmor[nPlayer] = { };
                                                                }
                                                            }

                                                            else
                                                            {
                                                                ::g_bArmor[nPlayer] = { };
                                                            }
                                                        }

                                                        else
                                                        {
                                                            ::g_bArmor[nPlayer] = { };
                                                        }
                                                    }

                                                    else
                                                    {
                                                        ::g_bArmor[nPlayer] = { };
                                                    }
                                                }

                                                else
                                                {
                                                    ::g_bArmor[nPlayer] = { };
                                                }
                                            }

                                            else
                                            {
                                                ::g_bArmor[nPlayer] = { };
                                            }
                                        }
                                    }

                                    else
                                    {
                                        ::g_bArmor[nPlayer] = { };
                                    }
                                }
                            }
                        }

                        else
                        {
                            if (::g_bRealScore)
                            {
                                if (!::std::strcmp(pszGameEventName, "player_death"))
                                {
                                    if ((nUserId = pGameEvent->GetInt("userid", -1)) > -1)
                                    {
                                        if ((nUserIdAttacker = pGameEvent->GetInt("attacker", -1)) > -1)
                                        {
                                            if (nUserId != nUserIdAttacker)
                                            {
                                                if (!bConVars)
                                                {
                                                    pFriendlyFire = ::icvar->FindVar("mp_friendlyfire");
                                                    {
                                                        bConVars = true;
                                                    }
                                                }

                                                bValidVictim = { };
                                                {
                                                    bValidAttacker = { };
                                                    {
                                                        pPlayerInfoVictim = nullptr;
                                                        {
                                                            pPlayerInfoAttacker = nullptr;
                                                            {
                                                                nAttacker = { };
                                                            }
                                                        }
                                                    }
                                                }

                                                for (nPlayer = xTo(true, int); nPlayer <= ::gpGlobals->maxClients; nPlayer++)
                                                {

#if SOURCE_ENGINE != SE_CSGO

                                                    if (pEdict = ::engine->PEntityOfEntIndex(nPlayer))

#else

                                                    if (pEdict = ::gpGlobals->pEdicts + nPlayer)

#endif

                                                    {
                                                        if (!(pEdict->m_fStateFlags & FL_EDICT_FREE))
                                                        {
                                                            if (!(pEdict->m_fStateFlags & FL_EDICT_DONTSEND))
                                                            {
                                                                if (pEdict->m_NetworkSerialNumber)
                                                                {
                                                                    if (pEdict->m_pNetworkable)
                                                                    {
                                                                        if (pServerEntity = pEdict->GetIServerEntity())
                                                                        {
                                                                            if (pServerUnknown = pEdict->GetUnknown())
                                                                            {
                                                                                if (pBaseEntity = pServerEntity->GetBaseEntity())
                                                                                {
                                                                                    if (pPlayerInfo = ::playerinfomanager->GetPlayerInfo(pEdict))
                                                                                    {
                                                                                        if (pPlayerInfo->IsPlayer())
                                                                                        {
                                                                                            if (pPlayerInfo->IsConnected())
                                                                                            {
                                                                                                if (pPlayerInfo->GetTeamIndex() > 0)
                                                                                                {
                                                                                                    if ((nUserIdActual = pPlayerInfo->GetUserID()) == nUserId)
                                                                                                    {
                                                                                                        ::g_nDeaths[nPlayer]++;
                                                                                                        {
                                                                                                            bValidVictim = true;
                                                                                                            {
                                                                                                                pPlayerInfoVictim = pPlayerInfo;
                                                                                                                {
                                                                                                                    if (::g_bFixDeaths || ::g_bFixScore)
                                                                                                                    {
                                                                                                                        ::g_nDeaths[nPlayer] = ::ValClamp(::g_nDeaths[nPlayer], xTo(false, int), ::g_nKills[nPlayer]);
                                                                                                                    }
                                                                                                                }
                                                                                                            }
                                                                                                        }
                                                                                                    }

                                                                                                    else
                                                                                                    {
                                                                                                        if (nUserIdActual == nUserIdAttacker)
                                                                                                        {
                                                                                                            ::g_nKills[nPlayer]++;
                                                                                                            {
                                                                                                                bValidAttacker = true;
                                                                                                                {
                                                                                                                    pPlayerInfoAttacker = pPlayerInfo;
                                                                                                                    {
                                                                                                                        nAttacker = nPlayer;
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

                                                if (!::g_bRealScoreFriendly)
                                                {
                                                    if (pFriendlyFire)
                                                    {
                                                        if (pFriendlyFire->GetBool())
                                                        {
                                                            if (bValidAttacker)
                                                            {
                                                                if (bValidVictim)
                                                                {
                                                                    if (pPlayerInfoAttacker)
                                                                    {
                                                                        if (pPlayerInfoVictim)
                                                                        {
                                                                            if (pPlayerInfoAttacker->GetTeamIndex() == pPlayerInfoVictim->GetTeamIndex())
                                                                            {
                                                                                ::g_nKills[nAttacker]--;
                                                                                {
                                                                                    if (::g_bFriendlyDecrease)
                                                                                    {
                                                                                        --::g_nKills[nAttacker];
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

#if SOURCE_ENGINE == SE_CSGO

#ifdef WIN32

constexpr int ::MySmmPlugin::GetEventDebugID() noexcept

#else

int ::MySmmPlugin::GetEventDebugID() noexcept

#endif

{
    return EVENT_DEBUG_ID_INIT;
}

#endif

#ifdef WIN32

constexpr ::RecipientFilter::RecipientFilter() noexcept
{
    Zero();
}

constexpr ::RecipientFilter::RecipientFilter(int nRecipient) noexcept
{
    Zero();
    {
        Add(nRecipient);
    }
}

constexpr ::RecipientFilter::RecipientFilter(int nRecipient, bool bReliable) noexcept
{
    Zero();
    {
        Add(nRecipient);
        {
            m_bReliable = bReliable;
        }
    }
}

constexpr ::RecipientFilter::RecipientFilter(int nRecipient, bool bReliable, bool bInitMessage) noexcept
{
    Zero();
    {
        Add(nRecipient);
        {
            m_bReliable = bReliable;
            {
                m_bInitMessage = bInitMessage;
            }
        }
    }
}

::RecipientFilter::~RecipientFilter() noexcept
{
    Zero();
}

constexpr bool ::RecipientFilter::IsReliable() const noexcept
{
    return m_bReliable;
}

constexpr bool ::RecipientFilter::IsInitMessage() const noexcept
{
    return m_bInitMessage;
}

constexpr int ::RecipientFilter::GetRecipientCount() const noexcept
{
    return xTo(m_nCount, int);
}

constexpr int ::RecipientFilter::GetRecipientIndex(int nSlot) const noexcept
{
    return (((nSlot < 0) || (nSlot >= xTo(m_nCount, int))) ? (-1) : (m_nRecipients[nSlot]));
}

constexpr void ::RecipientFilter::Add(int nRecipient) noexcept
{
    m_nRecipients[m_nCount++] = nRecipient;
}

constexpr void ::RecipientFilter::Set(bool bReliable) noexcept
{
    m_bReliable = bReliable;
}

constexpr void ::RecipientFilter::Set(bool bReliable, bool bInitMessage) noexcept
{
    m_bReliable = bReliable;
    {
        m_bInitMessage = bInitMessage;
    }
}

constexpr void ::RecipientFilter::Zero() noexcept
{
    ::std::memset(m_nRecipients, { }, sizeof m_nRecipients);
    {
        m_nCount = { };
        {
            m_bReliable = { };
            {
                m_bInitMessage = { };
            }
        }
    }
}

#else

::RecipientFilter::RecipientFilter() noexcept
{
    Zero();
}

::RecipientFilter::RecipientFilter(int nRecipient) noexcept
{
    Zero();
    {
        Add(nRecipient);
    }
}

::RecipientFilter::RecipientFilter(int nRecipient, bool bReliable) noexcept
{
    Zero();
    {
        Add(nRecipient);
        {
            m_bReliable = bReliable;
        }
    }
}

::RecipientFilter::RecipientFilter(int nRecipient, bool bReliable, bool bInitMessage) noexcept
{
    Zero();
    {
        Add(nRecipient);
        {
            m_bReliable = bReliable;
            {
                m_bInitMessage = bInitMessage;
            }
        }
    }
}

::RecipientFilter::~RecipientFilter() noexcept
{
    Zero();
}

bool ::RecipientFilter::IsReliable() const noexcept
{
    return m_bReliable;
}

bool ::RecipientFilter::IsInitMessage() const noexcept
{
    return m_bInitMessage;
}

int ::RecipientFilter::GetRecipientCount() const noexcept
{
    return xTo(m_nCount, int);
}

int ::RecipientFilter::GetRecipientIndex(int nSlot) const noexcept
{
    return (((nSlot < 0) || (nSlot >= xTo(m_nCount, int))) ? (-1) : (m_nRecipients[nSlot]));
}

void ::RecipientFilter::Add(int nRecipient) noexcept
{
    m_nRecipients[m_nCount++] = nRecipient;
}

void ::RecipientFilter::Set(bool bReliable) noexcept
{
    m_bReliable = bReliable;
}

void ::RecipientFilter::Set(bool bReliable, bool bInitMessage) noexcept
{
    m_bReliable = bReliable;
    {
        m_bInitMessage = bInitMessage;
    }
}

void ::RecipientFilter::Zero() noexcept
{
    ::std::memset(m_nRecipients, { }, sizeof m_nRecipients);
    {
        m_nCount = { };
        {
            m_bReliable = { };
            {
                m_bInitMessage = { };
            }
        }
    }
}

#endif

#if SOURCE_ENGINE == SE_CSGO

#ifndef WIN32

bool ::CustomConVar::operator < (const ::CustomConVar& _) const noexcept
{
    return (strName.compare(_.strName) < 0);
}

bool ::CustomConVar::operator > (const ::CustomConVar& _) const noexcept
{
    return (strName.compare(_.strName) > 0);
}

bool ::CustomConVar::operator == (const ::CustomConVar& _) const noexcept
{
    return (!(strName.compare(_.strName)));
}

#endif

#endif

#ifdef WIN32

constexpr void* __CreateISmmPluginISmmPlugin_interface() noexcept

#else

void* __CreateISmmPluginISmmPlugin_interface() noexcept

#endif

{
    return (static_cast <::ISmmPlugin*> (&::g_MySmmPlugin));
}

#ifdef WIN32

constexpr ::datamap_t* FindDataMap(::CBaseEntity* pBaseEntity) noexcept

#else

::datamap_t* FindDataMap(::CBaseEntity* pBaseEntity) noexcept

#endif

{
    if (!pBaseEntity)
    {
        return nullptr;
    }

    void** pTable = *reinterpret_cast <void***> (pBaseEntity);
    {
        if (!pTable)
        {
            return nullptr;
        }
    }

#ifdef WIN32

    void* pFunc = pTable[::x_nDataMap];
    {
        if (!pFunc)
        {
            return nullptr;
        }
    }

    union
    {
        ::datamap_t* (::DataMap::* pDataMap) ();

        void* pAddr;

    } u;

    u.pAddr = pFunc;

#else

    void* pFunc = pTable[::x_nDataMap];
    {
        if (!pFunc)
        {
            return nullptr;
        }
    }

    union
    {
        ::datamap_t* (::DataMap::* pDataMap) ();

        struct
        {
            void* pAddr;

            int nAdj;

        } s;

    } u;

    u.s.pAddr = pFunc;
    {
        u.s.nAdj = { };
    }

#endif

    void** pThis = *reinterpret_cast <void***> (&pBaseEntity);
    {
        if (!pThis)
        {
            return nullptr;
        }
    }

    return (::datamap_t*)(reinterpret_cast <::DataMap*> (pThis)->*u.pDataMap) ();
}

#ifdef WIN32

constexpr bool FindDataMapInfo(::datamap_t* pDataMap, const char* pszName, ::DataTableInfo* pDataTableInfo) noexcept

#else

bool FindDataMapInfo(::datamap_t* pDataMap, const char* pszName, ::DataTableInfo* pDataTableInfo) noexcept

#endif

{
    if (!pDataMap)
    {
        return { };
    }

    else if (!pszName)
    {
        return { };
    }

    else if (!pDataTableInfo)
    {
        return { };
    }

    while (pDataMap)
    {
        for (int nIter = { }; nIter < pDataMap->dataNumFields; ++nIter)
        {
            if (!(pDataMap->dataDesc))
            {
                continue;
            }

            else if (!((pDataMap->dataDesc[nIter]).fieldName))
            {
                continue;
            }

            else if (!::std::strcmp(pszName, pDataMap->dataDesc[nIter].fieldName))
            {
                pDataTableInfo->pTypeDescription = &(pDataMap->dataDesc[nIter]);
                {

#if SOURCE_ENGINE == SE_CSGO

                    pDataTableInfo->uOffs = pDataTableInfo->pTypeDescription->fieldOffset;

#else

                    pDataTableInfo->uOffs = *pDataTableInfo->pTypeDescription->fieldOffset;

#endif

                    {
                        return true;
                    }
                }
            }

            else if (!((pDataMap->dataDesc[nIter]).td))
            {
                continue;
            }

            else if (!::FindDataMapInfo(pDataMap->dataDesc[nIter].td, pszName, pDataTableInfo))
            {
                continue;
            }

#if SOURCE_ENGINE != SE_CSGO

            pDataTableInfo->uOffs += *(&(pDataMap->dataDesc[nIter]))->fieldOffset;

#else

            pDataTableInfo->uOffs += (&(pDataMap->dataDesc[nIter]))->fieldOffset;

#endif

            {
                return true;
            }
        }

        pDataMap = pDataMap->baseMap;
    }

    return { };
}

#ifdef WIN32

constexpr int DataSizeByFieldType(::fieldtype_t nType, int nFlags) noexcept

#else

int DataSizeByFieldType(::fieldtype_t nType, int nFlags) noexcept

#endif

{
    switch (nType)
    {
    case ::FIELD_TICK:
    case ::FIELD_MODELINDEX:
    case ::FIELD_MATERIALINDEX:
    case ::FIELD_INTEGER:
    case ::FIELD_COLOR32:
    {
        return 32;
    }

    case ::FIELD_CUSTOM:
    {
        if ((nFlags & FTYPEDESC_OUTPUT) == FTYPEDESC_OUTPUT)
        {
            return 32;
        }

        break;
    }

    case ::FIELD_SHORT:
    {
        return 16;
    }

    case ::FIELD_CHARACTER:
    {
        return 8;
    }

    case ::FIELD_BOOLEAN:
    {
        return xTo(true, int);
    }

    default:
    {
        return { };
    }
    }

    return { };
}

#ifdef WIN32

constexpr ::ServerClass* FindServerClass(const char* pszServerClassName) noexcept

#else

::ServerClass* FindServerClass(const char* pszServerClassName) noexcept

#endif

{
    if (!pszServerClassName)
    {
        return nullptr;
    }

    ::ServerClass* pServerClass = ::server->GetAllServerClasses();
    {
        while (pServerClass)
        {
            const char* pszClassName = pServerClass->GetName();
            {
                if (pszClassName)
                {
                    if (!::std::strcmp(pszServerClassName, pszClassName))
                    {
                        return pServerClass;
                    }
                }
            }

            pServerClass = pServerClass->m_pNext;
        }
    }

    return nullptr;
}

#ifdef WIN32

constexpr bool FindInSendTable(::SendTable* pSendTable, const char* pszName, ::SendPropInfo* pSendPropInfo, unsigned int uOffs) noexcept

#else

bool FindInSendTable(::SendTable* pSendTable, const char* pszName, ::SendPropInfo* pSendPropInfo, unsigned int uOffs) noexcept

#endif

{
    if (!pSendTable)
    {
        return { };
    }

    else if (!pszName)
    {
        return { };
    }

    else if (!pSendPropInfo)
    {
        return { };
    }

    int nProps = pSendTable->GetNumProps();
    {
        for (int nIter = { }; nIter < nProps; nIter++)
        {
            ::SendProp* pSendProp = pSendTable->GetProp(nIter);

            if (!pSendProp)
            {
                continue;
            }

            else if (pSendProp->IsInsideArray())
            {
                continue;
            }

            ::SendTable* pInnerSendTable = pSendProp->GetDataTable();

            const char* pszSendPropName = pSendProp->GetName();

            if (pszSendPropName && !::std::strcmp(pszName, pszSendPropName))
            {
                if (!pSendProp->GetOffset() && pInnerSendTable && pInnerSendTable->GetNumProps())
                {
                    ::SendProp* pLengthProxy = pInnerSendTable->GetProp({ });
                    {
                        if (pLengthProxy)
                        {
                            const char* pszLengthProxyName = pLengthProxy->GetName();

                            if (pszLengthProxyName && !::std::strcmp(pszLengthProxyName, "lengthproxy") && pLengthProxy->GetExtraData())
                            {
                                pSendPropInfo->pSendProp = pSendProp;
                                {
                                    pSendPropInfo->uOffs = uOffs + *reinterpret_cast <unsigned int*> (reinterpret_cast <int> (pLengthProxy->GetExtraData()) + ::x_nSendProp);
                                    {
                                        return true;
                                    }
                                }
                            }
                        }
                    }
                }

                pSendPropInfo->pSendProp = pSendProp;
                {
                    pSendPropInfo->uOffs = uOffs + pSendPropInfo->pSendProp->GetOffset();
                    {
                        return true;
                    }
                }
            }

            if (pInnerSendTable)
            {
                if (::FindInSendTable(pInnerSendTable, pszName, pSendPropInfo, uOffs + pSendProp->GetOffset()))
                {
                    return true;
                }
            }
        }
    }

    return { };
}

#ifdef WIN32

constexpr bool FindSendPropInfo(const char* pszServerClassName, const char* pszOffs, ::SendPropInfo* pSendPropInfo) noexcept

#else

bool FindSendPropInfo(const char* pszServerClassName, const char* pszOffs, ::SendPropInfo* pSendPropInfo) noexcept

#endif

{
    if (!pszServerClassName)
    {
        return { };
    }

    else if (!pszOffs)
    {
        return { };
    }

    ::ServerClass* pServerClass = ::FindServerClass(pszServerClassName);
    {
        if (!pServerClass)
        {
            return { };
        }
    }

    ::SendPropInfo sendPropInfo{ };
    {
        if (!::FindInSendTable(pServerClass->m_pTable, pszOffs, &sendPropInfo, { }))
        {
            return { };
        }
    }

    if (pSendPropInfo)
    {
        *pSendPropInfo = sendPropInfo;
    }

    return true;
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

        ::IChangeInfoAccessor* ::CBaseEdict::GetChangeAccessor()
        {
            return ::engine->GetChangeAccessor(((const ::edict_t*)(this)));
        }

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

        int numUsersPlaying(int nSkipThisEntityIndex) noexcept
        {
            static int nPlayer{ }, nPlaying{ };

            static ::edict_t* pEdict{ };
            static ::CBaseEntity* pBaseEntity{ };
            static ::IPlayerInfo* pPlayerInfo{ };
            static ::IServerUnknown* pServerUnknown{ };
            static ::IServerEntity* pServerEntity{ };

            for (nPlaying = xTo(false, int), nPlayer = xTo(true, int); nPlayer <= ::gpGlobals->maxClients; nPlayer++)
            {
                if (nSkipThisEntityIndex == nPlayer)
                {
                    continue;
                }

#if SOURCE_ENGINE != SE_CSGO

                if (pEdict = ::engine->PEntityOfEntIndex(nPlayer))

#else

                if (pEdict = ::gpGlobals->pEdicts + nPlayer)

#endif

                {
                    if (!(pEdict->m_fStateFlags & FL_EDICT_FREE))
                    {
                        if (!(pEdict->m_fStateFlags & FL_EDICT_DONTSEND))
                        {
                            if (pEdict->m_NetworkSerialNumber)
                            {
                                if (pEdict->m_pNetworkable)
                                {
                                    if (pServerEntity = pEdict->GetIServerEntity())
                                    {
                                        if (pServerUnknown = pEdict->GetUnknown())
                                        {
                                            if (pBaseEntity = pServerEntity->GetBaseEntity())
                                            {
                                                if (pPlayerInfo = ::playerinfomanager->GetPlayerInfo(pEdict))
                                                {
                                                    if (pPlayerInfo->IsPlayer())
                                                    {
                                                        if (pPlayerInfo->IsConnected())
                                                        {
                                                            if (pPlayerInfo->GetTeamIndex() > 1)
                                                            {
                                                                nPlaying++;
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

            return nPlaying;
        }

        int numUsersConnected(int nSkipThisEntityIndex) noexcept
        {
            static int nPlayer{ }, nConnected{ };

            static ::edict_t* pEdict{ };
            static ::CBaseEntity* pBaseEntity{ };
            static ::IPlayerInfo* pPlayerInfo{ };
            static ::IServerUnknown* pServerUnknown{ };
            static ::IServerEntity* pServerEntity{ };

            for (nConnected = xTo(false, int), nPlayer = xTo(true, int); nPlayer <= ::gpGlobals->maxClients; nPlayer++)
            {
                if (nSkipThisEntityIndex == nPlayer)
                {
                    continue;
                }

#if SOURCE_ENGINE != SE_CSGO

                if (pEdict = ::engine->PEntityOfEntIndex(nPlayer))

#else

                if (pEdict = ::gpGlobals->pEdicts + nPlayer)

#endif

                {
                    if (!(pEdict->m_fStateFlags & FL_EDICT_FREE))
                    {
                        if (!(pEdict->m_fStateFlags & FL_EDICT_DONTSEND))
                        {
                            if (pEdict->m_NetworkSerialNumber)
                            {
                                if (pEdict->m_pNetworkable)
                                {
                                    if (pServerEntity = pEdict->GetIServerEntity())
                                    {
                                        if (pServerUnknown = pEdict->GetUnknown())
                                        {
                                            if (pBaseEntity = pServerEntity->GetBaseEntity())
                                            {
                                                if (pPlayerInfo = ::playerinfomanager->GetPlayerInfo(pEdict))
                                                {
                                                    if (pPlayerInfo->IsPlayer())
                                                    {
                                                        if (pPlayerInfo->IsConnected())
                                                        {
                                                            nConnected++;
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

            return nConnected;
        }
