
#include "event.h"

namespace LootSpillage
{
    RE::BSEventNotifyControl DeathEventHandler::ProcessEvent(const TESDeathEvent *a_event, RE::BSTEventSource<TESDeathEvent> *a_eventSource) {
        using Result = RE::BSEventNotifyControl;

        auto* ref = a_event->actorDying.get();
        if (!ref || ref->GetFormType() != FormType::ActorCharacter || !ref->Is3DLoaded() || !a_event->dead) return Result::kContinue;

        auto* actor = ref->As<Actor>(); 
        if (!actor || actor->IsEssential()) return Result::kContinue;

        if (actor->IsPlayerRef()) return Result::kContinue;
        if (Settings::AllowActorKeywordExclusion() && actor->HasKeywordString("DisableLootDrop")) return Result::kContinue;

        if (Settings::FilterByPlayerKills() || Settings::FilterByFollowerKills()) {
            auto killerRef = a_event->actorKiller.get();
            if (!killerRef || killerRef->GetFormType() != FormType::ActorCharacter) return Result::kContinue;
            
            auto* killer = killerRef->As<Actor>();
            if (!killer) return Result::kContinue;

            // Only Player Kills
            if (Settings::FilterByPlayerKills() && !Settings::FilterByFollowerKills() && !killer->IsPlayerRef()) {
                SKSE::log::info("{}'s Killer is not the Player", actor->GetActorBase()->GetName());
                return Result::kContinue;
            }
            
            // Only Player or Follower Kills
            if (Settings::FilterByPlayerKills() && Settings::FilterByFollowerKills() && !killer->IsPlayerRef() && !killer->IsPlayerTeammate()) {
                SKSE::log::info("{}'s Killer is not the Player or a Follower", actor->GetActorBase()->GetName());
                return Result::kContinue;
            }
        }

        SKSE::log::info("Actor {} died", actor->GetActorBase()->GetName()); 
        LootHandler::DropInventory(actor); 

        return Result::kContinue; 
    }

    RE::BSEventNotifyControl CellLoadEventHandler::ProcessEvent(const TESCellAttachDetachEvent *a_event, RE::BSTEventSource<TESCellAttachDetachEvent> *a_eventSource) {
        using Result = RE::BSEventNotifyControl;
        
        if (a_event->attached) {
            // Skip if the cell is attaching
            return Result::kContinue; 
        }

        NiPointer<TESObjectREFR> refr = a_event->reference;
        LootHandler::CleanUpLoot(refr);

        return Result::kContinue;
    }
}