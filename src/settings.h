#pragma once

#include "SimpleIni.h"
#include "util.h"

using namespace RE; 
namespace LootSpillage
{
    class Settings
    {
        public:

            struct DroppedLoot
            {
                bool DropAll = true;
                bool DropArmor = false;
                bool DropWeapons = false;
                bool DropOther = false;
                bool DropUnplayable = false;
                bool AutoLootGold = false;
                bool AllowItemKeywordExclusion = false;
                float DropLimit = 15;
                bool DropQuest = false;
            };

            struct ActorTargets
            {
                bool FilterByPlayerKills = true;
                bool FilterByFollowerKills = true;
                bool IncludeCreatures = false;
                bool AllowActorKeywordExclusion = false;
            };

            struct Shaders
            {
                float Duration = 60.0f; 
                float FallOff = 1.0f; 
                float ApplyDelay = 300.0f;

                uint32_t BaseColor = 0xffffff;
                uint32_t WeaponColor = 0xfa6502; 
                uint32_t ArmorColor = 0x02e5fa;
                uint32_t ConsumableColor = 0x02fa8b;
                uint32_t ValuableColor = 0xfad502;
            };

            static void Load()
            {
                constexpr auto path = L"Data/SKSE/Plugins/LootSpillage/Settings.ini";

                CSimpleIniA ini;
                ini.SetUnicode();

                SI_Error status = ini.LoadFile(path);

                if (status < 0) 
                {
                    SKSE::log::info("Settings load failed");
                }
                else 
                {
                    SKSE::log::info("Settings loaded successfully");
                }
                
                DropOptions.DropAll = ini.GetBoolValue("Drops", "DropAll", true);
                DropOptions.DropArmor = ini.GetBoolValue("Drops", "DropArmor", false);
                DropOptions.DropWeapons  = ini.GetBoolValue("Drops", "DropWeapons", false);
                DropOptions.DropOther = ini.GetBoolValue("Drops", "DropOther", false);
                DropOptions.DropQuest = ini.GetDoubleValue("Drops", "DropLimit", 0);
                DropOptions.DropUnplayable = ini.GetBoolValue("Drops", "DropUnplayable", false);
                DropOptions.DropQuest = ini.GetBoolValue("Drops", "DropQuest", false);
                DropOptions.AllowItemKeywordExclusion = ini.GetBoolValue("Drops", "AllowItemKeywordExclusion", true);

                ActorOptions.IncludeCreatures = ini.GetBoolValue("Actors", "CreatureDrops", false);  
                ActorOptions.AllowActorKeywordExclusion = ini.GetBoolValue("Actors", "AllowActorKeywordExclusion", true);
                ActorOptions.FilterByPlayerKills = ini.GetBoolValue("Actors", "FilterByPlayerKills", true);
                ActorOptions.FilterByFollowerKills = ini.GetBoolValue("Actors", "FilterByFollowerKills", true);

                ShaderOptions.Duration = static_cast<float>(ini.GetDoubleValue("Shaders", "VfxDuration", 60.0)); 
                ShaderOptions.FallOff = static_cast<float>(ini.GetDoubleValue("Shaders", "VfxFallOff", 1.0)); 
                ShaderOptions.ApplyDelay = static_cast<float>(ini.GetDoubleValue("Shaders", "ApplyDelay", 300.0f));
                
                SKSE::log::info(
                    "DropOptions | DropAll: {} | DropArmor: {} | DropWeapons: {} | DropOther: {} | DropLimit: {} | DropQuest: {} | AllowItemKeywordExclusion {}", 
                    DropOptions.DropAll, 
                    DropOptions.DropArmor, 
                    DropOptions.DropWeapons, 
                    DropOptions.DropOther, 
                    DropOptions.DropLimit,
                    DropOptions.DropQuest,
                    DropOptions.AllowItemKeywordExclusion
                );
                SKSE::log::info(
                    "ActorOptions | IncludeCreatures: {} | AllowActorKeywordExclusion: {} | FilterByPlayerKills: {} | FilterByFollowerKills: {}", 
                    ActorOptions.IncludeCreatures, 
                    ActorOptions.AllowActorKeywordExclusion,
                    ActorOptions.FilterByPlayerKills,
                    ActorOptions.FilterByFollowerKills
                );
                SKSE::log::info(
                    "ShaderOptions | Duration: {} | FallOff: {} | ApplyDelay: {}", 
                    ShaderOptions.Duration, 
                    ShaderOptions.FallOff, 
                    ShaderOptions.ApplyDelay
                );
            }

