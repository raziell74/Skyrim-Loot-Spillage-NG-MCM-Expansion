#include "loot.h"

namespace LootSpillage
{
    void LootHandler::DropInventory(Actor *actor)
    {
        using Count = std::int32_t;
        using InventoryItemMap = std::map<TESBoundObject*, std::pair<Count, std::unique_ptr<InventoryEntryData>>>;
        
        if (CreatureFaction && !Settings::IncludeCreatures() && actor->GetFactionRank(CreatureFaction, false) > -1) return;

        float dropped = 0;
        float dropLimit = Settings::GetDropLimit();
        if (Settings::ShouldDropAll()) dropLimit = 0;

        InventoryItemMap inventory = actor->GetInventory();
        for(auto const& [item, inventoryData] : inventory)
        {
            if (dropLimit != 0 && dropped > dropLimit) {
                SKSE::log::info("{} hit the loot drop limit", actor->GetActorBase()->GetName()); 
                break;
            }
            DropLoot(actor, item, inventoryData);
            dropped++;
        }
    }

    bool LootHandler::CanDrop(TESBoundObject *loot)
    {
        if ((loot->GetName() != NULL) && (loot->GetName()[0] == '\0')) return false;
        if (!Settings::ShouldDropUnplayable() && !loot->GetPlayable()) return false;
        if (Settings::AllowItemKeywordExclusion() && loot->HasKeywordInArray(std::vector<BGSKeyword*> {DisableLootDropKYWD}, false)) return false;
        if (Settings::ShouldDropQuest() && loot->IsHeadingMarker()) return false;

        switch(loot->GetFormType())
        {
            case FormType::Armor:
                return Settings::ShouldDropAll() || Settings::ShouldDropArmor();

            case FormType::Weapon:
                return Settings::ShouldDropAll() || Settings::ShouldDropWeapons();
            
            case FormType::Scroll:
            case FormType::Book:
            case FormType::Ingredient:
            case FormType::Light:
            case FormType::Misc:
            case FormType::Apparatus:
            case FormType::Ammo:
            case FormType::AlchemyItem:
            case FormType::Note:
            case FormType::SoulGem:
            case FormType::KeyMaster:
                return Settings::ShouldDropAll() || Settings::ShouldDropOther();
            
            default:
                return false;
        }
    }

    void LootHandler::DropLoot(Actor *actor, TESBoundObject *loot, const std::pair<int32_t, std::unique_ptr<InventoryEntryData>> &inventoryData)
    {
        using Count = std::int32_t;
        using InventoryDropMap = std::map<TESBoundObject*, std::pair<Count, std::vector<ObjectRefHandle>>>;

        if (!CanDrop(loot)) return;
        if (inventoryData.first < 1) return;

        std::string isHeadingMarkerTest = "";
        if (loot->IsHeadingMarker()) isHeadingMarkerTest = "| Loot is Heading Marker";
        
        SKSE::log::info("Dropping {} [{}] {} from {} {}", 
            inventoryData.first, 
            loot->GetFormType(), 
            loot->GetName(), 
            actor->GetActorBase()->GetName(), 
            isHeadingMarkerTest
        );
        
        NiPoint3 position = actor->GetPosition();
        NiPoint3 angle = actor->GetAngle();
        
        float offsetX = Util::Numbers::GenerateRandomFloat(5.0f, 10.0f);
        float offsetY = Util::Numbers::GenerateRandomFloat(5.0f, 10.0f);
        float offsetZ = Util::Numbers::GenerateRandomFloat(5.0f, 10.0f);

        int flipX = Util::Numbers::GenerateRandomInt(0,1);  
        int flipY = Util::Numbers::GenerateRandomInt(0,1); 

        offsetX = (flipX == 1) ? offsetX * -1: offsetX; 
        offsetY = (flipY == 1) ? offsetY * -1 : offsetY; 
        position.x += offsetX; 
        position.y += offsetY;
        position.z += (actor->GetHeight() / 2.0f) + offsetZ;

        auto handle = actor->RemoveItem(loot, inventoryData.first, ITEM_REMOVE_REASON::kDropping, nullptr, actor, &position, &angle);
        if (!handle) return;
        
        std::vector<ObjectRefHandle> arr; 
        arr.emplace_back(handle); 

        std::pair<Count, std::vector<ObjectRefHandle>> pair(inventoryData.first, arr); 
        actor->GetDroppedInventory().emplace(loot, pair);
        
        auto* refr = handle.get().get();
        if (!refr) return;

        // Remove owner so loot that somehow has an owner assigned to it can be picked up by anyone
        refr->extraList.SetOwner(nullptr);

        LootShaders::QueueLootShader(refr);
    }
    
    void LootHandler::CleanUpLoot(NiPointer<TESObjectREFR> refr) {
        using Count = std::int32_t;
        using InventoryDropMap = std::map<TESBoundObject*, std::pair<Count, std::vector<ObjectRefHandle>>>;

        float cleanUpMode = Settings::GetCleanUpMode();
        if (cleanUpMode == 2) { // Do not clean loot
            return;
        }

        BGSListForm* DroppedLootList = Settings::GetDroppedLootList();
        if (!DroppedLootList || DroppedLootList->scriptAddedFormCount == 0) return;

        FormID formId = refr->GetFormID();
        if (!formId || !DroppedLootList->HasForm(formId)) {
            return;
        }
        
        if (refr->IsHeadingMarker()) {
            SKSE::log::info("{} [0x{:X}] is a quest marker - skipping", refr->GetName(), formId);
            return;
        }

        BSTArray<FormID>*  scriptAddedTempForms = DroppedLootList->scriptAddedTempForms;
        if (scriptAddedTempForms->empty()) return;
        
        // Release from the DroppedLootList to free up memory and prevent larger loops when checking HasForm
        std::uint32_t i = 0;
        std::uint32_t count = scriptAddedTempForms->size();
        for (i = 0; i < count; i++) {
            if (formId == scriptAddedTempForms->operator[](i)) {
                scriptAddedTempForms->erase(scriptAddedTempForms->data() + i);
                SKSE::log::info("Removed Form {} [0x{:X}] from SpilledLootList", refr->GetName(), formId);
                break;
            }
        }

        if (cleanUpMode == 1) { // Immediate cleanup on cell detach
            SKSE::log::info("Deleting loot {} [0x{:X}] from the world", refr->GetName(), formId);
            refr->Disable();
            refr->SetDelete(true);
            return;
        }

        // Default clean up mode, move to cleanup chest
        TESObjectREFR* cleanUpContainer = Settings::GetCleanUpContainer();
        Actor* garbageMan = Settings::GetGarbageMan();
        std::int32_t lootCount = refr->extraList.GetCount();
        if (!lootCount || lootCount == 0) lootCount = 1;
        SKSE::log::info("GarbageMan 0x{:X} is collecting {} {} [0x{:X}] to put into the cleanup chest 0x{:X}", garbageMan->GetFormID(), lootCount, refr->GetName(), formId, cleanUpContainer->GetFormID());
        garbageMan->PickUpObject(refr.get(), lootCount, false, false);
        
        RE::ExtraDataList* extraList = &refr->extraList;
        if (!extraList) extraList = nullptr;
        garbageMan->RemoveItem(refr.get()->GetObjectReference(), lootCount, ITEM_REMOVE_REASON::kStoreInContainer, nullptr, cleanUpContainer);

        // @TODO figure out how to place loot back on actor
    }
}
