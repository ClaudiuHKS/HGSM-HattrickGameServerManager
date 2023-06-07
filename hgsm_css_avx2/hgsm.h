
#pragma once

#include "nlohmann/json.hpp"

#include <fstream>

#ifndef WIN32

#include <sys/resource.h>

#include <dirent.h>

#else

#include <Windows.h>

#endif

#ifdef GetProp

#undef GetProp

#endif

#ifdef SetProp

#undef SetProp

#endif

#include <sh_vector.h>
#include <ISmmPlugin.h>
#include <filesystem.h>
#include <igameevents.h>
#include <iplayerinfo.h>
#include <inetchannel.h>
#include <inetchannelinfo.h>
#include <ihandleentity.h>
#include <irecipientfilter.h>
#include <iserverentity.h>
#include <itoolsystem.h>
#include <itoolframework.h>
#include <itoolentity.h>
#include <ienginetool.h>
#include <iserverenginetools.h>

#if SOURCE_ENGINE == SE_CSGO

#include <google/protobuf/descriptor.h>
#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/descriptor_database.h>
#include <google/protobuf/dynamic_message.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/service.h>
#include <google/protobuf/message.h>
#include <google/protobuf/message_lite.h>

#endif

#include <edict.h>
#include <server_class.h>
#include <inetmessage.h>
#include <shareddefs.h>
#include <sourcehook_impl_cproto.h>

#if SOURCE_ENGINE == SE_CSGO

#include <cstrike15_usermessages.pb.h>
#include <cstrike15_usermessage_helpers.h>

#endif

#include <usermessages.h>

#if SOURCE_ENGINE == SE_CSGO

#include <netmessages.pb.h>

#endif

#define xName "Hattrick Game Server Manager"

#define xTo( Var, Type ) ( ( Type ) ( Var ) )

#if SOURCE_ENGINE == SE_CSGO

#define xConVarsTotalRounded ( 2560 )

#endif

class MySmmPlugin : public ::ISmmPlugin, public ::IMetamodListener, public ::IConCommandBaseAccessor, public ::IGameEventListener2
{
public:

    bool Load(::PluginId, ::ISmmAPI*, char*, unsigned int, bool) noexcept;
    bool Unload(char*, unsigned int) noexcept;

    bool Pause(char*, unsigned int) noexcept;
    bool Unpause(char*, unsigned int) noexcept;

    void AllPluginsLoaded() noexcept;

public:

    void OnVSPListening(::IServerPluginCallbacks*) noexcept;

public:

    float Hook_GetTickInterval() const noexcept;
    bool Hook_LevelInit(const char*, const char*, const char*, const char*, bool, bool) noexcept;
    void Hook_LevelShutdown() noexcept;

    bool Hook_ClientConnect(::edict_t*, const char*, const char*, char*, int) noexcept;
    void Hook_ClientDisconnect(::edict_t*) noexcept;

    void Hook_ServerActivate_Post(::edict_t*, int, int) noexcept;
    bool Hook_LevelInit_Post(const char*, const char*, const char*, const char*, bool, bool) noexcept;
    void Hook_LevelShutdown_Post() noexcept;
    void Hook_GameFrame_Post(bool) noexcept;
    float Hook_GetTickInterval_Post() const noexcept;

    bool Hook_ClientConnect_Post(::edict_t*, const char*, const char*, char*, int) noexcept;
    void Hook_ClientPutInServer_Post(::edict_t*, const char*) noexcept;
    void Hook_ClientDisconnect_Post(::edict_t*) noexcept;

public:

    const char* GetAuthor() noexcept;
    const char* GetName() noexcept;
    const char* GetDescription() noexcept;
    const char* GetURL() noexcept;
    const char* GetLicense() noexcept;
    const char* GetVersion() noexcept;
    const char* GetDate() noexcept;
    const char* GetLogTag() noexcept;

public:

#ifdef WIN32

    constexpr bool RegisterConCommandBase(::ConCommandBase*) noexcept;

#else

    bool RegisterConCommandBase(::ConCommandBase*) noexcept;

#endif

public:

    void FireGameEvent(::IGameEvent*) noexcept;

#if SOURCE_ENGINE == SE_CSGO

#ifdef WIN32

    constexpr int GetEventDebugID() noexcept;

#else

    int GetEventDebugID() noexcept;

#endif

#endif

};

