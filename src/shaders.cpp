#include "shaders.h"
#include <chrono>

namespace LootSpillage
{
    void LootShaders::Load()
    {
        BaseShader =  FormUtil::Form::GetFormFromMod("LootSpillage.esp", 0x800)->As<TESEffectShader>();
        WeaponShader = FormUtil::Form::GetFormFromMod("LootSpillage.esp", 0x803)->As<TESEffectShader>();
        ArmorShader = FormUtil::Form::GetFormFromMod("LootSpillage.esp", 0x804)->As<TESEffectShader>();
        ValuableShader = FormUtil::Form::GetFormFromMod("LootSpillage.esp", 0x802)->As<TESEffectShader>();
        ConsumableShader = FormUtil::Form::GetFormFromMod("LootSpillage.esp", 0x801)->As<TESEffectShader>();
    }

    void LootShaders::Configure()
    {
        WriteLocker locker(dataLock);
        float fallOff = Settings::GetShaderFallOff();
        Duration = Settings::GetShaderDuration();
        Delay = Settings::GetShaderDelay();
        
        ConfigureShader(BaseShader, Settings::GetBaseShaderColor(), fallOff, "BaseShader");
        ConfigureShader(WeaponShader, Settings::GetWeaponShaderColor(), fallOff, "WeaponShader");
        ConfigureShader(ArmorShader, Settings::GetArmorShaderColor(), fallOff, "ArmorShader");
        ConfigureShader(ValuableShader, Settings::GetValuableShaderColor(), fallOff, "ValuableShader");
        ConfigureShader(ConsumableShader, Settings::GetConsumableShaderColor(), fallOff, "ConsumableShader");
    }

    void LootShaders::ConfigureShader(TESEffectShader *shader, Color color, float fallOff, std::string name)
    {
        shader->data.edgeEffectFallOff = fallOff;
        shader->data.edgeEffectColor = color;
        shader->SaveObjectBound();

        SKSE::log::info("{} color r{} g{} b{}", 
            name, 
            shader->data.edgeEffectColor.red, 
            shader->data.edgeEffectColor.green, 
            shader->data.edgeEffectColor.blue
        );
    }

    void LootShaders::QueueLootShader(TESObjectREFR *refr)
    {
        if (Duration == 0.0f) return;
        std::jthread wait(ApplyDelayedShader, refr->GetFormID()); 
        wait.detach(); 
    }

    void LootShaders::ApplyDelayedShader(FormID a_formID)
    {
        int delay = (int)Settings::GetShaderDelay();
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));

        auto* refr = TESForm::LookupByID<TESObjectREFR>(a_formID);
        if (!refr) return;

        int maxWaitTime = 5;
        int waitTime = 0;
        while(!refr->Is3DLoaded() && waitTime < maxWaitTime)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            waitTime++;
        }

        if (!refr->Is3DLoaded())
        {
            SKSE::log::info("Failed to load 3D for {}", refr->GetName());
            return;
        }

        ApplyLootShader(refr);
    }

    void LootShaders::ApplyLootShader(TESObjectREFR *refr)
    {
        if (!refr) return;
        if (!refr->Is3DLoaded())
        {
            SKSE::log::info("Failed to load 3D for {}", refr->GetName());
            return;
        }

        ReadLocker locker(dataLock);
        auto* baseObject = refr->GetBaseObject();
        if (!baseObject) return;

        TESEffectShader* shader;
        std::string shaderType = "";
        switch(baseObject->GetFormType())
        {
            case FormType::AlchemyItem:
                shader = ConsumableShader; 
                shaderType = "ConsumableShader";
                break;
            case FormType::Ingredient:
                shader = ConsumableShader;
                shaderType = "ConsumableShader";
                break;
            case FormType::Armor:
                shader = ArmorShader;
                shaderType = "ArmorShader";
                break;
            case FormType::Weapon:
                shader = WeaponShader;
                shaderType = "WeaponShader";
                break; 
            case FormType::KeyMaster:
                shader = ValuableShader;
                shaderType = "ValuableShader";
                break;
            default:
                shader = BaseShader;
                shaderType = "BaseShader";
        }
        if (baseObject->IsGold() || baseObject->IsSoulGem()) {
            shader = ValuableShader;
            shaderType = "ValuableShader";
        }

        SKSE::log::info("Applying {} to {}", 
            shaderType, 
            refr->GetName()
        );

        if (Duration <= -1.0f) {
            refr->ApplyEffectShader(shader);
        } else {
            refr->ApplyEffectShader(shader, Duration);
        }

        // Add the dropped ref formId to the DroppedLootList form list for later cleanup
        BGSListForm* DroppedLootList = Settings::GetDroppedLootList();
        SKSE::log::info("Adding {} {} [0x{:X}] to the DroppedLootList", refr->extraList.GetCount(), refr->GetName(), refr->GetFormID());
        DroppedLootList->AddForm(refr);
    }
}
