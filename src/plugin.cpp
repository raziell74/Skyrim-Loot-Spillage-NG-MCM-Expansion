#include "log.h"
#include "event.h"
#include "settings.h"
#include "shaders.h"
#include "hook.h"
void OnDataLoaded()
{
   
}

void MessageHandler(SKSE::MessagingInterface::Message* a_msg)
{
	switch (a_msg->type) {
	case SKSE::MessagingInterface::kDataLoaded:
        LootSpillage::DeathEventHandler::GetSingleton()->Install(); 
		LootSpillage::CellLoadEventHandler::GetSingleton()->Install();
		LootSpillage::Settings::Load(); 
		LootSpillage::LootShaders::Load(); 
		LootSpillage::LootShaders::Configure(); 
		LootSpillage::LootHandler::LoadReferences();
		break;
	case SKSE::MessagingInterface::kPostLoad:
		break;
	case SKSE::MessagingInterface::kPreLoadGame:
		break;
	case SKSE::MessagingInterface::kPostLoadGame:
		SKSE::log::info("Tick count {}", BGSSaveLoadManager::GetSingleton()->tickCount);
		LootSpillage::LootShaders::Configure();
		break;
	case SKSE::MessagingInterface::kNewGame:
		LootSpillage::LootShaders::Configure(); 
		break;
	}
}

void CleanSpilledLoot(RE::StaticFunctionTag*) { 
	// LootSpillage::LootHandler::CleanUpLoot();
}

void UpdateLootSpillageSettings(RE::StaticFunctionTag*) { 
	LootSpillage::Settings::LoadOverrides();
	LootSpillage::LootShaders::Configure();
}

bool BindPapyrusFunctions(RE::BSScript::IVirtualMachine* vm) {
    vm->RegisterFunction("UpdateLootSpillageSettings", "LootSpillageMCM", UpdateLootSpillageSettings);
	vm->RegisterFunction("CleanSpilledLoot", "LootSpillageMCM", CleanSpilledLoot);
    return true;
}

SKSEPluginLoad(const SKSE::LoadInterface *skse) {
    SKSE::Init(skse);
	SetupLog();

    auto messaging = SKSE::GetMessagingInterface();
	if (!messaging->RegisterListener("SKSE", MessageHandler)) {
		return false;
	}

	SKSE::GetPapyrusInterface()->Register(BindPapyrusFunctions);
	
    return true;
}
