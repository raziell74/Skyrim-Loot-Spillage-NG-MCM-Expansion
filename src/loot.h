#pragma once 
#include "settings.h"
#include "util.h"
#include "shaders.h"
using namespace RE;

namespace LootSpillage
{
    class LootHandler
    {
        public: 
                static void LoadReferences()
                {
                    CreatureFaction = TESForm::LookupByID(0x13)->As<TESFaction>();
                    DisableLootDropKYWD = FormUtil::Form::GetFormFromMod("LootSpillageMCM.esp", 0x80A)->As<BGSKeyword>(); 
                }

                static void DropInventory(Actor* actor);

            private:

                static bool CanDrop(TESBoundObject* loot);

                static void DropLoot(Actor *actor, TESBoundObject *loot, const std::pair<int32_t, std::unique_ptr<InventoryEntryData>> &inventoryData);
            
            static inline TESFaction* CreatureFaction;
            static inline BGSKeyword* DisableLootDropKYWD;
    };
}