            static void LoadOverrides()
            {
                TESGlobal* DropAll = FormUtil::Form::GetFormFromMod("LootSpillageMCM.esp", 0x801)->As<TESGlobal>();
                TESGlobal* DropArmor = FormUtil::Form::GetFormFromMod("LootSpillageMCM.esp", 0x802)->As<TESGlobal>();
                TESGlobal* DropWeapons = FormUtil::Form::GetFormFromMod("LootSpillageMCM.esp", 0x803)->As<TESGlobal>();
                TESGlobal* DropOther = FormUtil::Form::GetFormFromMod("LootSpillageMCM.esp", 0x804)->As<TESGlobal>();
                TESGlobal* DropUnplayable = FormUtil::Form::GetFormFromMod("LootSpillageMCM.esp", 0x805)->As<TESGlobal>();
                TESGlobal* AllowItemKeywordExclusion = FormUtil::Form::GetFormFromMod("LootSpillageMCM.esp", 0x80C)->As<TESGlobal>();
                TESGlobal* DropLimit = FormUtil::Form::GetFormFromMod("LootSpillageMCM.esp", 0x813)->As<TESGlobal>();
                TESGlobal* DropQuest = FormUtil::Form::GetFormFromMod("LootSpillageMCM.esp", 0x814)->As<TESGlobal>();

                DropOptions.DropAll = DropAll->value >= 1;
                DropOptions.DropArmor = DropArmor->value >= 1;
                DropOptions.DropWeapons = DropWeapons->value >= 1;
                DropOptions.DropOther = DropOther->value >= 1;
                DropOptions.DropUnplayable = DropUnplayable->value >= 1;
                DropOptions.AllowItemKeywordExclusion = AllowItemKeywordExclusion->value >= 1;
                DropOptions.DropLimit = DropLimit->value;
                DropOptions.DropQuest = DropQuest->value >= 1;

                TESGlobal* IncludeCreatures = FormUtil::Form::GetFormFromMod("LootSpillageMCM.esp", 0x806)->As<TESGlobal>();
                TESGlobal* AllowActorKeywordExclusion = FormUtil::Form::GetFormFromMod("LootSpillageMCM.esp", 0x80D)->As<TESGlobal>();
                TESGlobal* FilterByPlayerKills = FormUtil::Form::GetFormFromMod("LootSpillageMCM.esp", 0x817)->As<TESGlobal>();
                TESGlobal* FilterByFollowerKills = FormUtil::Form::GetFormFromMod("LootSpillageMCM.esp", 0x818)->As<TESGlobal>();

                ActorOptions.IncludeCreatures = IncludeCreatures->value >= 1;
                ActorOptions.AllowActorKeywordExclusion = AllowActorKeywordExclusion->value >= 1;
                ActorOptions.FilterByPlayerKills = FilterByPlayerKills->value >= 1;
                ActorOptions.FilterByFollowerKills = FilterByFollowerKills->value >= 1;
                
                TESGlobal* Duration = FormUtil::Form::GetFormFromMod("LootSpillageMCM.esp", 0x808)->As<TESGlobal>();
                TESGlobal* FallOff = FormUtil::Form::GetFormFromMod("LootSpillageMCM.esp", 0x809)->As<TESGlobal>();
                TESGlobal* ApplyDelay = FormUtil::Form::GetFormFromMod("LootSpillageMCM.esp", 0x807)->As<TESGlobal>();

                ShaderOptions.Duration = Duration->value;
                ShaderOptions.FallOff = FallOff->value;
                ShaderOptions.ApplyDelay = ApplyDelay->value;

                TESGlobal* BaseColor = FormUtil::Form::GetFormFromMod("LootSpillageMCM.esp", 0x80E)->As<TESGlobal>();
                TESGlobal* WeaponColor = FormUtil::Form::GetFormFromMod("LootSpillageMCM.esp", 0x80F)->As<TESGlobal>();
                TESGlobal* ArmorColor = FormUtil::Form::GetFormFromMod("LootSpillageMCM.esp", 0x810)->As<TESGlobal>();
                TESGlobal* ConsumableColor = FormUtil::Form::GetFormFromMod("LootSpillageMCM.esp", 0x811)->As<TESGlobal>();
                TESGlobal* ValuableColor = FormUtil::Form::GetFormFromMod("LootSpillageMCM.esp", 0x812)->As<TESGlobal>();

                ShaderOptions.BaseColor = (uint32_t)BaseColor->value;
                ShaderOptions.ArmorColor = (uint32_t)ArmorColor->value;
                ShaderOptions.WeaponColor = (uint32_t)WeaponColor->value;
                ShaderOptions.ConsumableColor = (uint32_t)ConsumableColor->value;
                ShaderOptions.ValuableColor = (uint32_t)ValuableColor->value;

                SKSE::log::info("Settings Override from MCM");
                SKSE::log::info(
                    "DropOptions | DropAll: {} | DropArmor: {} | DropWeapons: {} | DropOther: {} | DropLimit: {} | DropQuest: {} | AllowItemKeywordExclusion {}", 
                    DropOptions.DropAll, 
                    DropOptions.DropArmor, 
                    DropOptions.DropWeapons, 
                    DropOptions.DropOther, 
                    DropOptions.DropLimit,
                    DropOptions.DropQuest,
                    DropOptions.AllowItemKeywordExclusion
                );
                SKSE::log::info(
                    "ActorOptions | IncludeCreatures: {} | AllowActorKeywordExclusion: {} | FilterByPlayerKills: {} | FilterByFollowerKills: {}", 
                    ActorOptions.IncludeCreatures, 
                    ActorOptions.AllowActorKeywordExclusion,
                    ActorOptions.FilterByPlayerKills,
                    ActorOptions.FilterByFollowerKills
                );
                SKSE::log::info(
                    "ShaderOptions | Duration: {} | FallOff: {} | ApplyDelay: {}", 
                    ShaderOptions.Duration, 
                    ShaderOptions.FallOff, 
                    ShaderOptions.ApplyDelay
                );
            }

