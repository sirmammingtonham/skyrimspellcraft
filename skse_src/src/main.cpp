#include "common/IDebugLog.h"  // IDebugLog
#include "skse64_common/skse_version.h"  // RUNTIME_VERSION
#include "skse64/PluginAPI.h"  // SKSEInterface, PluginInfo

#include <ShlObj.h>  // CSIDL_MYDOCUMENTS

//#include "version.h"  // VERSION_VERSTRING, VERSION_MAJOR

#include "SpellCraft.h"

static PluginHandle g_handle = kPluginHandle_Invalid;
static SKSEPapyrusInterface* g_papyrus = NULL;
static SKSESerializationInterface* g_serialization = NULL;


extern "C" {
	bool SKSEPlugin_Query(const SKSEInterface* a_skse, PluginInfo* a_info)
	{
		gLog.OpenRelative(CSIDL_MYDOCUMENTS, "\\My Games\\Skyrim Special Edition\\SKSE\\SpellCraft.log");
		gLog.SetPrintLevel(IDebugLog::kLevel_DebugMessage);
		gLog.SetLogLevel(IDebugLog::kLevel_DebugMessage);

		_MESSAGE("SpellCraft Functions v%s", "1.0.1");

		a_info->infoVersion = PluginInfo::kInfoVersion;
		a_info->name = "SpellCraft Functions";
		a_info->version = 1.01;

		if (a_skse->isEditor) {
			_FATALERROR("[FATAL ERROR] Loaded in editor, marking as incompatible!\n");
			return false;
		} else if (a_skse->runtimeVersion != RUNTIME_VERSION_1_5_97) {
			_FATALERROR("[FATAL ERROR] Unsupported runtime version %08X!\n", a_skse->runtimeVersion);
			return false;
		}

		return true;
	}


	bool SKSEPlugin_Load(const SKSEInterface* a_skse)
	{
		g_papyrus = (SKSEPapyrusInterface*) a_skse->QueryInterface(kInterface_Papyrus);
		g_serialization = (SKSESerializationInterface*) a_skse->QueryInterface(kInterface_Serialization);
		g_handle = a_skse->GetPluginHandle();
		if (g_handle == kPluginHandle_Invalid) {
			return false;
		}
		if (!g_serialization || !g_papyrus) {
			return false;
		}

		_MESSAGE("[MESSAGE] SpellCraft loaded");


		bool btest = g_papyrus->Register(SpellCraft::RegisterFuncs);
		if (btest) {
			_MESSAGE("Register Succeeded");
		}
		g_serialization->SetUniqueID(g_handle, 'AL41');
		g_serialization->SetSaveCallback(g_handle, SpellCraft::SaveCallback);
		g_serialization->SetLoadCallback(g_handle, SpellCraft::LoadCallback);
		_MESSAGE("Serialization Set");

		return true;
	}
};