#ifdef WIN32

class RecipientFilter : public ::IRecipientFilter
{
public:

    constexpr RecipientFilter() noexcept;

public:

    constexpr RecipientFilter(int) noexcept;
    constexpr RecipientFilter(int, bool) noexcept;
    constexpr RecipientFilter(int, bool, bool) noexcept;

public:

    ~RecipientFilter() noexcept;

public:

    constexpr bool IsReliable() const noexcept;
    constexpr bool IsInitMessage() const noexcept;

public:

    constexpr int GetRecipientCount() const noexcept;

public:

    constexpr int GetRecipientIndex(int) const noexcept;

public:

    constexpr void Add(int) noexcept;

public:

    constexpr void Set(bool) noexcept;
    constexpr void Set(bool, bool) noexcept;

public:

    constexpr void Zero() noexcept;

public:

    int m_nRecipients[255]{ };

public:

    bool m_bReliable{ };
    bool m_bInitMessage{ };

public:

    unsigned int m_nCount{ };
};

#else

class RecipientFilter : public ::IRecipientFilter
{
public:

    RecipientFilter() noexcept;

public:

    RecipientFilter(int) noexcept;
    RecipientFilter(int, bool) noexcept;
    RecipientFilter(int, bool, bool) noexcept;

public:

    ~RecipientFilter() noexcept;

public:

    bool IsReliable() const noexcept;
    bool IsInitMessage() const noexcept;

public:

    int GetRecipientCount() const noexcept;

public:

    int GetRecipientIndex(int) const noexcept;

public:

    void Add(int) noexcept;

public:

    void Set(bool) noexcept;
    void Set(bool, bool) noexcept;

public:

    void Zero() noexcept;

public:

    int m_nRecipients[255]{ };

public:

    bool m_bReliable{ };
    bool m_bInitMessage{ };

public:

    unsigned int m_nCount{ };
};

#endif

#if SOURCE_ENGINE == SE_CSGO

class CustomConVar
{
public:

    ::std::string strName{ };
    ::std::string strVal{ };
    ::std::string strDef{ };
    ::std::string strMin{ };
    ::std::string strMax{ };
    ::std::string strHelp{ };

#ifndef WIN32

public:

    bool operator < (const ::CustomConVar&) const noexcept;
    bool operator > (const ::CustomConVar&) const noexcept;
    bool operator == (const ::CustomConVar&) const noexcept;

#endif

};

#endif

class SendPropInfo
{
public:

    ::SendProp* pSendProp;

public:

    unsigned int uOffs;
};

class DataTableInfo
{
public:

    ::typedescription_t* pTypeDescription;

public:

    unsigned int uOffs;
};

class DataMap
{
public:

};

extern ::ICvar* icvar;

extern ::IVEngineServer* engine;
extern ::IGameEventManager2* gameevents;
extern ::IServerPluginHelpers* helpers;

extern ::IServerTools* servertools;
extern ::IPlayerInfoManager* playerinfomanager;
extern ::IServerGameClients* gameclients;
extern ::IServerGameDLL* server;

extern ::IFileSystem* filesystem;
extern ::IBaseFileSystem* basefilesystem;

extern ::CGlobalVars* gpGlobals;

extern ::IServerPluginCallbacks* vsp_callbacks;

extern ::CSharedEdictChangeInfo* g_pSharedChangeInfo;

extern ::ISmmAPI* g_SMAPI;

extern ::SourceHook::ISourceHook* g_SHPtr;

extern ::ISmmPlugin* g_PLAPI;

extern ::PluginId g_PLID;

extern ::MySmmPlugin g_MySmmPlugin;

extern ::ConVar g_cvVersion;

extern ::InterfaceReg __g_CreateISmmPluginISmmPlugin_reg;

extern ::std::string g_strGameDir;

extern ::std::string g_strWelcome_1;
extern ::std::string g_strWelcome_2;
extern ::std::string g_strWelcome_3;
extern ::std::string g_strWelcome_4;
extern ::std::string g_strWelcome_5;
extern ::std::string g_strWelcome_6;

extern ::std::string g_strNotify_1;
extern ::std::string g_strNotify_2;

#if SOURCE_ENGINE == SE_CSGO

extern const ::google::protobuf::Message* g_pTextMsg;

#endif

#ifdef WIN32

