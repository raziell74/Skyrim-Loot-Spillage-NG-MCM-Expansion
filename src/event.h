#pragma once 
#include "loot.h"
#include "shaders.h"
#include "util.h"
using namespace RE; 
namespace LootSpillage
{
    class DeathEventHandler : public RE::BSTEventSink<RE::TESDeathEvent>
    {

        public: 
        static void Install()
        {
            ScriptEventSourceHolder::GetSingleton()->GetEventSource<TESDeathEvent>()->AddEventSink(GetSingleton()); 
            SKSE::log::info("Registered {}", typeid(RE::TESDeathEvent).name()); 
        }
        static DeathEventHandler* GetSingleton()
        {
            static DeathEventHandler singleton; 
            return &singleton; 
        }

         virtual RE::BSEventNotifyControl ProcessEvent(const TESDeathEvent *a_event, RE::BSTEventSource<TESDeathEvent> *a_eventSource) override;
    };

    class CellLoadEventHandler : public RE::BSTEventSink<RE::TESCellAttachDetachEvent>
    {

        public: 
        static void Install() {
            ScriptEventSourceHolder::GetSingleton()->GetEventSource<TESCellAttachDetachEvent>()->AddEventSink(GetSingleton()); 
            SKSE::log::info("Registered {}", typeid(RE::TESCellAttachDetachEvent).name()); 
        }
        static CellLoadEventHandler* GetSingleton() {
            static CellLoadEventHandler singleton; 
            return &singleton; 
        }

        virtual RE::BSEventNotifyControl ProcessEvent(const TESCellAttachDetachEvent *a_event, RE::BSTEventSource<TESCellAttachDetachEvent> *a_eventSource) override;
    };
}