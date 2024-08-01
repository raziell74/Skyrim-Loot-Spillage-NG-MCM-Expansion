#pragma once 
using namespace RE;

#include "util.h"
#include <shared_mutex>
#include "settings.h"
#include <unordered_set>
namespace LootSpillage
{
    enum class RarityType
	{
        None = 0,
		Common,
		Uncommon,
		Rare,
		Epic,
        Legendary,
        Artifact
	};

    class LootShaders 
    {
        using Shader = TESEffectShader; 
        using RarityType = LootSpillage::RarityType; 

        public: 
        
            static void Load();

            static void Configure();
            
            static void QueueLootShader(TESObjectREFR* refr);

            static void ApplyDelayedShader(FormID a_formID);
        
            static void ApplyLootShader(TESObjectREFR* refr);

            [[nodiscard]] static Shader* GetRarityShader(RarityType rarity, FormType formType) {
                if (rarity == RarityType::Common) return CommonShader;
                if (rarity == RarityType::Uncommon) return UncommonShader;
                if (rarity == RarityType::Rare) return RareShader;
                if (rarity == RarityType::Epic) return EpicShader;
                if (rarity == RarityType::Legendary) return LegendaryShader;
                if (rarity == RarityType::Artifact) return ArtifactShader;
                // if (formType == FormType::Armor) return ArmorShader;
                // if (formType == FormType::Weapon) return WeaponShader;
                return BaseShader;
            }

            [[nodiscard]] static RarityType GetRarity(TESObjectREFR* refr) {
                if (!refr) return RarityType::None;
                TESBoundObject* loot = refr->GetBaseObject();
                if (loot->HasKeywordInArray(std::vector<BGSKeyword*> {ArtifactKywd}, false)) return RarityType::Artifact;
                if (loot->HasKeywordInArray(std::vector<BGSKeyword*> {LegendaryKywd}, false)) return RarityType::Legendary;
                if (loot->HasKeywordInArray(std::vector<BGSKeyword*> {EpicKywd}, false)) return RarityType::Epic;
                if (loot->HasKeywordInArray(std::vector<BGSKeyword*> {RareKywd}, false)) return RarityType::Rare;
                if (loot->HasKeywordInArray(std::vector<BGSKeyword*> {UncommonKywd}, false)) return RarityType::Uncommon;
                if (loot->HasKeywordInArray(std::vector<BGSKeyword*> {CommonKywd}, false)) return RarityType::Common;
                return RarityType::None;
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
            static inline Shader* ArtifactShader;
            static inline std::vector<BGSKeyword*> ValuableKeywords;
            static inline BGSKeyword* CommonKywd;
            static inline BGSKeyword* UncommonKywd;
            static inline BGSKeyword* RareKywd;
            static inline BGSKeyword* EpicKywd;
            static inline BGSKeyword* LegendaryKywd;
            static inline BGSKeyword* ArtifactKywd;
            static inline float Duration;
            static inline float Delay;
    };
}