extern ::HWND__* g_pWnd;

#endif

extern bool g_bShow[80];

extern bool g_bArmor[80];

extern bool g_bHooked;
extern bool g_bEvHooked;
extern bool g_bNoReturn;

extern bool g_bRegenerate;
extern bool g_bRealScore;
extern bool g_bHideRadar;
extern bool g_bRealScoreFriendly;
extern bool g_bFriendlyDecrease;
extern bool g_bFixKills;
extern bool g_bFixDeaths;
extern bool g_bZeroMoney;
extern bool g_bFixScore;
extern bool g_bKeepHelmet;
extern bool g_bReliable;
extern bool g_bHourlyRestart;

extern int g_nDeaths[80];
extern int g_nKills[80];

extern int g_nTextMsg;

extern int g_nArmor;

extern int g_nTicks;

extern float g_fIntervalPerTick;

extern int m_iHealth, m_iHealthSize;
extern int m_iDeaths, m_iDeathsSize;
extern int m_iFrags, m_iFragsSize;

extern int m_ArmorValue, m_ArmorValueSize;

extern int m_bHasHelmet, m_bHasHelmetSize;

extern int m_iAccount, m_iAccountSize;

extern int m_flFlashMaxAlpha;
extern int m_flFlashDuration;

extern int x_nDataMap;
extern int x_nSendProp;

extern long long g_llMapChange;

#ifdef WIN32

extern unsigned long g_ulProc;

#endif

#ifdef WIN32

constexpr void* __CreateISmmPluginISmmPlugin_interface() noexcept;

#else

void* __CreateISmmPluginISmmPlugin_interface() noexcept;

#endif

#ifdef WIN32

constexpr ::datamap_t* FindDataMap(::CBaseEntity*) noexcept;
constexpr bool FindDataMapInfo(::datamap_t*, const char*, ::DataTableInfo*) noexcept;
constexpr int DataSizeByFieldType(::fieldtype_t, int) noexcept;
constexpr ::ServerClass* FindServerClass(const char*) noexcept;
constexpr bool FindInSendTable(::SendTable*, const char*, ::SendPropInfo*, unsigned int) noexcept;
constexpr bool FindSendPropInfo(const char*, const char*, ::SendPropInfo*) noexcept;

#else

::datamap_t* FindDataMap(::CBaseEntity*) noexcept;
bool FindDataMapInfo(::datamap_t*, const char*, ::DataTableInfo*) noexcept;
int DataSizeByFieldType(::fieldtype_t, int) noexcept;
::ServerClass* FindServerClass(const char*) noexcept;
bool FindInSendTable(::SendTable*, const char*, ::SendPropInfo*, unsigned int) noexcept;
bool FindSendPropInfo(const char*, const char*, ::SendPropInfo*) noexcept;

#endif

bool FileExists(::std::string) noexcept;
bool FileRemove(::std::string) noexcept;
bool FileCopy(::std::string, ::std::string) noexcept;
bool FileDirectory(::std::string) noexcept;

#ifdef WIN32

constexpr int ValClamp(int, int, int) noexcept;
constexpr float ValClamp(float, float, float) noexcept;

#else

int ValClamp(int, int, int) noexcept;
float ValClamp(float, float, float) noexcept;

#endif

unsigned int ComputeSafeErasePos(char*) noexcept;
unsigned int SafeErasePos(::std::string, unsigned int) noexcept;

#ifndef WIN32

extern "C" void __cxa_pure_virtual();

void* operator new (unsigned int);
void* operator new[](unsigned int);

void operator delete (void*);
void operator delete[](void*);

#endif

#ifdef WIN32

extern bool g_bMultiMediaFeatureIsActive;
extern bool g_bMultiMediaFeatureShouldStopThreadWorker;

extern unsigned int g_uiMultiMediaFeatureTimerResolution;
extern unsigned int g_uiMultiMediaFeatureMinPeriodAvail;

extern void* g_pMultiMediaFeatureThreadWorkerHandle;

#endif

#ifdef WIN32

unsigned long __stdcall MultiMediaFeatureThreadWorkerFunction(void*) noexcept;

void MultiMediaFeatureTryAttach() noexcept;
void MultiMediaFeatureTryDetach() noexcept;

#endif

int numUsersPlaying(int = -1) noexcept;
int numUsersConnected(int = -1) noexcept;
