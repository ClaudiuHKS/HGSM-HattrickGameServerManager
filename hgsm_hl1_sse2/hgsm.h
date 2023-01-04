
#pragma once

#include "nlohmann/json.hpp"

#include <fstream>

#ifndef WIN32

#include <sys/resource.h>

#include <dirent.h>

#else

#include <Windows.h>

#endif

#include <extdll.h>
#include <meta_api.h>

#ifdef open

#undef open

#endif

#ifdef close

#undef close

#endif

#ifdef snprintf

#undef snprintf

#endif

#define xName "Hattrick Game Server Manager"

#define xTo( Var, Type ) ( ( Type ) ( Var ) )

extern ::plugin_info_t Plugin_info;

extern ::globalvars_t* gpGlobals;
extern ::enginefuncs_t g_engfuncs;

extern ::meta_globals_t* gpMetaGlobals;
extern ::mutil_funcs_t* gpMetaUtilFuncs;
extern ::gamedll_funcs_t* gpGamedllFuncs;

extern ::DLL_FUNCTIONS gFunctionTable;
extern ::DLL_FUNCTIONS* gpFunctionTable;

extern ::DLL_FUNCTIONS gFunctionTable_Post;
extern ::DLL_FUNCTIONS* gpFunctionTable_Post;

extern ::enginefuncs_t gEngFuncTable;
extern ::enginefuncs_t* gpEngFuncTable;

extern ::enginefuncs_t gEngFuncTable_Post;
extern ::enginefuncs_t* gpEngFuncTable_Post;

extern ::META_FUNCTIONS gMetaFunctionTable;
extern ::META_FUNCTIONS* gpMetaFunctionTable;

extern ::cvar_t g_cvVersion;

extern ::std::string g_strGameDir;

extern ::std::string g_strWelcome_1;
extern ::std::string g_strWelcome_2;
extern ::std::string g_strWelcome_3;
extern ::std::string g_strWelcome_4;
extern ::std::string g_strWelcome_5;
extern ::std::string g_strWelcome_6;

extern ::std::string g_strNotify_1;
extern ::std::string g_strNotify_2;

#ifdef WIN32

extern ::HWND__* g_pWnd;

#endif

extern bool g_bShow[48];

extern bool g_bHide[48];
extern bool g_bArmor[48];
extern bool g_bRadar[48];

extern bool g_bAttached;

extern bool g_bLateLoaded;
extern bool g_bNoReturn;

extern bool g_bResetHud;
extern bool g_bDeathMsg;

extern bool g_bNoWeapons;

extern bool g_bStripper;

extern bool g_bKvEditor;
extern bool g_bKvAdder;

extern bool g_bHideRadar;
extern bool g_bHideMoney;
extern bool g_bHideTimer;
extern bool g_bHideFlash;

extern bool g_bRealScoreFriendly;
extern bool g_bRealScore;
extern bool g_bZeroMoney;
extern bool g_bFixScore;
extern bool g_bFixDeaths;
extern bool g_bFixKills;
extern bool g_bKeepHelmet;

extern bool g_bOneReliable;
extern bool g_bAllReliable;

extern int g_nDeaths[48];
extern int g_nKills[48];

extern int g_nDeathMsg;
extern int g_nScoreShort;
extern int g_nTextMsg;
extern int g_nHideWeapon;
extern int g_nScoreInfo;
extern int g_nCrosshair;
extern int g_nArmorType;
extern int g_nResetHud;

extern int g_nArmor;

extern int g_nTicks;

extern int g_nResetHudId;

extern int g_nDeathMsgByte;

extern int g_nOsOffs;

extern int g_nKiller;
extern int g_nVictim;

extern int m_iWindows;
extern int m_iLinux;
extern int m_iDodTeam;
extern int m_iDodScore;
extern int m_iDodDeaths;
extern int m_iKevlar;
extern int m_iTeam;
extern int m_iMoney;
extern int m_iFov;
extern int m_iDeaths;

#ifdef WIN32

extern unsigned long g_ulProc;

#endif

C_DLLEXPORT void WINAPI GiveFnptrsToDll(::enginefuncs_t*, ::globalvars_t*) noexcept;

C_DLLEXPORT int GetEntityAPI2(::DLL_FUNCTIONS*, int*);
C_DLLEXPORT int GetEntityAPI2_Post(::DLL_FUNCTIONS*, int*);

C_DLLEXPORT int GetEngineFunctions(::enginefuncs_t*, int*);
C_DLLEXPORT int GetEngineFunctions_Post(::enginefuncs_t*, int*);

C_DLLEXPORT void Meta_Init();

C_DLLEXPORT int Meta_Query(const char*, ::plugin_info_t**, ::mutil_funcs_t*);

C_DLLEXPORT int Meta_Attach(::PLUG_LOADTIME, ::META_FUNCTIONS*, ::meta_globals_t*, ::gamedll_funcs_t*);
C_DLLEXPORT int Meta_Detach(::PLUG_LOADTIME, ::PL_UNLOAD_REASON);

int Hook_Spawn(::edict_t*) noexcept;
void Hook_ServerActivate(::edict_t*, int, int) noexcept;
void Hook_KeyValue(::edict_t*, ::KeyValueData*) noexcept;
void Hook_ClientDisconnect(::edict_t*) noexcept;


#ifdef WIN32

constexpr void Hook_ServerDeactivate_Post() noexcept;

#else

void Hook_ServerDeactivate_Post() noexcept;

#endif

void Hook_ServerActivate_Post(::edict_t*, int, int) noexcept;
void Hook_StartFrame_Post() noexcept;
void Hook_ClientPutInServer_Post(::edict_t*) noexcept;
void Hook_ClientDisconnect_Post(::edict_t*) noexcept;

void Hook_MessageBegin_Post(int, int, const float*, ::edict_t*) noexcept;

#ifdef WIN32

constexpr void Hook_WriteByte_Post(int) noexcept;

#else

void Hook_WriteByte_Post(int) noexcept;

#endif

void Hook_MessageEnd_Post() noexcept;

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
