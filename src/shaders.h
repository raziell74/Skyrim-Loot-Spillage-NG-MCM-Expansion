#pragma once 
using namespace RE;

#include "util.h"
#include <shared_mutex>
#include "settings.h"
#include <unordered_set>
namespace LootSpillage
{
    class LootShaders 
    {
        using Shader = TESEffectShader; 

        public: 
        
            static void Load();

            static void Configure();
            
            static void QueueLootShader(TESObjectREFR* refr);

            static void ApplyDelayedShader(FormID a_formID);
        
            static void ApplyLootShader(TESObjectREFR* refr);

            [[nodiscard]] static Shader* GetRarityShader(std::string type, FormType formType) {
                if (type == "Common") return CommonShader;
                if (type == "Uncommon") return UncommonShader;
                if (type == "Rare") return RareShader;
                if (type == "Epic") return EpicShader;
                if (type == "Legendary") return LegendaryShader;
                if (formType == FormType::Armor) return ArmorShader;
                if (formType == FormType::Weapon) return WeaponShader;
                return BaseShader;
            }

            [[nodiscard]] static std::string GetRarity(TESObjectREFR* refr) {
                if (!refr) return "";
                TESBoundObject* loot = refr->GetBaseObject();
                if (loot->HasKeywordInArray(std::vector<BGSKeyword*> {CommonKywd}, false)) return "Common";
                if (loot->HasKeywordInArray(std::vector<BGSKeyword*> {UncommonKywd}, false)) return "Uncommon";
                if (loot->HasKeywordInArray(std::vector<BGSKeyword*> {RareKywd}, false)) return "Rare";
                if (loot->HasKeywordInArray(std::vector<BGSKeyword*> {EpicKywd}, false)) return "Epic";
                if (loot->HasKeywordInArray(std::vector<BGSKeyword*> {LegendaryKywd}, false)) return "Legendary";
                return "";
            }

        private:

            using Lock = std::shared_mutex;
            using ReadLocker = std::shared_lock<Lock>;
            using WriteLocker = std::unique_lock<Lock>;

            static void ConfigureShader(TESEffectShader* shader, Color color, float fallOff, std::string name);

            static inline Lock dataLock;
            static inline Shader* BaseShader;
            static inline Shader* ConsumableShader;
            static inline Shader* ValuableShader;
            static inline Shader* WeaponShader;
            static inline Shader* ArmorShader;
            static inline Shader* CommonShader;
            static inline Shader* UncommonShader;
            static inline Shader* RareShader;
            static inline Shader* EpicShader;
            static inline Shader* LegendaryShader;
            static inline std::vector<BGSKeyword*> ValuableKeywords;
            static inline BGSKeyword* CommonKywd;
            static inline BGSKeyword* UncommonKywd;
            static inline BGSKeyword* RareKywd;
            static inline BGSKeyword* EpicKywd;
            static inline BGSKeyword* LegendaryKywd;
            static inline float Duration;
            static inline float Delay;
    };
}