            [[nodiscard]] static bool ShouldDropAll() { return DropOptions.DropAll; }

            [[nodiscard]] static bool ShouldDropArmor() { return DropOptions.DropArmor; }

            [[nodiscard]] static bool ShouldDropWeapons() { return DropOptions.DropWeapons; }

            [[nodiscard]] static bool ShouldDropUnplayable() { return DropOptions.DropUnplayable; }

            [[nodiscard]] static bool ShouldDropOther() { return DropOptions.DropOther; }

            [[nodiscard]] static bool ShouldLootGold() { return DropOptions.AutoLootGold; }
            
            [[nodiscard]] static bool AllowItemKeywordExclusion() { return DropOptions.AllowItemKeywordExclusion; }

            [[nodiscard]] static float GetDropLimit() { return DropOptions.DropLimit; }

            [[nodiscard]] static bool ShouldDropQuest() { return DropOptions.DropQuest; }

            [[nodiscard]] static bool FilterByPlayerKills() { return ActorOptions.FilterByPlayerKills; }

            [[nodiscard]] static bool FilterByFollowerKills() { return ActorOptions.FilterByFollowerKills; }

            [[nodiscard]] static bool IncludeCreatures() { return ActorOptions.IncludeCreatures; }

            [[nodiscard]] static bool AllowActorKeywordExclusion() { return ActorOptions.AllowActorKeywordExclusion; }

            [[nodiscard]] static float GetShaderDuration() { return ShaderOptions.Duration; }

            [[nodiscard]] static float GetShaderDelay() { return ShaderOptions.ApplyDelay; }

            [[nodiscard]] static float GetShaderFallOff() { return ShaderOptions.FallOff; }

            [[nodiscard]] static Color GetBaseShaderColor() { return Color(ShaderOptions.BaseColor); }

            [[nodiscard]] static Color GetArmorShaderColor() { return Color(ShaderOptions.ArmorColor); }

            [[nodiscard]] static Color GetWeaponShaderColor() { return Color(ShaderOptions.WeaponColor); }

            [[nodiscard]] static Color GetConsumableShaderColor() { return Color(ShaderOptions.ConsumableColor); }

            [[nodiscard]] static Color GetValuableShaderColor() { return Color(ShaderOptions.ValuableColor); }

        private: 

            static inline DroppedLoot DropOptions;
            static inline Shaders ShaderOptions;
            static inline ActorTargets ActorOptions;
    };   
}